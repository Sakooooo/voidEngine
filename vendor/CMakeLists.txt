# if(MSVC)
#   enable_language(ASM_MASM)
#   enable_language(ASM_NASM)
#   enable_language(ASM)
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreaded         "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDLL      "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDebug    "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDebugDLL "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_DEBUG_INFORMATION_FORMAT_Embedded "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_DEBUG_INFORMATION_FORMAT_ProgramDatabase "")
#   set(CMAKE_ASM_COMPILE_OPTIONS_MSVC_DEBUG_INFORMATION_FORMAT_EditAndContinue "")
# endif()

# if (POLICY CMP0194)
#   cmake_policy(SET CMP0194 NEW)
# endif()

# set(SDL_STATIC ON)
# set(SDL_SHARED OFF)
# set(SDLIMAGE_AVIF OFF)
# set(SDLMIXER_WAVPACK OFF)
# set(BUILD_SHARED_LIBS OFF)

# add_subdirectory(SDL)

find_package(SDL3 CONFIG REQUIRED)

find_package(SDL3_image CONFIG REQUIRED)

find_package(Lua REQUIRED)

add_library(vendor INTERFACE)

target_include_directories(vendor PRIVATE
  ${SDL3_INCLUDE_DIRECTORIES}
  ${SDL3_image_INCLUDE_DIRECTORIES}
  ${Lua_INCLUDE_DIRS}
)

target_link_libraries(vendor INTERFACE SDL3::SDL3-static ${Lua_LIBRARIES} SDL3_image::SDL3_image-static)
