cmake_minimum_required(VERSION 3.10)
aux_source_directory(. src)
add_executable(ljasm ${src})