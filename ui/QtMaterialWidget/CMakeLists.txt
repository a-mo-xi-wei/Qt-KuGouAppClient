cmake_minimum_required(VERSION 3.10)

project(QtMaterialWidget VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets StateMachine Core)

add_subdirectory(MaterialCommon)

add_subdirectory(QtMaterialDialog)

add_subdirectory(QtMaterialCircularProgress)

add_subdirectory(QtMaterialSnackbar)

add_subdirectory(QtMaterialTextField)

