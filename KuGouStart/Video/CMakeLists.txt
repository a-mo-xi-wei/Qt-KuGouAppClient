cmake_minimum_required(VERSION 3.10)

project(Video VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools)

set(SOURCE_FILE
   Video.cpp
   Video.h
   Video.ui
   video.css
)
#添加视频频道模块
add_subdirectory(VideoChannelWidget)
#添加MV模块
add_subdirectory(MVWidget)
#添加视频模块
add_subdirectory(VideoWidget)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools
                        VideoChannelWidget MVWidget VideoWidget
                        MyScrollArea MyStackedWidget)
