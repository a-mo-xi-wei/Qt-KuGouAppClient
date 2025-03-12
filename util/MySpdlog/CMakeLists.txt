cmake_minimum_required(VERSION 3.10)

project(MySpdlog VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/*.h)
file(GLOB SOURCE_CFG_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/cfg/*.h)
file(GLOB SOURCE_FMT_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/fmt/*.h)
file(GLOB SOURCE_DETAILS_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/details/*.h)
file(GLOB SOURCE_SINKS_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/sinks/*.h)

set(SOURCE_FILE
        include/logger.hpp
        include/mylog.hpp
        ${SOURCE_CPP}
        ${SOURCE_H}
        ${SOURCE_CFG_H}
        ${SOURCE_FMT_H}
        ${SOURCE_DETAILS_H}
        ${SOURCE_SINKS_H}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_COMPILED_LIB)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()
