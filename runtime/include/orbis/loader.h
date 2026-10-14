#pragma once
#include <string>

#include "orbis/abi.h"

namespace orbis {

struct LoadedModule {
  GuestAddr base = 0;   // dirección mínima mapeada
  GuestAddr entry = 0;  // entry point (ya con base aplicada)
  std::size_t size = 0;
};

// Carga un ELF64 x86-64 YA EXTRAÍDO por el usuario (no descifra nada).
// M0: solo mapea segmentos PT_LOAD y calcula el entry point.
// Pendiente: relocations, tabla de imports por NID, TLS.
bool load_module(const std::string& path, LoadedModule& out, std::string& error);

}  // namespace orbis
