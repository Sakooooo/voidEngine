SET(SOURCES
  main.cpp
  engine.cpp
  gui.cpp
)

include_directories(./)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
