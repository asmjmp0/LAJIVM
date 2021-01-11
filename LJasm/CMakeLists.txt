cmake_minimum_required(VERSION 3.17)
project(LJasm)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LJasm
        AnalyseCode.cpp
        AnalyseCode.h
        JITcomp.cpp
        JITcomp.h
        labelstruct.cpp
        labelstruct.h
        LJasm.cpp
        LJasm.h)