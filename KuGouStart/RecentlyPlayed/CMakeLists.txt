cmake_minimum_required(VERSION 3.10)

project(RecentlyPlayed VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE
    RecentlyPlayed.cpp
    RecentlyPlayed.h
    RecentlyPlayed.ui
)

#包含单曲模块
add_subdirectory(RecentlySingleSong)
#包含歌单模块
add_subdirectory(RecentlySongList)
#包含视频模块
add_subdirectory(RecentlyVideoWidget)
#包含音乐频道模块
add_subdirectory(RecentlySongChannel)
#包含MV频道模块
add_subdirectory(RecentlyMVChannel)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE RECENTLYPLAYED_LIBRARY)

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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MyStackedWidget MyCheckBox
                        RecentlySingleSong RecentlySongList RecentlyVideoWidget
                        RecentlySongChannel RecentlyMVChannel)
