#
project(SDL_Learn)

cmake_minimum_required(VERSION 3.10)

SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/CMakeModules/;${CMAKE_MODULE_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)

INCLUDE (globalsetting)

add_subdirectory(01sdl_learn)
add_subdirectory(02sdl_learn)
add_subdirectory(03sdl_learn)
add_subdirectory(04sdl_learn)
add_subdirectory(05sdl_learn)
add_subdirectory(06sdl_learn)
add_subdirectory(07sdl_learn)
add_subdirectory(08sdl_learn)
add_subdirectory(09sdl_learn)
add_subdirectory(10sdl_learn)
add_subdirectory(11sdl_learn)
