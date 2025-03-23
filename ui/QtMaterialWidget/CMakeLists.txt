cmake_minimum_required(VERSION 3.10)

project(QtMaterialWidget VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(QtMaterialDialog)

add_subdirectory(QtMaterialCircularProgress)