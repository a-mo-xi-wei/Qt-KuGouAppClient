cmake_minimum_required(VERSION 3.10)

project(VideoMVWidget VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(BOARDFILE
	SlideBoard/SideHideLabel.h
	SlideBoard/SideHideLabel.cpp
	SlideBoard/SlideShow.h
	SlideBoard/SlideShow.cpp
)

set(BLOCKFILE
	MVBlockWidget.h
	MVBlockWidget.cpp
	MVBlockWidget.ui
	block.css
)

set(SOURCE_FILE
    MVWidget.cpp
    MVWidget.h
	MVWidget.ui
    mv.css
	${BLOCKFILE}
	${BOARDFILE}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets MyScrollArea UpToolButton
		MyBlockWidget InteractiveButtonBase)
