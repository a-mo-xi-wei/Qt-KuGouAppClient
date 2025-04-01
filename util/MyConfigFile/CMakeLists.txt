cmake_minimum_required(VERSION 3.10)

project(MyConfigFile VERSION 1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED MyConfigFile.cpp MyConfigFile.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE MYCONFIG_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)