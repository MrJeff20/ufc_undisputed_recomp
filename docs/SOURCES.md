# Fuentes y notas tecnicas

## Recompilacion Xbox 360

- `rexdex/recompiler` documenta el enfoque clasico: extraer XEX, decompilar a una imagen
  intermedia, generar codigo y lanzar con un runtime de host. El README tambien recalca
  que los juegos reales son dificiles por CPU PowerPC, endianess, XEX cifrado/empacado,
  GPU Xenos/EDRAM y llamadas inlined.
- `sp00nznet/360tools` se presenta como toolkit/playbook actual alrededor de ReXGlue:
  extraer `default.xex` y assets, inicializar proyecto, codegen PowerPC a C++, completar
  stubs/hooks y compilar con CMake.
- `XenonRecomp` local convierte codigo PowerPC de Xbox 360 a C++, pero no proporciona
  runtime completo. Su README indica que el runtime y la adaptacion por juego quedan a
  cargo del proyecto.

## UFC Undisputed 3

- `ufc-u3-model-converter` muestra que el juego usa archivos `*.pac` y `*.mpc` para
  modelos/contenido, con tooling comunitario para conversion hacia Xbox 360/Xenia.
- El hilo de ZenHAX sobre UFC Undisputed 3 enlaza un script QuickBMS `ufc3.bms` para
  unpack de archivos del juego, util para inventariar assets despues de extraer la ISO.

## XEX

- La documentacion Free60/XEX describe `XEX2` como contenedor ejecutable de Xbox 360,
  big-endian, con headers opcionales, contenido cifrado/compactado y PE PowerPC interno.

## Implicacion para este proyecto

La ruta realista empieza por extraccion + inventario + emulacion/debug en Xenia, y luego
recompilacion incremental: primero generar artefactos de analisis, despues resolver
runtime, imports, memoria, filesystem, GPU, audio e input por capas.
