# orbis-port-base

Runtime base y herramientas para construir ports por juego de títulos PS4 a Windows.
Cada port es un proyecto separado que depende de este runtime.

## Reglas del proyecto (legal / clean-room)

- Este repositorio NO contiene código, firmware, librerías del sistema, claves ni assets de Sony ni de ningún desarrollador.
- Todo el runtime se escribe desde cero a partir de comportamiento observable y documentación pública.
- El usuario aporta su propio dump. Las herramientas procesan el dump localmente.
- No se distribuyen binarios generados a partir de un juego.
- La obtención/descifrado de dumps queda fuera del repo (paso externo del usuario).

## Licencia y código de terceros

- Este repositorio incluye código copiado de shadPS4 (GPL-2.0-or-later), por lo que todo el proyecto se
  distribuye bajo GPL-2.0-or-later y mantiene sus avisos de copyright. Detalle en `THIRD_PARTY.md`.
- Créditos: shadPS4 Emulator Project (https://github.com/shadps4-emu/shadPS4).
- "Clean-room" aquí se refiere a Sony y a los desarrolladores de juegos: nunca se copia nada suyo.
  Reutilizar código de otros emuladores open source es válido si se respeta su licencia.
- Todo código de terceros va en `third_party/<proyecto>/` (submódulo o copia con su LICENSE),
  nunca mezclado sin atribución en `runtime/`.
- Alternativa a copiar: contribuir upstream o depender de ellos como librería.

## Estructura

```
orbis-port-base/
├─ runtime/                 Runtime común (HLE de libkernel y libSce*)
│  ├─ include/orbis/        Headers públicos (ABI, tipos, handles)
│  └─ src/
│     ├─ abi/               Puente SysV <-> Win64, trampolines
│     ├─ loader/            Carga de módulos ELF ya extraídos, relocations, imports
│     ├─ memory/            Mapeo a direcciones fijas, direct memory, TLS
│     ├─ kernel/            libkernel: hilos, mutex, semáforos, tiempo, archivos
│     ├─ sce/               libSce*: audio, pad, user, save data, trofeos, red
│     └─ gfx/               GNM -> Vulkan, recompilador de shaders GCN -> SPIR-V
├─ tools/
│  ├─ import-scan/          Lista imports/NIDs que usa un juego
│  └─ port-init/            Genera el esqueleto de un port a partir del dump del usuario
├─ ports/
│  └─ _template/            Plantilla de port por juego (parches, imports propios)
├─ tests/
│  └─ homebrew/             ELF de prueba (ej. OpenOrbis) para validar el runtime
└─ docs/
```

## Hoja de ruta

1. **M0** Loader mínimo: mapear un ELF homebrew, resolver imports, saltar al entry point.
2. **M1** libkernel básica: malloc/mmap, hilos, printf a consola.
3. **M2** Ventana + Vulkan: limpiar pantalla desde un homebrew.
4. **M3** GNM mínimo: comandos de dibujo simples + primeros shaders.
5. **M4** Audio, pad, archivos con rutas virtuales (/app0, /savedata0).
6. **M5** Primer port de un juego real, usando `ports/_template`.

## Puntos técnicos clave

- Convención de llamada: el guest usa System V AMD64; Windows usa Win64.
  Las funciones HLE se declaran con `ORBIS_ABI` (sysv_abi en clang/gcc) para evitar thunks en guest->host.
  Host->guest (entry point, callbacks) pasa por trampolines en `runtime/src/abi`.
- TLS: el guest accede a `fs:`. En Windows x64 no se puede fijar la base de FS desde user-mode
  de forma portable, así que hay que parchear esos accesos al cargar el módulo (ver `memory/`).
- Memoria: el guest espera direcciones fijas y bloques "direct memory"; hay que reservarlas al inicio.
- Imports: la PS4 resuelve símbolos por NID (hash). `tools/import-scan` los lista y el runtime
  mantiene una tabla NID -> implementación.
