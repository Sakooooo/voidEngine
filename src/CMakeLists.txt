set(SOURCES
  main.cpp
  engine.cpp
  gui.cpp
  world.cpp
  mind.cpp
  script.cpp
  camera.cpp
  lua_helpers/render.cpp
  lua_helpers/event.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
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
    unofficial::luau::Luau.VM
    unofficial::luau::Luau.Ast
    unofficial::luau::Luau.Common
    unofficial::luau::Luau.Config
    unofficial::luau::Luau.Compiler
    SDL3_image::SDL3_image-static
    imgui::imgui
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
    imgui::imgui
  )
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
	    ${CMAKE_SOURCE_DIR}/assets/scripts
	    $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts
)
