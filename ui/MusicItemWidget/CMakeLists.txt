cmake_minimum_required(VERSION 3.10)

project(MusicItemWidget VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE
    MusicItemWidget.h
    MusicItemWidget.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MUSICITEMWIDGET_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyMenu ElaWidgetTools)
