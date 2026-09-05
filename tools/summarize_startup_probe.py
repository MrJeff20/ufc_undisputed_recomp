#!/usr/bin/env python3
"""Summarize UFC 3 startup probe logs.

This is intentionally log-format driven: it groups the thread-aware ReXGlue
diagnostics produced during startup probes without changing runtime behavior.
"""

from __future__ import annotations

import argparse
import re
from collections import Counter
from pathlib import Path


WAIT_RE = re.compile(
    r"\[rex-wait\]\s+tid=([0-9A-F]+)\s+handle=([0-9A-F]+)\s+type=(\d+)\s+"
    r"timeout=(-?\d+)\s+result=([0-9A-F]+)\s+lr=([0-9A-F]+)\s+after Wait"
)
WAIT_MULTIPLE_RE = re.compile(
    r"\[rex-wait-multiple\]\s+tid=([0-9A-F]+).*?result=([0-9A-F]+)\s+"
    r"lr=([0-9A-F]+).*?handles=([^\r\n]+)"
)
EVENT_RE = re.compile(
    r"\[rex-event\]\s+tid=([0-9A-F]+)\s+action=(\S+)\s+handle=([0-9A-F]+).*?"
    r"lr=([0-9A-F]+)\s+count=(\d+)"
)
STARTUP_RE = re.compile(
    r"\[startup\]\s+tid=([0-9A-F]+)\s+enter\s+(sub_[0-9A-F]+)(?:\s+count=(\d+))?"
)
STARTUP_STATE_RE = re.compile(
    r"\[startup-state\]\s+tid=([0-9A-F]+)\s+(sub_[0-9A-F]+)\s+count=(\d+)\s+obj=([0-9A-F]+)\s+(.*)"
)
STARTUP_WAITMULTI_CALL_RE = re.compile(
    r"\[startup-waitmulti-call\]\s+tid=([0-9A-F]+)\s+count=(\d+)\s+handles=([0-9A-F]+)\s+"
    r"waitAll=(\d+)\s+timeout=([0-9A-F]+)\s+alertable=(\d+)\s+"
    r"h0=([0-9A-F]+)\s+h1=([0-9A-F]+)\s+h2=([0-9A-F]+)\s+h3=([0-9A-F]+)"
)
STARTUP_WAITMULTI_RETURN_RE = re.compile(
    r"\[startup-waitmulti-return\]\s+tid=([0-9A-F]+)\s+result=([0-9A-F]+)"
)


def print_counter(title: str, counter: Counter[str], limit: int) -> None:
    print(f"\n{title}:")
    if not counter:
        print("  none")
        return
    for key, count in counter.most_common(limit):
        print(f"  {count:6d} {key}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("log", type=Path, help="Probe .err.log to summarize")
    parser.add_argument("--limit", type=int, default=20, help="Rows per section")
    args = parser.parse_args()

    wait_groups: Counter[str] = Counter()
    wait_multiple_groups: Counter[str] = Counter()
    waitmulti_calls: Counter[str] = Counter()
    waitmulti_returns: Counter[str] = Counter()
    event_latest: dict[str, int] = {}
    startup_latest: dict[str, int] = {}
    startup_state_latest: dict[str, tuple[int, str]] = {}

    for line in args.log.read_text(encoding="utf-8", errors="replace").splitlines():
        if match := WAIT_RE.search(line):
            tid, handle, object_type, timeout, result, lr = match.groups()
            wait_groups[f"{tid}|{handle}|{object_type}|{lr}|{timeout}|{result}"] += 1
        if match := WAIT_MULTIPLE_RE.search(line):
            tid, result, lr, handles = match.groups()
            wait_multiple_groups[f"{tid}|{lr}|{result}|{handles}"] += 1
        if match := STARTUP_WAITMULTI_CALL_RE.search(line):
            tid, count, handles, wait_all, timeout, alertable, h0, h1, h2, h3 = match.groups()
            key = f"{tid}|count={count}|handles={handles}|waitAll={wait_all}|timeout={timeout}|alertable={alertable}|{h0},{h1},{h2},{h3}"
            waitmulti_calls[key] += 1
        if match := STARTUP_WAITMULTI_RETURN_RE.search(line):
            tid, result = match.groups()
            waitmulti_returns[f"{tid}|result={result}"] += 1
        if match := EVENT_RE.search(line):
            tid, action, handle, lr, count = match.groups()
            event_latest[f"{tid}|{action}|{handle}|{lr}"] = max(
                int(count), event_latest.get(f"{tid}|{action}|{handle}|{lr}", 0)
            )
        if match := STARTUP_RE.search(line):
            tid, function, count = match.groups()
            key = f"{tid}|{function}"
            startup_latest[key] = max(int(count or "1"), startup_latest.get(key, 0))
        if match := STARTUP_STATE_RE.search(line):
            tid, function, count, obj, state = match.groups()
            key = f"{tid}|{function}|{obj}"
            startup_state_latest[key] = (int(count), state)

    print(f"log: {args.log}")
    print_counter("wait groups: tid|handle|type|lr|timeout|result", wait_groups, args.limit)
    print_counter("wait-multiple groups: tid|lr|result|handles", wait_multiple_groups, args.limit)
    print_counter("startup waitmulti calls: tid|count|handles|waitAll|timeout|alertable|h0,h1,h2,h3", waitmulti_calls, args.limit)
    print_counter("startup waitmulti returns: tid|result", waitmulti_returns, args.limit)
    print_counter("event latest counts: tid|action|handle|lr", Counter(event_latest), args.limit)
    print_counter("startup enter latest counts: tid|function", Counter(startup_latest), args.limit)
    print("\nstartup-state latest: tid|function|obj:")
    if not startup_state_latest:
        print("  none")
    else:
        ordered = sorted(startup_state_latest.items(), key=lambda item: item[1][0], reverse=True)
        for key, (count, state) in ordered[: args.limit]:
            print(f"  {count:6d} {key} {state}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())