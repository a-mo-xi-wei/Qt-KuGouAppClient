cmake_minimum_required(VERSION 3.10)

project(ListenBook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE
    ListenBook.cpp
    ListenBook.h
    ListenBook.ui
    listen.css
)

#添加推荐模块
add_subdirectory(ListenRecommend)
#添加我的下载模块
add_subdirectory(ListenMyDownload)
#添加最近播放模块
add_subdirectory(ListenRecentlyPlay)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets ListenRecommend
                    ListenMyDownload ListenRecentlyPlay MyStackedWidget)
