#pragma once
#include <cstddef>
#include <cstdint>

#include "orbis/abi.h"

namespace orbis {

// Reserva un rango de direcciones fijo para el guest. Devuelve false si el SO no lo concede.
bool guest_reserve(GuestAddr addr, std::size_t size);

// Hace accesible (RWX por ahora, se afinará por segmento) un rango ya reservado.
bool guest_commit(GuestAddr addr, std::size_t size);

// Libera un rango reservado.
void guest_release(GuestAddr addr, std::size_t size);

}  // namespace orbis
