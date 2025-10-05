cmake_minimum_required(VERSION 3.10)

project(SearchResultWidget VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE
        Searchresultwidget.h
        Searchresultwidget.cpp
)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILE})

set(LINK_OBJS ${LINK_OBJS} "$<TARGET_OBJECTS:${PROJECT_NAME}>" PARENT_SCOPE)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets UiWidgets Common)
