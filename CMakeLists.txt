cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(WARNING "In-source build detected! Please build out-of-source!")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT MSVC)
    set(CCACHE_PROGRAM "${CMAKE_SOURCE_DIR}/tools/ccache.exe")

    if (EXISTS ${CCACHE_PROGRAM})
        message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
    else()
        message(WARNING "ccache not found in tools directory!")
    endif()
endif()

#ui目录
add_subdirectory(ui)

#util目录
add_subdirectory(util)

#开始模块
add_subdirectory(KuGouStart)

#服务器模块
add_subdirectory(KuGouServer)