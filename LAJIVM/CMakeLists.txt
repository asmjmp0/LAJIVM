cmake_minimum_required(VERSION 3.17)
project(LAJIVM)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LAJIVM
        bin_read.cpp
        bin_read.h
        BinType.h
        ErrorList.h
        ExecuteInstruction.cpp
        ExecuteInstruction.h
        init.cpp
        init.h
        ins_stack.cpp
        JITexec.cpp
        JITexec.h
        LAJIVM.cpp
        LenInstruction.cpp
        LenInstruction.h
        lvm_debug.cpp
        MemoryManage.cpp
        MemoryManage.h
        ModuleManage.cpp
        ModuleManage.h
        VmRegister.h)
