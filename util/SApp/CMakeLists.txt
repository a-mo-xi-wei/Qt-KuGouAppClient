cmake_minimum_required(VERSION 3.10)

project(SApp VERSION 1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED SApp.cpp SApp.h SConfigFile.cpp SConfigFile.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE SAPP_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets)