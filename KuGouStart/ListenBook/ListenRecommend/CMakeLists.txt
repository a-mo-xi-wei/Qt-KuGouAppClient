cmake_minimum_required(VERSION 3.10)

project(ListenRecommend VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(GalleryFile
    Gallery/GalleryPhotoWidget.h
    Gallery/GalleryPhotoWidget.cpp
    Gallery/GalleryWidget.h
    Gallery/GalleryWidget.cpp
)

set(ListenTableWidgetFile
    ListenTableWidget/ListenTableWidget.h
    ListenTableWidget/ListenTableWidget.cpp
    ListenTableWidget/ListenTableWidget.ui
)

set(ListenRankWidgetFile
    ListenRankWidget/ListenRankWidget.h
    ListenRankWidget/ListenRankWidget.cpp
    ListenRankWidget/RankPartWidget.h
    ListenRankWidget/RankPartWidget.cpp
    ListenRankWidget/RankPartWidget.ui
    ListenRankWidget/RankListWidget.h
    ListenRankWidget/RankListWidget.cpp
    ListenRankWidget/RankListWidget.ui
)

set(SOURCE_FILE
    ListenRecommend.cpp
    ListenRecommend.h
    ListenRecommend.ui
    ${GalleryFile}
    ${ListenTableWidgetFile}
    ${ListenRankWidgetFile}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Gallery
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenTableWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenRankWidget)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets IconBesideTextToolButton MyMenu
                        InteractiveButtonBase MyScrollArea MyBlockWidget)
