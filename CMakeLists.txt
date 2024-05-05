cmake_minimum_required(VERSION 3.20)
project(snake-game)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(snake-game src/main.cpp)

include_directories(lib/raylib/src src lib/raygui/src)

if (EMSCRIPTEN)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ASYNCIFY -sALLOW_MEMORY_GROWTH")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    find_library(RL_LIBRARY NAMES raylib PATHS lib/raylib/src NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    target_link_libraries(snake-game ${RL_LIBRARY})
else()
    add_subdirectory(lib/raylib)
    target_link_libraries(snake-game raylib)
    set_target_properties(snake-game PROPERTIES LINK_FLAGS "--preload-file ${CMAKE_SOURCE_DIR}/resources/audio@resources/audio")
endif()