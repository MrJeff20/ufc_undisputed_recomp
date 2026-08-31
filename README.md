# UFC Undisputed 3 Xbox 360 Recomp

Workspace para investigar una recompilacion/port experimental de una copia propia de
`UFC Undisputed 3` de Xbox 360.

## Hallazgos

- Se encontro una copia propia del juego y una extraccion util para el analisis.
- Se encontro un intento anterior de recompilacion con material reutilizable.
- Estan disponibles las herramientas necesarias para analizar el XEX, generar
  C++ y construir codigo nativo con CMake y Clang.
- Se preparo una copia modificable de XenonRecomp adaptada al proyecto.
- El XEX fue analizado y el codigo PowerPC/VMX utilizado por el juego fue
  convertido a fuentes C++.

## Progreso actual

Ver `docs/STATUS.md`.

Resumen actual: la copia local de `XenonRecomp` ya redujo las instrucciones
PowerPC/VMX no reconocidas de 16,215 a 0 en la pasada completa de UFC. El
siguiente hito es terminar de compilar y enlazar el C++ generado dentro de un
ejecutable x64 con adaptaciones nativas para Windows.

## Build nativo experimental

El repo ahora incluye un `CMakeLists.txt` raiz para compilar incrementalmente el
C++ generado por `XenonRecomp`.

Comandos principales:

- `scripts\02_syntax_check.bat`: valida una unidad generada de prueba.
- `scripts\03_build_incremental.bat`: configura Ninja/Clang y compila el target
  CMake `ufc_native_incremental`.
- `scripts\04_compile_sweep.bat`: compila cada `ppc_output\ppc_recomp.N.cpp`
  por separado y genera reportes en `logs\compile_sweep`.

Estado de validacion:

- El sweep completo pasa las 396 unidades sin errores.
- El codigo regenerado no contiene llamadas a `__builtin_debugtrap()`.
- El build CMake incremental produce correctamente todos los objetos x64.

El siguiente paso tecnico es enlazar el primer ejecutable nativo x64.

## Nota legal y practica

Este workspace no incluye herramientas, claves, cracks ni archivos distribuidos del juego.
Los scripts asumen una copia local propia y solo crean inventario/configuracion para analisis.