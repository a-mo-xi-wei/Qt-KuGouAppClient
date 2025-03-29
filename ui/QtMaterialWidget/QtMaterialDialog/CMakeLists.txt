cmake_minimum_required(VERSION 3.10)

project(MaterialDialog VERSION 1.0 LANGUAGES CXX)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Windows(MSVC)平台下QtMaterialOverlayWidget类的虚函数符号未被正确导出或编译到动态库中,一坨。。。

set(SOURCE_FILES
    ${SOURCE_CPP}
    ${SOURCE_H}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE MATERIALDIALOG_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::StateMachine MaterialCommon)
