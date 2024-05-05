cmake_minimum_required(VERSION 3.20)
project(snake-game)

add_executable(snake-game src/main.cpp)

include_directories(lib/raylib/src src lib/raygui/src)
add_subdirectory(lib/raylib)

target_link_libraries(snake-game raylib)