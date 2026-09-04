#!/usr/bin/env python3
"""Diagnose UFC 3 indirect bctr targets that are likely jump tables.

The tool intentionally works from XenonRecomp's generated PPC artefacts first:
the ppc_recomp.*.cpp files preserve enough original PPC comments and internal
loc_* labels to recover many switch tables without adding runtime thunks.
"""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PPC_OUTPUT = ROOT / "ppc_output"
OUTPUT_DIR = ROOT / "tools" / "output"


FUNC_RE = re.compile(r"^PPC_FUNC_IMPL\(__imp__(sub_[0-9A-Fa-f]{8})\)")
LOC_RE = re.compile(r"^loc_([0-9A-Fa-f]{8}):")
COMMENT_RE = re.compile(r"^\s*//\s*(.+?)\s*$")
CMPLWI_RE = re.compile(r"cmplwi\s+cr\d+,r(\d+),(\d+)", re.IGNORECASE)
BGT_RE = re.compile(r"bgt\s+cr\d+,0x([0-9A-Fa-f]+)", re.IGNORECASE)
LIS_RE = re.compile(r"lis\s+r(\d+),(-?\d+)", re.IGNORECASE)
ADDI_RE = re.compile(r"addi\s+r(\d+),r\1,(-?\d+)", re.IGNORECASE)
RLWINM_RE = re.compile(r"rlwinm\s+r\d+,r(\d+),2,0,29", re.IGNORECASE)
LWZ_RE = re.compile(r"lwz\s+r(\d+),(-?\d+)\(r(\d+)\)", re.IGNORECASE)
CASE_RE = re.compile(r"^\s*case\s+\d+:\s*$")
GOTO_RE = re.compile(r"^\s*goto\s+loc_([0-9A-Fa-f]{8});")


@dataclass
class FunctionBlock:
    name: str
    file: Path
    start_line: int
    end_line: int
    lines: list[str]
    addresses: set[int]

    @property
    def start_addr(self) -> int:
        return int(self.name.removeprefix("sub_"), 16)

    @property
    def end_addr_hint(self) -> int:
        if self.addresses:
            return max(self.addresses)
        return self.start_addr


def parse_u32(value: str) -> int:
    return int(value, 0) & 0xFFFFFFFF


def fmt_addr(value: int) -> str:
    return f"0x{value:08X}"


def reg_num(value: str) -> int:
    value = value.strip().lower()
    if not value.startswith("r"):
        raise argparse.ArgumentTypeError(f"register must look like r11, got {value!r}")
    return int(value[1:])


def load_functions() -> list[FunctionBlock]:
    functions: list[FunctionBlock] = []
    for path in sorted(PPC_OUTPUT.glob("ppc_recomp.*.cpp")):
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        starts: list[tuple[int, str]] = []
        for i, line in enumerate(lines):
            m = FUNC_RE.match(line)
            if m:
                starts.append((i, m.group(1)))
        for index, (start, name) in enumerate(starts):
            end = starts[index + 1][0] if index + 1 < len(starts) else len(lines)
            body = lines[start:end]
            addresses = {int(name.removeprefix("sub_"), 16)}
            for line in body:
                loc = LOC_RE.match(line)
                if loc:
                    addresses.add(int(loc.group(1), 16))
            functions.append(FunctionBlock(name, path, start + 1, end, body, addresses))
    return functions


def find_function(functions: list[FunctionBlock], pc: int, target: int) -> FunctionBlock:
    exact = [fn for fn in functions if pc in fn.addresses or target in fn.addresses]
    if exact:
        return min(exact, key=lambda fn: (target not in fn.addresses, pc not in fn.addresses))

    containing = [
        fn
        for fn in functions
        if fn.start_addr <= pc <= fn.end_addr_hint + 0x100
    ]
    if containing:
        return min(containing, key=lambda fn: abs(fn.start_addr - pc))

    raise SystemExit(f"could not locate containing PPC function for PC {fmt_addr(pc)}")


def comment_at(lines: list[str], index: int) -> str | None:
    match = COMMENT_RE.match(lines[index])
    return match.group(1) if match else None


def find_bctr_index(fn: FunctionBlock, pc: int, target: int) -> int:
    candidates = []
    for i, line in enumerate(fn.lines):
        comment = comment_at(fn.lines, i)
        if comment == "bctr":
            candidates.append(i)
        if "switch (ctx.r" in line:
            candidates.append(i)

    if not candidates:
        raise SystemExit(f"{fn.name} has no bctr/switch candidate in {fn.file}")

    target_loc = f"loc_{target:08X}"
    with_target = [
        i for i in candidates
        if any(target_loc in line for line in fn.lines[i : min(len(fn.lines), i + 180)])
    ]
    if with_target:
        return with_target[0]

    # The generated C++ does not retain the PC on the bctr comment itself, so
    # use the only candidate when there is just one.
    if len(candidates) == 1:
        return candidates[0]
    return candidates[0]


