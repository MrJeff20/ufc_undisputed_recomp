# UFC Undisputed 3 Xbox 360 Recomp

Workspace para investigar una recompilacion/port experimental de una copia propia de
`UFC Undisputed 3` de Xbox 360.

## Estado inicial

- ISO localizada: `D:\Xbox360\UFC Undisputed 3 (USA, Europe) (En,Fr,De,Es,It,Pt).iso`
- Intento anterior localizado: `D:\Xbox360\IntentoRecompilacion`
- Herramientas locales utiles:
  - Ghidra 12.0: `D:\Xbox360\IntentoRecompilacion\ghidra_12.0_PUBLIC`
  - Xenia Canary: `D:\Xbox360\IntentoRecompilacion\xenia_pruebas\xenia_canary.exe`
  - XenonAnalyse: `D:\Xbox360\IntentoRecompilacion\XenonRecomp\build\XenonAnalyse\XenonAnalyse.exe`
  - XenonRecomp: `D:\Xbox360\IntentoRecompilacion\XenonRecomp\build\XenonRecomp\XenonRecomp.exe`
  - exiso/extract-xiso: `D:\Nueva carpeta\isoextract\exiso.exe`
  - CMake: `C:\Program Files\CMake\bin\cmake.exe`
  - Python: `C:\Python314\python.exe`
  - Java: `C:\Program Files\Semeru\jdk-26.0.2.0-openj9\bin\java.exe`
- Copia de trabajo modificable dentro del workspace:
  - `tools\XenonRecomp-ufc`
  - binario compilado: `tools\XenonRecomp-ufc\build-vs\XenonRecomp\XenonRecomp.exe`

## Primer objetivo

1. Extraer legalmente la ISO propia a una carpeta de trabajo.
2. Confirmar que existe `default.xex`.
3. Ejecutar `XenonAnalyse` para generar tablas de salto.
4. Preparar el TOML minimo para `XenonRecomp`.
5. Importar el XEX/base image en Ghidra para revisar entrada, imports y patrones de runtime.

## Progreso actual

Ver `docs/STATUS.md`.

Resumen actual: la copia local de `XenonRecomp` ya redujo las instrucciones
PowerPC/VMX no reconocidas de 16,215 a 0 en la pasada completa de UFC. El
siguiente hito es hacer compilable y enlazable el C++ generado con un runtime
minimo de Windows.

## Build nativo experimental

El repo ahora incluye un `CMakeLists.txt` raiz para compilar incrementalmente el
C++ generado por `XenonRecomp`.

Comandos principales:

- `scripts\02_syntax_check.bat`: valida una unidad generada de prueba.
- `scripts\03_build_incremental.bat`: configura Ninja/Clang y compila el target
  CMake `ufc_native_incremental`.
- `scripts\04_compile_sweep.bat`: compila cada `ppc_output\ppc_recomp.N.cpp`
  por separado y genera reportes en `logs\compile_sweep`.

Resultado del ultimo sweep:

- 396 unidades revisadas.
- 384 pasan sintaxis.
- 12 fallan.

Bloqueos actuales:

- Labels generados faltantes en 10 unidades recompiladas.
- Cast invalido de `mulhdu` en 2 unidades, emitido como
  `unsigned __int128(ctx.rN.u64)`.

Los siguientes pasos tecnicos son parchear la emision de labels en
`tools\XenonRecomp-ufc`, corregir el cast de `mulhdu`, regenerar `ppc_output` y
volver a ejecutar `scripts\04_compile_sweep.bat`.

## Nota legal y practica

Este workspace no incluye herramientas, claves, cracks ni archivos distribuidos del juego.
Los scripts asumen una copia local propia y solo crean inventario/configuracion para analisis.