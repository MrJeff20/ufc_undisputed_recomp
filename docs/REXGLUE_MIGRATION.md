# UFC3 ReXGlue migration

## Minimum layout

- `ppc_output/`: generated XenonRecomp sources; do not edit by hand.
- `src/rexglue/`: UFC-owned image metadata and future app hooks.
- `thirdparty/rexglue-sdk/`: runtime SDK submodule.
- `src/native/`: temporary prototype retained as behavioral reference only.

The main executable should ultimately be a thin `rex::ReXApp` host. ReXGlue
owns guest memory, function dispatch, VFS, kernel state, imports, threading and
XEX loading. UFC owns only its generated code, image constants, game-specific
hooks and data.

## Integration contract

`UFC3PPCImageConfig` describes the generated code and image ranges and exposes
`PPCFuncMappings` to `rex::Runtime::Setup`. ReXGlue then loads `default.xex`
through its VFS and launches the XEX entry point (`0x823C7E68`) through its
kernel thread path.

## Current incompatibility

The existing output was generated with XenonRecomp's legacy `ppc_context.h`.
Its `PPCFunc` and `PPCFuncMapping` shapes match ReXGlue, but `PPCContext` does
not: ReXGlue adds runtime-owned context fields and uses different dispatch,
MMIO, timing and synchronization macros. Mixing those definitions would violate
the C++ one-definition rule and shift register offsets.

`ufc3_rex_game` is therefore intentionally compile-only scaffolding. It proves
that the current generated object corpus and image metadata can be packaged as
a ReXGlue-linked game archive, but it must not be executed until the generated
sources use ReXGlue's PPC headers and macros.