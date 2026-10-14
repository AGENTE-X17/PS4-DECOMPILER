# Plantilla de port

Copia esta carpeta a `ports/<nombre-del-juego>/`. Contiene SOLO tu código:

- `patches/`: parches propios (en forma de diff o reglas, nunca el binario del juego).
- `imports.cpp`: implementaciones HLE especificas de este titulo.
- `port.toml`: ruta al dump del USUARIO (fuera del repo) y opciones.

El dump, los binarios extraidos y cualquier salida generada deben estar en `.gitignore`.
