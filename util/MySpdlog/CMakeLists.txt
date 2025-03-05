cmake_minimum_required(VERSION 3.10)

project(MySpdlog)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()
