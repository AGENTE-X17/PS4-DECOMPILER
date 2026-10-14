#include "orbis/abi.h"

namespace orbis {

// Clang genera la llamada con convención System V aunque el host sea Windows,
// gracias al atributo sysv_abi en el tipo de función.
using GuestFn = std::uint64_t(ORBIS_ABI*)(std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t);

std::uint64_t call_guest(GuestAddr fn, std::uint64_t a0, std::uint64_t a1, std::uint64_t a2,
                         std::uint64_t a3) {
  return reinterpret_cast<GuestFn>(fn)(a0, a1, a2, a3);
}

}  // namespace orbis
