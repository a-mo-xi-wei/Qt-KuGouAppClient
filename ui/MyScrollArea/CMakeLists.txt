cmake_minimum_required(VERSION 3.10)

project(MyScrollArea VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE
    MyScrollArea.h
    MyScrollArea.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYSCROLLAREA_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyButton MySpdlog)