def infer_prelude(fn: FunctionBlock, bctr_index: int, selector: int) -> tuple[int | None, int | None, int | None, int | None]:
    entry_count = None
    default = None
    inferred_selector = None
    computed_base = None
    hi_reg = None
    hi_value = None

    for i in range(max(0, bctr_index - 24), bctr_index):
        comment = comment_at(fn.lines, i)
        if not comment:
            continue
        if m := CMPLWI_RE.search(comment):
            inferred_selector = int(m.group(1))
            entry_count = int(m.group(2)) + 1
        elif m := BGT_RE.search(comment):
            default = int(m.group(1), 16)
        elif m := RLWINM_RE.search(comment):
            inferred_selector = int(m.group(1))
        elif m := LIS_RE.search(comment):
            hi_reg = int(m.group(1))
            hi_value = int(m.group(2))
        elif m := ADDI_RE.search(comment):
            reg = int(m.group(1))
            if hi_reg == reg and hi_value is not None:
                computed_base = ((hi_value << 16) + int(m.group(2))) & 0xFFFFFFFF

    if inferred_selector is not None and inferred_selector != selector:
        print(f"warning: requested selector r{selector}, prelude suggests r{inferred_selector}")

    return inferred_selector, computed_base, entry_count, default


def decode_lwz_word(line: str) -> int | None:
    comment = COMMENT_RE.match(line)
    if not comment:
        return None
    m = LWZ_RE.search(comment.group(1))
    if not m:
        return None
    rt, disp, ra = int(m.group(1)), int(m.group(2)), int(m.group(3))
    return ((32 << 26) | (rt << 21) | (ra << 16) | (disp & 0xFFFF)) & 0xFFFFFFFF


def read_switch_entries(lines: list[str], bctr_index: int) -> list[int]:
    entries: list[int] = []
    in_case = False
    for line in lines[bctr_index : min(len(lines), bctr_index + 260)]:
        if CASE_RE.match(line):
            in_case = True
            continue
        if in_case:
            m = GOTO_RE.match(line)
            if m:
                entries.append(int(m.group(1), 16))
                in_case = False
            elif "default:" in line:
                break
    return entries


def read_comment_entries(lines: list[str], bctr_index: int, entry_count: int | None) -> list[int]:
    limit = entry_count or 96
    entries: list[int] = []
    for line in lines[bctr_index + 1 : min(len(lines), bctr_index + 1 + limit * 3)]:
        word = decode_lwz_word(line)
        if word is None:
            continue
        if 0x82000000 <= word <= 0x83FFFFFF:
            entries.append(word)
            if len(entries) >= limit:
                break
    return entries


def classify_targets(
    functions: list[FunctionBlock], fn: FunctionBlock, entries: list[int]
) -> tuple[list[int], list[int], list[int]]:
    known_functions = {f.start_addr for f in functions}
    fn_end = fn.end_addr_hint + 0x100
    internal = sorted({addr for addr in entries if fn.start_addr <= addr <= fn_end})
    external = sorted({addr for addr in entries if addr in known_functions and addr not in internal})
    unknown = sorted({addr for addr in entries if addr not in internal and addr not in known_functions})
    return internal, external, unknown


def render_candidate(pc: int, selector: int, default: int | None, labels: list[int]) -> str:
    out = ["[[switch]]", f"base = {fmt_addr(pc)}", f"r = {selector}"]
    if default is not None:
        out.append(f"default = {fmt_addr(default)}")
    out.append("labels = [")
    for i in range(0, len(labels), 4):
        chunk = ", ".join(fmt_addr(v) for v in labels[i : i + 4])
        out.append(f"    {chunk},")
    out.append("]")
    out.append("")
    return "\n".join(out)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--pc", required=True, type=parse_u32)
    parser.add_argument("--target", required=True, type=parse_u32)
    parser.add_argument("--selector", required=True, type=reg_num)
    parser.add_argument("--table-base", required=True, type=parse_u32)
    args = parser.parse_args()

    functions = load_functions()
    fn = find_function(functions, args.pc, args.target)
    bctr_index = find_bctr_index(fn, args.pc, args.target)
    inferred_selector, computed_base, entry_count, default = infer_prelude(fn, bctr_index, args.selector)

    entries = read_switch_entries(fn.lines, bctr_index)
    source = "generated switch"
    if not entries:
        entries = read_comment_entries(fn.lines, bctr_index, entry_count)
        source = "post-bctr PPC comments"
    if entry_count and len(entries) > entry_count:
        entries = entries[:entry_count]

    if args.target not in entries:
        print(f"warning: observed target {fmt_addr(args.target)} was not found in recovered entries")

    internal, external, unknown = classify_targets(functions, fn, entries)
    candidate = render_candidate(args.pc, args.selector, default, entries)

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    output_path = OUTPUT_DIR / f"switch_{args.pc:08X}.toml"
    output_path.write_text(candidate, encoding="utf-8")

    table_note = fmt_addr(computed_base) if computed_base is not None else "unknown"
    requested_note = fmt_addr(args.table_base)
    print(f"function: {fn.name} ({fn.file.relative_to(ROOT)}:{fn.start_line})")
    print(f"bctr PC: {fmt_addr(args.pc)}")
    print(f"selector: r{args.selector}" + (f" (inferred r{inferred_selector})" if inferred_selector is not None else ""))
    print(f"table base: {requested_note} (computed data base {table_note}; config base {fmt_addr(args.pc)})")
    print(f"entry count: {len(entries)}" + (f" (bound suggests {entry_count})" if entry_count else ""))
    print(f"unique targets: {len(set(entries))}")
    print("internal labels: " + ", ".join(fmt_addr(v) for v in internal))
    print("external functions: " + (", ".join(fmt_addr(v) for v in external) if external else "none"))
    print("unknown targets: " + (", ".join(fmt_addr(v) for v in unknown) if unknown else "none"))
    print(f"entry source: {source}")
    print(f"candidate: {output_path.relative_to(ROOT)}")
    print()
    print(candidate)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())



