cmake_minimum_required(VERSION 4.0)

# Do it for the vine
set(CMAKE_CXX_STANDARD 26)

# Game engines are built statically cause that's what it do Yugi!

if(WIN32)
  set(VCPKG_TARGET_TRIPLET "x64-windows-static")
else()
  # Do people seriously use vcpkg on anything other than windows :D

  # this shit breaks on nix i don't know why i don't care
  set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

project(voidEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(SDL3 CONFIG REQUIRED)

find_package(SDL3_image CONFIG REQUIRED)

find_package(Lua REQUIRED)

find_package(imgui CONFIG REQUIRED)

add_subdirectory(src)
