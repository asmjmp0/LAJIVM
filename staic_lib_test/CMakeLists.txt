cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(OUT_DIR ../../staic_lib_test)
aux_source_directory(../LAJIVM lib_src)
add_library(link_obj OBJECT ${lib_src})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/lib)
add_library(lajivm_static STATIC $<TARGET_OBJECTS:link_obj> ../LAJIVM/staticExport/staticExport.h ../LAJIVM/staticExport/staticExport.cpp)
#复制头文件到目标文件夹
file(COPY ../LAJIVM/headers DESTINATION "${OUT_DIR}")
file(COPY ../LAJIVM/staticExport/staticExport.h DESTINATION "${OUT_DIR}/staticExport")
link_directories(./lib)
include_directories(./headers)
include_directories(./staticExport)
add_executable(mytest ./src/test.cpp)
target_link_libraries(mytest lajivm_static)