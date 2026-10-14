#pragma once
#include <cstdint>

// Las funciones HLE que el guest (PS4) llama usan System V AMD64.
// Declarándolas así, el compilador genera el prólogo correcto y no hace falta thunk.
#if defined(__clang__) || defined(__GNUC__)
#define ORBIS_ABI __attribute__((sysv_abi))
#else
#error "Se requiere Clang o GCC para ORBIS_ABI"
#endif

namespace orbis {

using GuestAddr = std::uint64_t;

// Llama a una función del guest (entry point, callbacks) con ABI System V.
// Implementado en abi/trampoline.cpp.
std::uint64_t call_guest(GuestAddr fn, std::uint64_t a0 = 0, std::uint64_t a1 = 0,
                         std::uint64_t a2 = 0, std::uint64_t a3 = 0);

// Entrada de la tabla de imports: NID -> implementación host.
struct ImportEntry {
  const char* nid;   // hash del símbolo (base64 corto)
  const char* name;  // nombre legible, solo para depuración
  void* fn;          // puntero a función ORBIS_ABI
};

}  // namespace orbis
