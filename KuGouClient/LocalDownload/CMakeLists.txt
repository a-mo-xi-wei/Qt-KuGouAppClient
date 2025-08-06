cmake_minimum_required(VERSION 3.10)

project(LocalDownload VERSION 1.0 LANGUAGES CXX)

#更加地通用
file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SRC_UI ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

set(SOURCE_FILE
        ${SRC_CPP}
        ${SRC_H}
        ${SRC_UI}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalSong
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadedSong
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadedVideo
        ${CMAKE_CURRENT_SOURCE_DIR}/Downloading)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia MyHttpClient MyMaskWidget
        MusicItemWidget MySearchLineEdit MyScrollArea MySlidingStackedWidget MyAsyncThread SApp)
