cmake_minimum_required(VERSION 3.10)

project(MyCollection VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools)

set(SOURCE_FILE
    MyCollection.cpp
    MyCollection.h
    MyCollection.ui
    collection.css
)

#添加单曲模块
add_subdirectory(SingleSong)
#添加歌单模块
add_subdirectory(SongListWidget)
#添加专辑模块
add_subdirectory(SpecialAlbum)
#添加视频模块
add_subdirectory(CollectVideo)
#添加歌手模块
add_subdirectory(SingerWidget)
#添加设备模块
add_subdirectory(DeviceWidget)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools
                        SingleSong SongListWidget SpecialAlbum CollectVideo
                        SingerWidget DeviceWidget MySearchLineEdit MyStackedWidget)
