cmake_minimum_required(VERSION 3.10)

project(MaterialTextField VERSION 1.0 LANGUAGES CXX)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILES
    ${SOURCE_CPP}
    ${SOURCE_H}
    ../MaterialCommon/qtmaterialstyle.h
    ../MaterialCommon/qtmaterialstyle.cpp
    ../MaterialCommon/qtmaterialtheme.h
    ../MaterialCommon/qtmaterialtheme_p.h
    ../MaterialCommon/qtmaterialtheme.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE MATERIALSNACKBAR_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::StateMachine)
