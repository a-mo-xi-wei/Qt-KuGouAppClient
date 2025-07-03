cmake_minimum_required(VERSION 3.10)

project(ListenBook VERSION 1.0 LANGUAGES CXX)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRecommend/Gallery
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRecommend/ListenRankWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRecommend/ListenTableWidget
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenMyDownload
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenMyDownload/DownloadedWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenMyDownload/DownloadingWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRecentlyPlay
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRecommend
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MySlidingStackedWidget MyMenu MyBlockWidget
        MyButton MyAsyncThread MyScrollArea ElaWidgetTools)
