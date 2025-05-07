cmake_minimum_required(VERSION 3.10)

project(MyPlayer VERSION 1.0 LANGUAGES CXX)

set(USE_PCM_PLAYER 1)
add_definitions(-DUSE_PCM_PLAYER -DENABLE_SDL)

file(GLOB_RECURSE SOURCE_SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(SOURCE_FILE
        ${SOURCE_SRC_H}
        ${SOURCE_SRC_CPP}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYPLAYER_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/ffmpeg/win64/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/SDL2/win64/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/frame
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/PcmPlayer
        ${CMAKE_CURRENT_SOURCE_DIR}/VideoPlayer
)

target_link_directories(${PROJECT_NAME} PRIVATE
        ${FFMPEG_IMPORT_LIB_PATH}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Widgets
        Qt6::Core
        ${SDL2_IMPORT_LIB}
        avformat-58
        avcodec-58
        avutil-56
        swresample-3
        swscale-5
        postproc-55
        avfilter-7
        avdevice-58
)

# 收集需要复制的 DLL 文件
set(DLL_PATHS ${ZLIB_DLL} ${SDL2_DLL})  # 包含 zlib.dll 和 SDL2.dll
file(GLOB FFMPEG_DLLS ${FFMPEG_BIN_PATH}/*.dll)  # 收集 FFmpeg 的所有 DLL
list(APPEND DLL_PATHS ${FFMPEG_DLLS})  # 将 FFmpeg DLL 添加到列表

if(WIN32)
    # 复制所有 DLL 文件到目标目录
    foreach(DLL ${DLL_PATHS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                COMMENT "Copying ${DLL} to target directory..."
        )
    endforeach()
endif()