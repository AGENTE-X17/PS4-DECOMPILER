#include "orbis/loader.h"

#include <cstdio>
#include <cstring>
#include <vector>

#include "orbis/memory.h"

namespace orbis {

namespace {

// Estructuras ELF64 mínimas (propias, para no depender de <elf.h> en Windows).
struct Elf64Ehdr {
  unsigned char ident[16];
  std::uint16_t type, machine;
  std::uint32_t version;
  std::uint64_t entry, phoff, shoff;
  std::uint32_t flags;
  std::uint16_t ehsize, phentsize, phnum, shentsize, shnum, shstrndx;
};

struct Elf64Phdr {
  std::uint32_t type, flags;
  std::uint64_t offset, vaddr, paddr, filesz, memsz, align;
};

constexpr std::uint32_t PT_LOAD = 1;
constexpr std::uint16_t EM_X86_64 = 62;
constexpr std::size_t kPage = 0x1000;

std::uint64_t align_down(std::uint64_t v) { return v & ~(kPage - 1); }
std::uint64_t align_up(std::uint64_t v) { return (v + kPage - 1) & ~(kPage - 1); }

}  // namespace

bool load_module(const std::string& path, LoadedModule& out, std::string& error) {
  FILE* f = std::fopen(path.c_str(), "rb");
  if (!f) { error = "no se pudo abrir " + path; return false; }

  Elf64Ehdr eh{};
  if (std::fread(&eh, sizeof eh, 1, f) != 1 || std::memcmp(eh.ident, "\x7f" "ELF", 4) != 0) {
    std::fclose(f);
    error = "no es un ELF valido (recuerda: debe estar ya descifrado/extraido)";
    return false;
  }
  if (eh.machine != EM_X86_64 || eh.ident[4] != 2) {
    std::fclose(f);
    error = "se esperaba ELF64 x86-64";
    return false;
  }

  std::vector<Elf64Phdr> ph(eh.phnum);
  std::fseek(f, static_cast<long>(eh.phoff), SEEK_SET);
  if (std::fread(ph.data(), sizeof(Elf64Phdr), ph.size(), f) != ph.size()) {
    std::fclose(f);
    error = "program headers truncados";
    return false;
  }

  for (const auto& p : ph) {
    if (p.type != PT_LOAD) continue;
    GuestAddr start = align_down(p.vaddr);
    std::size_t len = align_up(p.vaddr + p.memsz) - start;

    if (!guest_reserve(start, len) || !guest_commit(start, len)) {
      std::fclose(f);
      error = "no se pudo reservar la direccion fija pedida por el modulo";
      return false;
    }
    std::fseek(f, static_cast<long>(p.offset), SEEK_SET);
    if (std::fread(reinterpret_cast<void*>(p.vaddr), 1, p.filesz, f) != p.filesz) {
      std::fclose(f);
      error = "segmento truncado";
      return false;
    }
    if (out.base == 0 || start < out.base) out.base = start;
    if (start + len > out.base + out.size) out.size = start + len - out.base;
  }
  std::fclose(f);

  out.entry = eh.entry;
  return true;
}

}  // namespace orbis
