# Plan de arranque

## Fase 0: Inventario local

- Ejecutar `scripts/00_inventory.ps1`.
- Confirmar rutas de ISO, Xenia, Ghidra y XenonRecomp.
- Detectar si hay una extraccion previa de UFC.

## Fase 1: Extraccion de la copia propia

- Extraer la ISO a `D:\Xbox360\UFC Undisputed 3 Extracted` o una ruta equivalente.
- Verificar que exista `default.xex`.
- Registrar tamano, hash y lista de archivos principales.

Bloqueo actual: no se encontro una herramienta de extraccion instalada en PATH. Candidatas:
`Velocity-XEXISO`, `Xbox Image Browser`, `extract-xiso`, `xextool`, o una utilidad equivalente
ya confiable para Xbox 360.

## Fase 2: Analisis ejecutable

- Copiar o apuntar el `default.xex` extraido desde `config/ufc_undisputed_3.toml`.
- Ejecutar `scripts/01_analyze_xex.ps1 -XexPath "<ruta al default.xex>"`.
- Revisar `config/ufc_switch_tables.toml`.

## Fase 3: Ghidra

- Crear proyecto Ghidra separado para UFC.
- Importar `default.xex` o imagen volcada/desempaquetada cuando este disponible.
- Identificar:
  - entry point
  - imports XAM/XBOXKRNL/D3D
  - funciones save/restore GPR/FPR/VMX
  - patrones de jump tables
  - cadenas y rutas de archivos

## Fase 4: Recompilacion experimental

- Completar direcciones obligatorias del TOML.
- Ejecutar `XenonRecomp`.
- Clasificar errores por: instrucciones PPC faltantes, jump tables, excepciones, imports,
  MMIO, filesystem, graphics, audio, input.

## Fase 5: Runtime por capas

- Stubs minimos para kernel/filesystem.
- Validacion de arranque hasta primera pantalla o primer fallo estable.
- Integracion grafica/audio/input solo cuando el flujo de CPU sea repetible.
