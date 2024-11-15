cmake_minimum_required(VERSION 3.10)

project(MusicCloudDisk VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools)

set(SOURCE_FILE
    MusicCloudDisk.cpp
    MusicCloudDisk.h
    MusicCloudDisk.ui
    cloud.css
)

#包含已上传歌曲模块
add_subdirectory(UploadedSong)
#包含正在上传歌曲模块
add_subdirectory(UploadingSong)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools
                        MyStackedWidget CloudUploadedSong CloudUploadingSong)
