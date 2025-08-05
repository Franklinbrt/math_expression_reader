cmake_minimum_required(VERSION 3.10)
project(matrix_read)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(libs/exprtk)

add_executable(matrix_read_1 src/main.cpp)
add_executable(matrix_read_2 src/main_2.cpp)
