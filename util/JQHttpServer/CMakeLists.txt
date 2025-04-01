cmake_minimum_required(VERSION 3.10)

project(JQHttpServer VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Network Concurrent Widgets Gui)

file(GLOB SOURCE_HPP ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SOURCE_HPP}
    ${SOURCE_CPP}
    ${SOURCE_H}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Network Qt6::Concurrent Qt6::Widgets Qt6::Gui)
