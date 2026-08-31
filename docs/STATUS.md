# Estado actual

Fecha local: 2026-08-30

## Hecho

- Confirmada ISO local de UFC Undisputed 3:
  `D:\Xbox360\UFC Undisputed 3 (USA, Europe) (En,Fr,De,Es,It,Pt).iso`
- Localizadas herramientas del intento anterior:
  - Ghidra 12.0
  - Xenia Canary
  - XenonAnalyse
  - XenonRecomp
  - Visual Studio/CMake
- Validado extractor adicional en:
  `D:\Nueva carpeta\isoextract\exiso.exe`
- Clonado y compilado `extract-xiso` en:
  `tools\extract-xiso\build-nmake\extract-xiso.exe`
- Generada lista completa de archivos:
  `ufc_file_list.txt`
- Extraido `default.xex` sin extraer toda la ISO:
  `extracted\default.xex`
- Extraido asset de prueba:
  `extracted\sample_main_menu.ytx`
- Generadas tablas iniciales con `XenonAnalyse`:
  `config\ufc_switch_tables.toml`
- Ejecutado `XenonRecomp` sobre UFC y generado C++:
  `ppc_output`
- Copiada una version de trabajo de `XenonRecomp` dentro del workspace:
  `tools\XenonRecomp-ufc`
- Parcheada y compilada la copia local del recompilador:
  `tools\XenonRecomp-ufc\build-vs\XenonRecomp\XenonRecomp.exe`

## Datos importantes

- `default.xex`:
  - Tamano: 20,856,832 bytes
  - SHA256: `39FB34CB7C42526469E85E2A1068A34EEDDCB82ABCD85E0937FC35B8B363D8E8`
- XDVDFS:
  - Partition base: `0x0FD90000`
  - Root sector: `84816`
- PPC map:
  - Image base: `0x82000000`
  - Image size: `0x1640000`
  - Code base: `0x82220000`
  - Code size: `0x103CDB0`
- Recompiled output:
  - 400 files
  - 310,869,337 bytes generated
- `D:\Nueva carpeta\isoextract\exiso.exe`:
  - Version: `extract-xiso v2.7.0 (09.30.11) for win32`
  - SHA256: `293C152CE3C06368E7C16E75EE43A00551570910A90BEFCC633F06CDDB2E72DC`
  - Confirmado con `-l`: lista correctamente la particion de juego de UFC.

## Register save/restore candidates

These are now configured under `[main]` in `config/ufc_undisputed_3.toml`.

- `savegprlr_14_address = 0x82EFC8D0`
- `restgprlr_14_address = 0x82EFC920`
- `savefpr_14_address = 0x82EFD180`
- `restfpr_14_address = 0x82EFD1CC`
- `savevmx_14_address = 0x82EFD430`
- `savevmx_64_address = 0x82EFD4C4`
- `restvmx_14_address = 0x82EFD6C8`
- `restvmx_64_address = 0x82EFD75C`

## Recompiler patch progress

All source/tooling changes are kept inside this workspace. The original
`D:\Xbox360\IntentoRecompilacion\XenonRecomp` folder was not modified.

Baseline unrecognized instructions from `logs\xenonrecomp_main_keys.log`: 16,215.

After local patches:

- `logs\xenonrecomp_ufc_patched_01.log`: 8,196
- `logs\xenonrecomp_ufc_patched_02.log`: 589
- `logs\xenonrecomp_ufc_patched_03.log`: 43
- `logs\xenonrecomp_ufc_patched_04.log`: 8
- `logs\xenonrecomp_ufc_patched_05.log`: 0

Implemented or mapped in the local copy:

- Branch/update: `bdzf`
- Integer/carry/logical: `addc`, `addme`, `subfze`, `eqv`
- Memory update variants: `lfsu`, `lfsux`, `lfdu`, `ldux`, `lhzu`, `lhzux`,
  `lhau`, `stfsu`, `stfsux`, `stfdu`, `sthu`, `sthux`, `lbzux`, `lwzux`,
  `stbux`, `stdux`
