cmake_minimum_required(VERSION 3.24)
project(orbis_port_base LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Requiere clang (clang-cl o clang++) por __attribute__((sysv_abi))
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(WARNING "Se recomienda Clang: el runtime usa sysv_abi para el puente con el guest")
endif()

add_library(orbis_runtime STATIC
  runtime/src/abi/trampoline.cpp
  runtime/src/loader/module_loader.cpp
  runtime/src/memory/guest_memory.cpp
)
target_include_directories(orbis_runtime PUBLIC runtime/include)

add_executable(orbis_run runtime/src/main.cpp)
target_link_libraries(orbis_run PRIVATE orbis_runtime)

# Código copiado de shadPS4 (GPL-2.0-or-later). Aún no se compila aquí:
# requiere fmt, boost, magic_enum, sirit, xbyak, robin-map y el sistema de logging/singleton de common/.
# Se activará hito por hito (M0: core/loader + aerolib; M3: shader_recompiler).
option(ORBIS_USE_SHADPS4 "Compilar las partes copiadas de shadPS4" OFF)
