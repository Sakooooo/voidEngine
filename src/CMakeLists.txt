SET(SOURCES
  main.cpp
  engine.cpp
  gui.cpp
  world.cpp
)

include_directories(./)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${SDL3_INCLUDE_DIRECTORIES}
  ${SDL3_image_INCLUDE_DIRECTORIES}
  ${Lua_INCLUDE_DIRS}
)

if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3-static
    ${Lua_LIBRARIES}
    SDL3_image::SDL3_image-static
    imgui::imgui
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    ${Lua_LIBRARIES}
    SDL3_image::SDL3_image
    imgui::imgui
  )
endif()
