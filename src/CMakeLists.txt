set(SOURCES
  main.cpp
  engine.cpp
  gui.cpp
  world.cpp
  mind.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# FindLua (and vcpkg's wrapper for it) export LUA_INCLUDE_DIR / LUA_LIBRARIES,
# all uppercase. The SDL3 / imgui imported targets carry their own include
# directories, so nothing extra is needed for them.
message(STATUS "Lua libraries: ${LUA_LIBRARIES}")

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LUA_INCLUDE_DIR}
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # clang flags #embed as a C23 extension even in C++26 mode; it's intentional.
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-c23-extensions)
endif()

if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3-static
    ${LUA_LIBRARIES}
    SDL3_image::SDL3_image-static
    imgui::imgui
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    ${LUA_LIBRARIES}
    SDL3_image::SDL3_image
    imgui::imgui
  )
endif()
