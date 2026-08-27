CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

# Game engines are built statically cause that's what it do Yugi!

IF (WIN32)
  set(VCPKG_TARGET_TRIPLET "x64-windows-static")
ELSE()
  # Do people seriously use vcpkg on anything other than windows :D
ENDIF()

project(voidEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(SDL3 CONFIG REQUIRED)

find_package(SDL3_image CONFIG REQUIRED)

find_package(Lua REQUIRED)

find_package(imgui CONFIG REQUIRED)

add_subdirectory(src)
