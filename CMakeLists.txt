cmake_minimum_required(VERSION 3.22)
project(Clox)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CLOX_SRC)
add_executable(Clox ${CLOX_SRC})
