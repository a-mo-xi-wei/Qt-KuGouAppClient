cmake_minimum_required(VERSION 3.10)

project(LocalDownload VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

#更加地通用
file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SRC_UI ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

set(SOURCE_FILE
    ${SRC_CPP}
    ${SRC_H}
    ${SRC_UI}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE LOCALDOWNLOAD_LIBRARY)

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

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalSong
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadedSong
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadedVideo
        ${CMAKE_CURRENT_SOURCE_DIR}/Downloading)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia MyHttpClient
        MusicItemWidget MySearchLineEdit UpToolButton MyScrollArea MySlidingStackedWidget)
