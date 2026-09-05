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
STARTUP_NOTIFY_LOOP_RE = re.compile(
    r"\[startup-notify-loop\]\s+tid=([0-9A-F]+)\s+(before|after)\s+"
    r"(sub_[0-9A-F]+)\s+count=(\d+)\s+block=([0-9A-F]+)\s+"
    r"h0=([0-9A-F]+)\s+h1=([0-9A-F]+)\s+h2=([0-9A-F]+)\s+stop12=([0-9A-F]+)"
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
    r"(?:callerLR=([0-9A-F]+)\s+r30=([0-9A-F]+)\s+r31=([0-9A-F]+)\s+)?"
    r"h0=([0-9A-F]+)\s+h1=([0-9A-F]+)\s+h2=([0-9A-F]+)\s+h3=([0-9A-F]+)"
)
STARTUP_WAITMULTI_RETURN_RE = re.compile(
    r"\[startup-waitmulti-return\]\s+tid=([0-9A-F]+)\s+result=([0-9A-F]+)"
)
STARTUP_WAITMULTI_OWNER_RE = re.compile(
    r"\[startup-waitmulti-owner\]\s+tid=([0-9A-F]+)\s+result=([0-9A-F]+)\s+"
    r"owner=([0-9A-F]+)\s+vtbl=([0-9A-F]+)\s+field4=([0-9A-F]+)\s+"
    r"field40=([0-9A-F]+)\s+field44=([0-9A-F]+)\s+field48=([0-9A-F]+)\s+"
    r"field52=([0-9A-F]+)\s+field56=([0-9A-F]+)\s+field68=([0-9A-F]+)(?:\s+handles=([0-9A-F]+))?"
)
STARTUP_QUEUE_COUNTER_RE = re.compile(
    r"\[startup-queue-counter\]\s+tid=([0-9A-F]+)\s+(before|after)\s+"
    r"(sub_[0-9A-F]+)\s+count=(\d+)\s+owner=([0-9A-F]+)\s+vtbl=([0-9A-F]+)\s+"
    r"field4=([0-9A-F]+)\s+field48=([0-9A-F]+)\s+field56=([0-9A-F]+)\s+"
    r"field68=([0-9A-F]+)"
)
STARTUP_FRAME_POP_RE = re.compile(
    r"\[startup-frame-pop\]\s+tid=([0-9A-F]+)\s+(after)\s+count=(\d+)\s+lr=([0-9A-F]+)\s+"
    r"owner=([0-9A-F]+)\s+queue=([0-9A-F]+)\s+item=([0-9A-F]+)\s+"
    r"item0=([0-9A-F]+)\s+type2=([0-9A-F]+)\s+item4=([0-9A-F]+)\s+item8=([0-9A-F]+)\s+"
    r"itemC=([0-9A-F]+)\s+item10=([0-9A-F]+)\s+producer140=([0-9A-F]+)\s+consumer144=([0-9A-F]+)"
)
STARTUP_FRAME_DISPATCH_RE = re.compile(
    r"\[startup-frame-dispatch\]\s+tid=([0-9A-F]+)\s+(before|after)\s+"
    r"(sub_[0-9A-F]+)\s+count=(\d+)\s+lr=([0-9A-F]+)\s+"
    r"r3=([0-9A-F]+)\s+r4=([0-9A-F]+)\s+r5=([0-9A-F]+)\s+r6=([0-9A-F]+)\s+"
    r"r7=([0-9A-F]+)\s+r8=([0-9A-F]+)\s+ret=([0-9A-F]+)"
)
STARTUP_FRAME_QUEUE_RE = re.compile(
    r"\[startup-frame-queue\]\s+tid=([0-9A-F]+)\s+(before|after)\s+"
    r"(sub_[0-9A-F]+)\s+count=(\d+)\s+lr=([0-9A-F]+)\s+obj=([0-9A-F]+)\s+"
    r"flag40=([0-9A-F]+)\s+flag64=([0-9A-F]+)\s+frame136=([0-9A-F]+)\s+"
    r"(?:read140|producer140)=([0-9A-F]+)\s+(?:write144|consumer144)=([0-9A-F]+)\s+event164=([0-9A-F]+)\s+"
    r"item=([0-9A-F]+)\s+itemType=([0-9A-F]+)\s+ret=([0-9A-F]+)"
)
STARTUP_SCHEDULER_SOURCE_RE = re.compile(
    r"\[startup-scheduler-source\]\s+tid=([0-9A-F]+)\s+(before|after)\s+"
    r"(sub_[0-9A-F]+)\s+count=(\d+)\s+lr=([0-9A-F]+)\s+owner=([0-9A-F]+)\s+"
    r"vtbl=([0-9A-F]+)\s+flag64=([0-9A-F]+)\s+list80=([0-9A-F]+)\s+"
    r"field136=([0-9A-F]+)\s+field140=([0-9A-F]+)\s+field144=([0-9A-F]+)\s+"
    r"field164=([0-9A-F]+)\s+ret=([0-9A-F]+)\s+arg4=([0-9A-F]+)"
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
    waitmulti_owner_states: Counter[str] = Counter()
    queue_counter_states: Counter[str] = Counter()
    scheduler_source_states: Counter[str] = Counter()
    frame_queue_states: Counter[str] = Counter()
    frame_dispatch_states: Counter[str] = Counter()
    frame_pop_states: Counter[str] = Counter()
    notify_loop_states: Counter[str] = Counter()
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
            tid, count, handles, wait_all, timeout, alertable, caller_lr, r30, r31, h0, h1, h2, h3 = match.groups()
            key = f"{tid}|count={count}|handles={handles}|waitAll={wait_all}|timeout={timeout}|alertable={alertable}|callerLR={caller_lr or 'unknown'}|r30={r30 or 'unknown'}|r31={r31 or 'unknown'}|{h0},{h1},{h2},{h3}"
            waitmulti_calls[key] += 1
        if match := STARTUP_WAITMULTI_RETURN_RE.search(line):
            tid, result = match.groups()
            waitmulti_returns[f"{tid}|result={result}"] += 1
        if match := STARTUP_WAITMULTI_OWNER_RE.search(line):
            tid, result, owner, vtbl, f4, f40, f44, f48, f52, f56, f68, handles = match.groups()
            key = (
                f"{tid}|result={result}|owner={owner}|vtbl={vtbl}|field4={f4}|"
                f"field40={f40}|field44={f44}|field48={f48}|field52={f52}|"
                f"field56={f56}|field68={f68}|handles={handles or 'unknown'}"
            )
            waitmulti_owner_states[key] += 1
        if match := STARTUP_QUEUE_COUNTER_RE.search(line):
            tid, phase, function, count, owner, vtbl, f4, f48, f56, f68 = match.groups()
            key = (
                f"{tid}|{phase}|{function}|owner={owner}|vtbl={vtbl}|"
                f"field4={f4}|field48={f48}|field56={f56}|field68={f68}|seen={count}"
            )
            queue_counter_states[key] += 1
        if match := STARTUP_FRAME_POP_RE.search(line):
            tid, phase, count, lr, owner, queue, item, item0, type2, item4, item8, itemc, item10, producer140, consumer144 = match.groups()
            frame_pop_states[f"{tid}|type={type2}|owner={owner}|lr={lr}|item={item}|item0={item0}|item4={item4}|item8={item8}|itemC={itemc}|item10={item10}|producer140={producer140}|consumer144={consumer144}|seen={count}"] += 1
        if match := STARTUP_FRAME_DISPATCH_RE.search(line):
            tid, phase, function, count, lr, r3, r4, r5, r6, r7, r8, ret = match.groups()
            key = (
                f"{tid}|{phase}|{function}|lr={lr}|r3={r3}|r4={r4}|r5={r5}|"
                f"r6={r6}|r7={r7}|r8={r8}|ret={ret}|seen={count}"
            )
            frame_dispatch_states[key] += 1
        if match := STARTUP_FRAME_QUEUE_RE.search(line):
            tid, phase, function, count, lr, obj, flag40, flag64, frame136, producer140, consumer144, event164, item, item_type, ret = match.groups()
            key = (
                f"{tid}|{phase}|{function}|lr={lr}|obj={obj}|flag40={flag40}|flag64={flag64}|"
                f"frame136={frame136}|producer140={producer140}|consumer144={consumer144}|event164={event164}|"
                f"item={item}|itemType={item_type}|ret={ret}|seen={count}"
            )
            frame_queue_states[key] += 1
        if match := STARTUP_SCHEDULER_SOURCE_RE.search(line):
            tid, phase, function, count, lr, owner, vtbl, flag64, list80, f136, f140, f144, f164, ret, arg4 = match.groups()
            key = (
                f"{tid}|{phase}|{function}|lr={lr}|owner={owner}|vtbl={vtbl}|flag64={flag64}|"
                f"list80={list80}|field136={f136}|field140={f140}|field144={f144}|"
                f"field164={f164}|ret={ret}|arg4={arg4}|seen={count}"
            )
            scheduler_source_states[key] += 1
        if match := STARTUP_NOTIFY_LOOP_RE.search(line):
            tid, phase, function, count, block, h0, h1, h2, stop12 = match.groups()
            notify_loop_states[f"{tid}|{phase}|{function}|block={block}|h0={h0}|h1={h1}|h2={h2}|stop12={stop12}|seen={count}"] += 1
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
    print_counter("startup waitmulti owner states", waitmulti_owner_states, args.limit)
    print_counter("startup queue-counter states", queue_counter_states, args.limit)
    print_counter("startup scheduler-source states", scheduler_source_states, args.limit)
    print_counter("startup frame-queue states", frame_queue_states, args.limit)
    print_counter("startup frame-pop states", frame_pop_states, args.limit)
    print_counter("startup frame-dispatch states", frame_dispatch_states, args.limit)
    print_counter("startup notify-loop states", notify_loop_states, args.limit)
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