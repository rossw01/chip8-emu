cmake_minimum_required(VERSION 3.10)
project(chip8-emu)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/source/include)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/src/*.cpp")

add_executable(chip8-emu ${SOURCES})
