#include "orbis/memory.h"

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#endif

namespace orbis {

#if defined(_WIN32)

bool guest_reserve(GuestAddr addr, std::size_t size) {
  void* p = VirtualAlloc(reinterpret_cast<void*>(addr), size, MEM_RESERVE, PAGE_NOACCESS);
  return p == reinterpret_cast<void*>(addr);
}

bool guest_commit(GuestAddr addr, std::size_t size) {
  void* p = VirtualAlloc(reinterpret_cast<void*>(addr), size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  return p != nullptr;
}

void guest_release(GuestAddr addr, std::size_t) {
  VirtualFree(reinterpret_cast<void*>(addr), 0, MEM_RELEASE);
}

#else  // Linux/macOS: solo para desarrollo y tests del loader

bool guest_reserve(GuestAddr addr, std::size_t size) {
  void* p = mmap(reinterpret_cast<void*>(addr), size, PROT_NONE,
                 MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
  return p == reinterpret_cast<void*>(addr);
}

bool guest_commit(GuestAddr addr, std::size_t size) {
  return mprotect(reinterpret_cast<void*>(addr), size, PROT_READ | PROT_WRITE | PROT_EXEC) == 0;
}

void guest_release(GuestAddr addr, std::size_t size) {
  munmap(reinterpret_cast<void*>(addr), size);
}

#endif

}  // namespace orbis
