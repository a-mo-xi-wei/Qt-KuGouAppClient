cmake_minimum_required(VERSION 3.10)

project(Live VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(DIRFILE1
    LiveBigLeftWidget/LiveBigLeftWidget.h
    LiveBigLeftWidget/LiveBigLeftWidget.cpp
    LiveBigLeftWidget/LiveBigLeftWidget.ui
)
set(DIRFILE2
    LiveBlockWidget/LiveBlockWidget.h
    LiveBlockWidget/LiveBlockWidget.cpp
    LiveBlockWidget/LiveBlockWidget.ui
)
set(DIRFILE3
        LiveCommonPartWidget/LiveCommonPartWidget.h
        LiveCommonPartWidget/LiveCommonPartWidget.cpp
        LiveCommonPartWidget/LiveCommonPartWidget.ui
)
set(DIRFILE4
    LiveMusicPartWidget/LiveMusicPartWidget.h
    LiveMusicPartWidget/LiveMusicPartWidget.cpp
    LiveMusicPartWidget/LiveMusicPartWidget.ui
)
set(DIRFILE5
    PopularWidgets/PopularLeftWidget.h
    PopularWidgets/PopularLeftWidget.cpp
    PopularWidgets/PopularRightToolButton.h
    PopularWidgets/PopularRightToolButton.cpp
)

set(SOURCE_FILE
    Live.cpp
    Live.h
    Live.ui
    live.css
    ${DIRFILE1}
    ${DIRFILE2}
    ${DIRFILE3}
    ${DIRFILE4}
    ${DIRFILE5}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyScrollArea MyBlockWidget MyAsyncThread)
