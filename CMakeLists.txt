cmake_minimum_required(VERSION 3.12)

project(Midi2 C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(OUT_DIR ./out)

add_subdirectory(src)
add_compile_options(-Wall)
