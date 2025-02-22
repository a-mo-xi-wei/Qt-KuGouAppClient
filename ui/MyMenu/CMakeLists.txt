cmake_minimum_required(VERSION 3.10)

project(MyMenu VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

#file(GLOB SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#file(GLOB SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#更加地通用
file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SRC_CPP}
    ${SRC_H}
    menu.css
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ListOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SortOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SongOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/TitleOptionMenu)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
