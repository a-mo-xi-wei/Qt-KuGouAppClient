cmake_minimum_required(VERSION 3.10)

project(MyMenu VERSION 1.0 LANGUAGES CXX)

#更加地通用
file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    MenuBtn.hpp
    ${SRC_CPP}
    ${SRC_H}
    menu.css
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYMENU_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ListOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SortOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SongOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/TitleOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/GLOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/TrayIconMenu)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MySpdlog)