- VMX/VMX128 subset: `vslh`, `vsrah`, `vsubshs`, `vspltish`, `vmaxsh`,
  `vminsh`, `vcmpgtsh`, `vcmpgtsw`, `vcmpequh`, `vaddsbs`, `vaddsws`,
  `vavguh`, `vsel128`, `vnor`, `vnor128`, `vsrh`, `vsrab`, `vrlh`,
  `vsububm`, `vpkswss`, `vpkswss128`, `vpkswus128`, `vpkshss`,
  `vpkshss128`, `vpkuhus128`, `vpkuwum`, `vcfpuxws128`, `vctuxs`, `vsl`,
  `lvehx`, `vadduhs`, `vsubuwm`, `vsubuws`
- Condition/cache/scalar/64-bit: `bdnzt`, `cror`, `crorc`, `dcbst`,
  `frsqrte`, `mulhd`, `mulhdu`

No unrecognized instructions remain in `logs\xenonrecomp_ufc_patched_05.log`.
The generator still reports six `vcmpgtuh.` locations with an RC bit for which it
does not currently emit a condition-register comparison. This is a compatibility
risk to validate once the generated code can run.

## Next practical step

Create a buildable host project around the generated C++, then perform a
syntax-only compile to identify the runtime APIs and generated-code fixes needed
before creating the minimal Windows launcher layer.

## Host CMake build progress

- Added a root CMake project for the generated UFC C++.
- Added `ufc_ppc_generated` as an OBJECT library over `ppc_output\ppc_func_mapping.cpp`
  and all `ppc_output\ppc_recomp.*.cpp` files.
- Added `ufc_native_incremental` as the incremental build target.
- Added `scripts\03_build_incremental.bat` to configure Ninja with Visual Studio LLVM
  `clang++.exe` and build `ufc_native_incremental`.
- Validated configuration and build startup in `build\ufc-native`.

Current first compile blockers:

- `ppc_output\ppc_recomp.128.cpp:5638`: missing label `loc_827ECE14`
- `ppc_output\ppc_recomp.128.cpp:5641`: missing label `loc_827ECE0C`
- `ppc_output\ppc_recomp.13.cpp:17167`: missing label `loc_822BBCB8`

These are generated-code label emission issues, not CMake/runtime include issues.

## Compile sweep progress

Added `scripts\04_compile_sweep.ps1` and `scripts\04_compile_sweep.bat`.
The sweep compiles each generated translation unit independently with Visual Studio
LLVM `clang++.exe`, writes one log per source, and emits:

- `logs\compile_sweep\summary.csv`
- `logs\compile_sweep\summary.txt`

Full syntax-only sweep result from 2026-08-30 13:33:54 to 13:43:18:

- Sources checked: 396
- Passed: 384
- Failed: 12

Failure families:

- Missing generated labels in 10 translation units:
  - `ppc_recomp.13.cpp`
  - `ppc_recomp.31.cpp`
  - `ppc_recomp.44.cpp`
  - `ppc_recomp.128.cpp`
  - `ppc_recomp.155.cpp`
  - `ppc_recomp.157.cpp`
  - `ppc_recomp.158.cpp`
  - `ppc_recomp.159.cpp`
  - `ppc_recomp.204.cpp`
  - `ppc_recomp.318.cpp`
- Invalid generated `mulhdu` cast syntax in 2 translation units:
  - `ppc_recomp.171.cpp`: emits `unsigned __int128(ctx.rN.u64)`
  - `ppc_recomp.277.cpp`: emits `unsigned __int128(ctx.rN.u64)`

Recommended next fixes:

1. Patch XenonRecomp label emission for branches that target intra-function labels
   not currently materialized in the output.
2. Patch `mulhdu` generation to use a Clang-compatible cast, such as
   `(unsigned __int128)(value)`, or a local unsigned-128 alias/helper.
