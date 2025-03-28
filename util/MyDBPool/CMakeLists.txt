cmake_minimum_required(VERSION 3.10)

project(MyDBPoll VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Sql Network Gui Widgets)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB SOURCE_COMMON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.cpp)
file(GLOB SOURCE_COMMON_C ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.c)
file(GLOB SOURCE_COMMON_H ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.h)

set(SOURCE_FILE
    ${SOURCE_CPP}
    ${SOURCE_H}
    ${SOURCE_COMMON_CPP}
    ${SOURCE_COMMON_C}
    ${SOURCE_COMMON_H}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon)

target_link_libraries(${PROJECT_NAME} Qt6::Sql Qt6::Network Qt6::Gui
                        Qt6::Widgets MyQsLog z)
