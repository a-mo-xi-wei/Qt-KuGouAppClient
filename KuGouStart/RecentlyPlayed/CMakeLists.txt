cmake_minimum_required(VERSION 3.10)

project(RecentlyPlayed VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE
    RecentlyPlayed.cpp
    RecentlyPlayed.h
    RecentlyPlayed.ui
    recently.css
)

#包含单曲模块
add_subdirectory(SingleSong)
#包含歌单模块
add_subdirectory(RecentlySongList)
#包含视频模块
add_subdirectory(RecentlyVideoWidget)
#包含音乐频道模块
add_subdirectory(SongChannel)
#包含MV频道模块
add_subdirectory(MVChannel)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyStackedWidget MyCheckBox
                        RecentlySingleSong RecentlySongList RecentlyVideoWidget
                        RecentlySongChannel RecentlyMVChannel)
