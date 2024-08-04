cmake_minimum_required(VERSION 3.22)
project(clox)

set(CMAKE_C_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

# add_definitions(-DDEBUG)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CLOX_SRC)
add_executable(clox ${CLOX_SRC})
