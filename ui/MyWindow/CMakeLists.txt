cmake_minimum_required(VERSION 3.10)

project(MyWindow VERSION 1.0 LANGUAGES CXX)

file(GLOB BASICWINDOW_H ${CMAKE_CURRENT_SOURCE_DIR}/BasicWindow/*.h)
file(GLOB BASICWINDOW_CPP ${CMAKE_CURRENT_SOURCE_DIR}/BasicWindow/*.cpp)

file(GLOB_RECURSE MAINWINDOW_H ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/*.h)
file(GLOB_RECURSE MAINWINDOW_CPP ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/*.cpp)

set(SOURCE_FILE
        ${BASICWINDOW_H}
        ${BASICWINDOW_CPP}
        ${MAINWINDOW_H}
        ${MAINWINDOW_CPP}
        MyTrayIcon/MyTrayIcon.h
        MyTrayIcon/MyTrayIcon.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYWINDOW_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        BasicWindow
        MyTrayIcon
        MainWindow
        MainWindow/WaterDrop
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Core MySpdlog
        MyMenu MySearchLineEdit MyButton MyDialog DynamicBackground)