3. Regenerate `ppc_output`, then rerun `scripts\04_compile_sweep.bat` before
   returning to the incremental CMake target.

## Short generator fix: mulhdu cast

Patched `tools\XenonRecomp-ufc\XenonRecomp\recompiler.cpp` so `PPC_INST_MULHDU`
emits Clang-compatible unsigned 128-bit casts:

- Previous generated form: `unsigned __int128(value)`
- New generated form: `((unsigned __int128)(value))`

Rebuilt the local XenonRecomp binary with `scripts\build_xenonrecomp_ufc.bat`,
regenerated `ppc_output`, and validated the two previously affected translation
units:

- `scripts\04_compile_sweep.bat -StartIndex 171 -StopIndex 171`: passed
- `scripts\04_compile_sweep.bat -StartIndex 277 -StopIndex 277`: passed

Expected syntax sweep status after this fix: 386 passing translation units and
10 remaining failures, all in the missing-label family. Run the full sweep again
to confirm the aggregate count after the label-emission fix.

## Short generator fix: cross-function conditional CTR branches

Patched `tools\XenonRecomp-ufc\XenonRecomp\recompiler.cpp` so conditional
branches, including `bdz`/`bdnz` variants, emit a function call plus `return`
when the branch target is outside the current recompiled function scope. This
prevents invalid C++ `goto loc_*` references to labels that exist inside another
host function.

Implementation notes:

- Added `printConditionalBranchTo(target, condition)`.
- Reused the existing `printFunctionCall(target)` path for out-of-function
  branch targets.
- Updated `bdz`, `bdnz`, `bdnzf`, `bdnzt`, and `bdzf` emission to use the helper.
- Kept in-function branch emission as local `goto loc_*`.

Rebuilt XenonRecomp, regenerated `ppc_output`, and validated representative plus
previously failing translation units:

- `ppc_recomp.13.cpp`: passed
- `ppc_recomp.31.cpp`: passed
- `ppc_recomp.44.cpp`: passed
- `ppc_recomp.128.cpp`: passed
- `ppc_recomp.155.cpp`: passed
- `ppc_recomp.157.cpp`: passed
- `ppc_recomp.158.cpp`: passed
- `ppc_recomp.159.cpp`: passed
- `ppc_recomp.204.cpp`: passed
- `ppc_recomp.318.cpp`: passed

Expected syntax sweep status after the `mulhdu` and branch-target fixes: all 396
generated translation units should pass syntax. Run `scripts\04_compile_sweep.bat`
to confirm the full aggregate result.

## Short generator fix: vcmpgtuh. CR6 result

Patched `PPC_INST_VCMPGTUH` emission so the record form (`vcmpgtuh.`) updates
CR6 from the generated 16-bit comparison mask, matching the existing VMX
comparison handlers.

Rebuilt XenonRecomp and regenerated `ppc_output` without the previous RC-bit
warnings. The four translation units containing `cmpgt_epu16` all pass the
syntax sweep: `50`, `340`, `348`, and `349`.
## Short generator fix: vcmpgtub. CR6 result

Patched `PPC_INST_VCMPGTUB` emission so the record form (`vcmpgtub.`) updates
CR6 from the 16-byte comparison mask, matching `VCMPEQUB` and the corrected
`VCMPGTUH` handler.

UFC currently emits only the non-record form in `ppc_recomp.50.cpp` and
`ppc_recomp.327.cpp`, so regenerated output is unchanged for this build. Both
translation units pass the syntax sweep after rebuilding XenonRecomp.
## Short generator fix: vcmpgtsh. full-lane CR6 mask

Patched `PPC_INST_VCMPGTSH` record-form emission to reduce all eight signed
16-bit comparison lanes into CR6. The previous float movemask inspected only
four sign bits and could miss false results in alternating halfword lanes.

