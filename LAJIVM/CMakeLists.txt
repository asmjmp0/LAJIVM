cmake_minimum_required(VERSION 3.10)
aux_source_directory(. src)
set(CMAKE_CXX_STANDARD 14)
add_executable(lajivm ${src})