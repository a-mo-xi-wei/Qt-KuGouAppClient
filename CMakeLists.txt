cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 设置可执行文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置静态库文件输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置动态库文件输出目录

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(WARNING "In-source build detected! Please build out-of-source!")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 使用ccache
# 统一跨平台ccache配置
if (NOT MSVC)  # 同时适用于GCC/Clang环境
    # 检测操作系统类型
    if (WIN32)
        set(CCACHE_EXECUTABLE_NAME "ccache.exe")
    elseif (UNIX)
        set(CCACHE_EXECUTABLE_NAME "ccache")
    endif()

    # 优先使用项目自带ccache
    set(CCACHE_PROGRAM "${CMAKE_SOURCE_DIR}/tools/ccache/${CCACHE_EXECUTABLE_NAME}")

    if (EXISTS ${CCACHE_PROGRAM})
        message(STATUS "Using project ccache: ${CCACHE_PROGRAM}")
        # 设置执行权限（仅Linux需要）
        if (UNIX)
            execute_process(COMMAND chmod +x ${CCACHE_PROGRAM})
        endif()
    else()
        # 回退到系统路径查找
        find_program(CCACHE_PROGRAM ${CCACHE_EXECUTABLE_NAME})
        if (CCACHE_PROGRAM)
            message(STATUS "Using system ccache: ${CCACHE_PROGRAM}")
        else()
            message(WARNING "ccache not found! Recommended locations:\n"
                    "  - Windows: ${CMAKE_SOURCE_DIR}/tools/ccache/ccache.exe\n"
                    "  - Linux: ${CMAKE_SOURCE_DIR}/tools/ccache/ccache")
        endif()
    endif()

    # 配置编译器包装
    if (CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
    endif()
endif()

# 定义库文件路径
set(ZLIB_IMPORT_LIB ${CMAKE_SOURCE_DIR}/tools/zlib/libs/zlib.lib)
set(ZLIB_DLL ${CMAKE_SOURCE_DIR}/tools/zlib/libs/zlib.dll)

# 验证文件存在性
if (NOT EXISTS ${ZLIB_IMPORT_LIB})
    message(FATAL_ERROR "zlib.lib not found at ${ZLIB_IMPORT_LIB}")
else()
    message(STATUS "found zlib.lib at ${ZLIB_IMPORT_LIB}")
endif()

if (NOT EXISTS ${ZLIB_DLL})
    message(FATAL_ERROR "zlib.dll not found at ${ZLIB_DLL}")
else()
    message(STATUS "found zlib.lib at ${ZLIB_DLL}")
endif()

#ui目录
add_subdirectory(ui)

#util目录
add_subdirectory(util)

#开始模块
add_subdirectory(KuGouStart)

#服务器模块
add_subdirectory(KuGouServer)
