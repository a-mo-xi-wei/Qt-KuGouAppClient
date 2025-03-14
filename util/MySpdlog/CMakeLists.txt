cmake_minimum_required(VERSION 3.10)

project(MySpdlog VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/*.h)
file(GLOB SOURCE_CFG_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/cfg/*.h)
file(GLOB SOURCE_FMT_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/fmt/*.h)
file(GLOB SOURCE_DETAILS_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/details/*.h)
file(GLOB SOURCE_SINKS_H ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/sinks/*.h)

set(SOURCE_FILE
        include/logger.hpp
        include/mylog.h
        include/mylog.cpp
        ${SOURCE_CPP}
        ${SOURCE_H}
        ${SOURCE_CFG_H}
        ${SOURCE_FMT_H}
        ${SOURCE_DETAILS_H}
        ${SOURCE_SINKS_H}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_COMPILED_LIB)

# 显式设置运行时 .dll 到 bin/, 导入库到 lib/
# 跨平台输出路径配置  （根据平台优化）
if(WIN32)
    # Windows: DLL → bin/, 导入库 → lib/
    set_target_properties(${TARGET_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
else()
    # Linux/macOS: 共享库 → lib/
    set_target_properties(${TARGET_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} Qt6::Core)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()
