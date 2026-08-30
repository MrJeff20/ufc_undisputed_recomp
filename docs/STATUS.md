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
