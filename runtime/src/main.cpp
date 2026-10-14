#include <cstdio>
#include <string>

#include "orbis/abi.h"
#include "orbis/loader.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::fprintf(stderr, "uso: orbis_run <modulo.elf>\n");
    return 1;
  }

  orbis::LoadedModule mod;
  std::string err;
  if (!orbis::load_module(argv[1], mod, err)) {
    std::fprintf(stderr, "error: %s\n", err.c_str());
    return 2;
  }

  std::printf("modulo cargado: base=0x%llx tamano=0x%zx entry=0x%llx\n",
              (unsigned long long)mod.base, mod.size, (unsigned long long)mod.entry);

  // M0 todavia no resuelve imports ni relocations: saltar al entry se activara
  // cuando exista la tabla NID -> HLE. Por ahora solo validamos el mapeo.
  return 0;
}
