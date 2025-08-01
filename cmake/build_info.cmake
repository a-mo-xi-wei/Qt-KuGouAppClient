# 输出 CMake 版本和构建系统类型
message("CMake Version: ${CMAKE_VERSION}")
message("Generator: ${CMAKE_GENERATOR}")

# 输出编译器信息
message("C Compiler ID: ${CMAKE_C_COMPILER_ID}")
message("C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# 输出构建类型和编译选项
message("Build Type: ${CMAKE_BUILD_TYPE}")
message("C++ Compiler Flags: ${CMAKE_CXX_FLAGS}")

# 输出链接选项
message("Executable Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")

# 输出构建和源代码目录
message("Build Directory: ${CMAKE_BINARY_DIR}")
message("Source Directory: ${CMAKE_SOURCE_DIR}")

# 输出目标架构
message("Target Processor: ${CMAKE_SYSTEM_PROCESSOR}")

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(WARNING "In-source build detected! Please build out-of-source!")
endif()

message(STATUS "Using project ccache: ${CCACHE_PROGRAM}")

if (CCACHE_PROGRAM)
    message(STATUS "Using system ccache: ${CCACHE_PROGRAM}")
else()
    message(WARNING "ccache not found! Recommended locations:\n"
            "  - Windows: ${CMAKE_SOURCE_DIR}/tools/ccache/ccache.exe\n"
            "  - Linux: ${CMAKE_SOURCE_DIR}/tools/ccache/ccache")
endif()

# 验证文件存在性
if (NOT EXISTS ${ZLIB_IMPORT_LIB})
    message(FATAL_ERROR "libzlib.dll.a not found at ${ZLIB_IMPORT_LIB}")
else()
    message(STATUS "found libzlib.dll.a at ${ZLIB_IMPORT_LIB}")
endif()

if (NOT EXISTS ${ZLIB_DLL})
    message(FATAL_ERROR "zlib.dll not found at ${ZLIB_DLL}")
else()
    message(STATUS "found zlib.dll at ${ZLIB_DLL}")
endif()

# 验证文件存在性
if (NOT EXISTS ${SDL2_IMPORT_LIB})
    message(FATAL_ERROR "libSDL2.dll.a not found at ${SDL2_IMPORT_LIB}")
else()
    message(STATUS "found libSDL2.dll.a at ${SDL2_IMPORT_LIB}")
endif()

if (NOT EXISTS ${SDL2_DLL})
    message(FATAL_ERROR "SDL2.dll not found at ${SDL2_DLL}")
else()
    message(STATUS "found SDL2.dll at ${SDL2_DLL}")
endif()

# 验证目录存在性
if (NOT EXISTS ${FFMPEG_IMPORT_LIB_PATH})
    message(FATAL_ERROR "FFmpeg library directory not found at ${FFMPEG_IMPORT_LIB_PATH}")
else()
    message(STATUS "found FFmpeg library directory at ${FFMPEG_IMPORT_LIB_PATH}")
endif()

if (NOT EXISTS ${FFMPEG_BIN_PATH})
    message(FATAL_ERROR "FFmpeg binary directory not found at ${FFMPEG_BIN_PATH}")
else()
    message(STATUS "found FFmpeg binary directory at ${FFMPEG_BIN_PATH}")
endif()

message(STATUS "found qrc files :")
foreach(qrc ${QRC_FILE})
    message(STATUS "  ${qrc}")
endforeach()
