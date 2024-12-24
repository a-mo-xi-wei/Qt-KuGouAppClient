cmake_minimum_required(VERSION 3.10)

project(GuessLoveWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE
    GLTabWidget.cpp
    GLTabWidget.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#通过这种方式，你告诉 CMake 将该目录添加为包含目录，使得源代码能够包含同一目录中的头文件。
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