The corrected output uses the 16-byte integer mask with `0xFFFF`. UFC contains
multiple `vcmpgtsh.` instructions in `ppc_recomp.50.cpp`; the regenerated file
shows the corrected CR6 update and passes the syntax sweep.
## Short generator fix: vcmpbfp128 implementation

Implemented `PPC_INST_VCMPBFP` and `PPC_INST_VCMPBFP128` according to the
AltiVec compare-bounds semantics. Each float lane now emits bit 31 when the
value exceeds the upper bound and bit 30 when it falls below the negated lower
bound; unordered comparisons set both bits. The record form also updates only
CR6[EQ] to report whether every lane is within bounds.

This removes four generated `__builtin_debugtrap()` calls from
`ppc_recomp.252.cpp`. XenonRecomp rebuilt successfully, output regenerated, and
the affected translation unit passes the syntax sweep. Two generated traps
remain, both in `ppc_recomp.37.cpp` (`vpkd3d128` and `vupkd3d128`).
## Short generator fix: vupkd3d128 UINT_2101010

Implemented format `12` (`UINT_2101010`) for `PPC_INST_VUPKD3D128`. The
handler preserves the packed source word, extracts signed 10-bit X/Y/Z and
unsigned 2-bit W fields, applies the VMX128 3.0-plus-bit representation, clamps
the -512 XYZ edge to -511, and writes lanes using the generator's reversed
vector layout.

This removes the `vupkd3d128 v63,v63,12` debug trap from
`ppc_recomp.37.cpp`. XenonRecomp rebuilt successfully, generated output shows
the unpack sequence, and the affected translation unit passes syntax. One
generated trap remains: `vpkd3d128` in the same file.
## Short generator fix: vpkd3d128 UINT_2101010

Implemented format `3` (`UINT_2101010`) for `PPC_INST_VPKD3D128`, completing
the inverse of the existing unpack path. The handler handles signed 10-bit
X/Y/Z saturation, unsigned 2-bit W saturation, NaN encodings, reversed vector
lanes, and the selected destination word.

This removes the final generated `__builtin_debugtrap()` from
`ppc_recomp.37.cpp`. XenonRecomp rebuilt successfully, generated output contains
no debug traps, and the affected translation unit passes syntax. A complete
396-file sweep is still required to confirm the aggregate result.
## Short generator fix: vpkd3d128 D3D color packing

Corrected the D3D color form of `PPC_INST_VPKD3D128` to pack the low byte of
each source lane directly. The previous emitter treated the source as floating
point and saturated it before extraction, which broke the natural round trip
with the corresponding unpack instruction.

UFC uses this format in one generated translation unit. XenonRecomp rebuilt,
the regenerated sequence now performs byte extraction and reordering only, and
the affected unit passes the syntax sweep.
## Short generator fix: vupkd3d128 SHORT2 minimum clamp

Corrected the SHORT2 form of `PPC_INST_VUPKD3D128` so an input component of
`INT16_MIN` is clamped to `-INT16_MAX` before constructing the VMX128 unpacked
representation. This preserves the symmetric signed range and avoids emitting
the invalid -32768 edge representation.

The format is used broadly across UFC's generated code. XenonRecomp rebuilt,
the regenerated sources contain the clamp, and representative translation
units from the beginning, middle, and end of the corpus pass syntax.
## Short generator fix: vpkd3d128 FLOAT16_4 source aliasing

Corrected the FLOAT16_4 form of `PPC_INST_VPKD3D128` to copy the complete
source vector before writing packed half-floats. Several UFC instructions use
the same vector as source and destination; the previous generated sequence
could overwrite later source components before reading them.

XenonRecomp rebuilt and regenerated output now reads conversion data from the
preserved copy. Representative aliasing and non-aliasing translation units both
pass syntax.
## Full compile validation

The complete syntax sweep now passes all 396 generated translation units with
zero failures. The incremental CMake target also builds the complete generated
object set successfully with Clang. The next build milestone is a native x64
executable target and entry point.