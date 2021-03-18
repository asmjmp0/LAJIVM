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