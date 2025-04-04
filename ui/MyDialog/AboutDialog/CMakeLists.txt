cmake_minimum_required(VERSION 3.10)

project(AboutDialog VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE
    AboutDialog.cpp
    AboutDialog.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE ABOUTDIALOG_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network MaterialDialog
        MySpdlog MyButton ElaWidgetTools MyAsyncThread MyHttpClient)
