set(GUI_SOURCES
  guis/debug.cpp
  guis/tree.cpp
)

set(SOURCES
  main.cpp
  engine.cpp
  gui.cpp
  ${GUI_SOURCES}
  world.cpp
  script.cpp
  camera.cpp
  system.cpp
  rectangle.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # clang flags #embed as a C23 extension even in C++26 mode; it's intentional.
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-c23-extensions)
endif()

find_package(spdlog CONFIG REQUIRED) # logging library
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3-static
    unofficial::luau::Luau.VM
    unofficial::luau::Luau.Ast
    unofficial::luau::Luau.Common
    unofficial::luau::Luau.Config
    unofficial::luau::Luau.Compiler
    SDL3_image::SDL3_image-static
    imgui::imgui
    spdlog::spdlog
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    Luau.VM
    Luau.Ast
    Luau.Common
    Luau.Config
    Luau.Compiler
    SDL3_image::SDL3_image
    imgui
    spdlog::spdlog
  )
endif()


# Copy every script next to the executable, each file gets its own rule so
# editing (or adding) a script recopies it on the next build without relinking
set(SCRIPTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets/scripts)
set(SCRIPTS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/scripts)
file(GLOB_RECURSE SCRIPT_FILES CONFIGURE_DEPENDS ${SCRIPTS_SOURCE_DIR}/*)

set(SCRIPT_OUTPUTS)
foreach(SCRIPT_FILE ${SCRIPT_FILES})
  file(RELATIVE_PATH SCRIPT_RELATIVE ${SCRIPTS_SOURCE_DIR} ${SCRIPT_FILE})
  set(SCRIPT_OUTPUT ${SCRIPTS_OUTPUT_DIR}/${SCRIPT_RELATIVE})
  add_custom_command(
    OUTPUT ${SCRIPT_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SCRIPT_FILE} ${SCRIPT_OUTPUT}
    DEPENDS ${SCRIPT_FILE}
    COMMENT "Copying script ${SCRIPT_RELATIVE}"
  )
  list(APPEND SCRIPT_OUTPUTS ${SCRIPT_OUTPUT})
endforeach()

add_custom_target(copy_scripts ALL DEPENDS ${SCRIPT_OUTPUTS})
add_dependencies(${PROJECT_NAME} copy_scripts)
