cmake_minimum_required(VERSION 3.10)

project(ListenMyDownload VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(DownloadedFile
    DownloadedWidget/DownloadedWidget.h
    DownloadedWidget/DownloadedWidget.cpp
    DownloadedWidget/DownloadedWidget.ui
)
set(DownloadingFile
    DownloadingWidget/DownloadingWidget.h
    DownloadingWidget/DownloadingWidget.cpp
    DownloadingWidget/DownloadingWidget.ui
)

set(SOURCE_FILE
    ListenMyDownload.cpp
    ListenMyDownload.h
    ListenMyDownload.ui
    ${DownloadedFile}
    ${DownloadingFile}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadedWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadingWidget)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyStackedWidget MySpdlog)
