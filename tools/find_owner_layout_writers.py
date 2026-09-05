#!/usr/bin/env python3
"""List PPC functions that write UFC startup owner-layout offsets.

The recompiler keeps the original PPC instruction as a comment immediately before
most generated stores. This tool groups those comments by containing function so
we can pick targeted instrumentation without scanning giant probe logs by hand.
"""

from __future__ import annotations

import argparse
import re
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

DEFAULT_OFFSETS = (4, 40, 64, 68, 136, 140, 144, 164)
FUNC_RE = re.compile(r"PPC_FUNC_IMPL\(__imp__(sub_[0-9A-F]+)\)")
STORE_COMMENT_RE = re.compile(
    r"//\s+(st[bhwdu]+)\s+([^,]+),\s*(-?\d+)\((r\d+)\)", re.IGNORECASE
)


@dataclass(frozen=True)
class StoreHit:
    file: Path
    line: int
    function: str
    op: str
    src: str
    offset: int
    base: str
    text: str


def parse_offsets(value: str | None) -> set[int]:
    if not value:
        return set(DEFAULT_OFFSETS)
    offsets: set[int] = set()
    for item in value.split(","):
        item = item.strip()
        if not item:
            continue
        offsets.add(int(item, 0))
    return offsets


def iter_hits(root: Path, offsets: set[int]) -> list[StoreHit]:
    hits: list[StoreHit] = []
    for file in sorted(root.glob("ppc_recomp.*.cpp")):
        function = "<unknown>"
        for line_no, line in enumerate(file.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
            if match := FUNC_RE.search(line):
                function = match.group(1)
                continue
            match = STORE_COMMENT_RE.search(line)
            if not match:
                continue
            op, src, raw_offset, base = match.groups()
            offset = int(raw_offset, 10)
            if offset in offsets:
                hits.append(StoreHit(file, line_no, function, op, src, offset, base, line.strip()))
    return hits


def classify(function_hits: list[StoreHit]) -> str:
    offsets = {hit.offset for hit in function_hits}
    if 68 in offsets and 4 in offsets:
        return "queue counter / producer-consumer"
    if 68 in offsets:
        return "queue write/read counter"
    if 4 in offsets:
        return "queue limit/init or cleanup"
    if {40, 64} & offsets:
        return "flags/events"
    return "state/init"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path("ppc_output"), help="Directory containing ppc_recomp.*.cpp")
    parser.add_argument("--offsets", help="Comma-separated offsets, decimal or hex; defaults to UFC owner offsets")
    parser.add_argument("--limit", type=int, default=200, help="Maximum functions to print")
    args = parser.parse_args()

    hits = iter_hits(args.root, parse_offsets(args.offsets))
    grouped: dict[str, list[StoreHit]] = defaultdict(list)
    for hit in hits:
        grouped[hit.function].append(hit)

    ranked = sorted(
        grouped.items(),
        key=lambda item: (68 not in {hit.offset for hit in item[1]}, 4 not in {hit.offset for hit in item[1]}, item[0]),
    )

    print(f"root: {args.root}")
    print(f"functions: {len(grouped)}")
    print(f"stores: {len(hits)}")
    for function, function_hits in ranked[: args.limit]:
        offsets = ",".join(str(offset) for offset in sorted({hit.offset for hit in function_hits}))
        first = function_hits[0]
        print(f"\n{function} [{classify(function_hits)}] offsets={offsets} first={first.file}:{first.line}")
        for hit in function_hits:
            print(f"  {hit.file}:{hit.line}: {hit.text}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())