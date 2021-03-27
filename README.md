# LAJIVM
### 闲着无聊写的虚拟机
#### 整个项目用cmake构建

### target LAJIVM
虚拟机解释器的运行时

### target LJasm
虚拟机字节码的编译器

### 编译运行

#### lajivm
编译运行时
```shell
cd LAJIVM
mkdir build
cd build
cmake ..
make
```
编译静态库
```shell
cd LAJIVM
mkdir build
cd build
cmake .. -DFLAG_STATIC="TRUE" -DOUT_DIR="the path of the static library"
make
```
### ljasm
同编译lajivm运行时

### 使用静态库
```
.
├── CMakeLists.txt
├── build
├── headers -> headers of lajivm
├── lib
│   └── liblajivm_static.a
├── src
│   └── test.cc 
└── staticExport
    └── staticExport.h -> header of export
```

#### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 14)
project(test)
aux_source_directory(./src src)
LINK_DIRECTORIES(./lib)
add_executable(test ${src})
TARGET_LINK_LIBRARIES(test liblajivm_static.a)
```
#### 测试静态库代码 
```shell
cd ./LAJIVM
./build_static.sh
cd ./staic_lib_test
./build_and_test.sh
```
```c
fib_index:32
fib_value:2178309
time taken:617.291016ms
```