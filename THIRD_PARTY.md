# Código de terceros

## shadPS4 (https://github.com/shadps4-emu/shadPS4)

- Licencia: GPL-2.0-or-later (ver `third_party/shadps4/LICENSE` y `third_party/shadps4/LICENSES/`).
- Commit de origen: ver `third_party/shadps4/UPSTREAM_COMMIT.txt`.
- Copyright: shadPS4 Emulator Project. Cada archivo conserva su cabecera SPDX original; no se deben quitar.
- Estado: copia sin modificar, tal como está upstream. Cualquier cambio propio debe marcarse en el archivo
  (comentario `// Modified for orbis-port-base: ...`) y mantener la licencia.

### Qué se copió y para qué

| Ruta en `third_party/shadps4/src/` | Uso previsto | Hito |
|---|---|---|
| `core/loader/` (elf, symbols_resolver, dwarf) | Parseo de ELF y resolución de símbolos | M0 |
| `core/aerolib/` | Tabla NID -> nombre de símbolo (solo nombres públicos, sin claves) | M0 |
| `core/address_space.*`, `core/tls.*` | Mapa de memoria fija y manejo de TLS/`fs` | M0/M1 |
| `shader_recompiler/` (completo) | Recompilador de shaders GCN -> SPIR-V | M3 |
| `common/*` (subconjunto) | Utilidades de las que dependen los anteriores | M0 |

### Deliberadamente NO copiado

- Todo lo relacionado con descifrado de ejecutables, claves y licencias de contenido.
- `core/libraries/` (HLE de libSce*): se copiará módulo a módulo cuando lo necesite un port concreto.
- `video_core/`, `core/linker.*`, `core/module.*`, `core/memory.*`: muy acoplados entre sí; se integrarán después.

### Dependencias externas que el código copiado necesita

fmt, boost (container, intrusive), magic_enum, sirit, SPIRV-Headers, xbyak, robin-map, nlohmann-json.
Hoy NO están enlazadas en el CMake: el código copiado todavía no se compila dentro de este proyecto.

### Obligaciones al distribuir

1. Publicar el código fuente completo de este proyecto bajo GPL-2.0-or-later (o compatible).
2. Mantener `LICENSE`, cabeceras SPDX y este archivo.
3. Dar crédito visible a shadPS4 en el README del proyecto.
