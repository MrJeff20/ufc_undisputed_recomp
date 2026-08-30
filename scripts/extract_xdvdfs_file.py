#!/usr/bin/env python3
import argparse
import os
import struct
from dataclasses import dataclass

SECTOR_SIZE = 2048
HEADER_MAGIC = b"MICROSOFT*XBOX*MEDIA"
HEADER_OFFSET = 0x10000
GLOBAL_LSEEK_OFFSET = 0x0FD90000
XGD3_LSEEK_OFFSET = 0x02080000
XGD1_LSEEK_OFFSET = 0x18300000
PARTITION_OFFSETS = (0, GLOBAL_LSEEK_OFFSET, XGD3_LSEEK_OFFSET, XGD1_LSEEK_OFFSET)
ATTR_DIR = 0x10
PAD_SHORT = 0xFFFF


@dataclass
class Entry:
    path: str
    start_sector: int
    size: int
    attributes: int


def read_at(f, offset, size):
    f.seek(offset)
    data = f.read(size)
    if len(data) != size:
        raise EOFError(f"short read at 0x{offset:x}")
    return data


def detect_partition(f):
    for base in PARTITION_OFFSETS:
        header = read_at(f, base + HEADER_OFFSET, len(HEADER_MAGIC))
        if header == HEADER_MAGIC:
            root_sector, root_size = struct.unpack(
                "<II", read_at(f, base + HEADER_OFFSET + len(HEADER_MAGIC), 8)
            )
            return base, root_sector, root_size
    raise RuntimeError("XDVDFS header not found")


def read_entry(f, dir_start, entry_offset):
    offset = dir_start + entry_offset * 4
    data = read_at(f, offset, 14)
    left, right, start_sector, size, attributes, name_len = struct.unpack("<HHIIBB", data)
    name = read_at(f, offset + 14, name_len).decode("ascii", errors="replace")
    return left, right, Entry(name, start_sector, size, attributes)


def walk_tree(f, base, dir_start, prefix, entry_offset, out, allow_zero=False):
    if entry_offset == PAD_SHORT or (entry_offset == 0 and not allow_zero):
        return

    left, right, entry = read_entry(f, dir_start, entry_offset)

    if left:
        walk_tree(f, base, dir_start, prefix, left, out)

    full_path = f"{prefix}/{entry.path}" if prefix else entry.path
    normalized = full_path.replace("\\", "/")
    out[normalized.lower()] = Entry(normalized, entry.start_sector, entry.size, entry.attributes)

    if entry.attributes & ATTR_DIR and entry.size > 0:
        child_start = base + entry.start_sector * SECTOR_SIZE
        walk_tree(f, base, child_start, normalized, 0, out, allow_zero=True)

    if right:
        walk_tree(f, base, dir_start, prefix, right, out)


def build_index(iso_path):
    with open(iso_path, "rb") as f:
        base, root_sector, root_size = detect_partition(f)
        root_start = base + root_sector * SECTOR_SIZE
        entries = {}
        walk_tree(f, base, root_start, "", 0, entries, allow_zero=True)
        return base, root_sector, root_size, entries


def extract_file(iso_path, target_path, output_path):
    base, root_sector, root_size, entries = build_index(iso_path)
    key = target_path.strip("/\\").replace("\\", "/").lower()
    if key not in entries:
        raise RuntimeError(f"file not found: {target_path}")

    entry = entries[key]
    if entry.attributes & ATTR_DIR:
        raise RuntimeError(f"target is a directory: {target_path}")

    os.makedirs(os.path.dirname(os.path.abspath(output_path)), exist_ok=True)
    with open(iso_path, "rb") as src, open(output_path, "wb") as dst:
        src.seek(base + entry.start_sector * SECTOR_SIZE)
        remaining = entry.size
        while remaining:
            chunk = src.read(min(1024 * 1024, remaining))
            if not chunk:
                raise EOFError("unexpected EOF while extracting")
            dst.write(chunk)
            remaining -= len(chunk)

    return base, root_sector, root_size, entry


def main():
    parser = argparse.ArgumentParser(description="Extract one file from an Xbox/Xbox 360 XDVDFS ISO")
    parser.add_argument("iso")
    parser.add_argument("path")
    parser.add_argument("output")
    args = parser.parse_args()

    base, root_sector, root_size, entry = extract_file(args.iso, args.path, args.output)
    print(f"partition_base=0x{base:x}")
    print(f"root_sector={root_sector}")
    print(f"root_size={root_size}")
    print(f"file={entry.path}")
    print(f"start_sector={entry.start_sector}")
    print(f"size={entry.size}")
    print(f"output={args.output}")


if __name__ == "__main__":
    main()
