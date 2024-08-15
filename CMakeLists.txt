cmake_minimum_required(VERSION 3.30)
project(chip8-emu)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/source/include ${SDL2_INCLUDE_DIRS})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/src/*.cpp")

add_executable(chip8-emu ${SOURCES})

target_link_libraries(chip8-emu ${SDL2_LIBRARIES})
