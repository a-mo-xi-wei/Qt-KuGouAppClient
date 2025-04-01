cmake_minimum_required(VERSION 3.10)

project(MySpdlog VERSION 1.0)

file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    include/logger.hpp
    ${SRC_CPP}
    ${SRC_H}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_COMPILED_LIB)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} Qt6::Core)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()
