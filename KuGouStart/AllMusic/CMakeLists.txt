cmake_minimum_required(VERSION 3.10)

project(AllMusic VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE
    AllMusic.cpp
    AllMusic.h
    AllMusic.ui
)

#添加"全部"模块
add_subdirectory(AllWidget)
#添加"喜欢"模块
add_subdirectory(AllLove)
#添加"歌单"模块
add_subdirectory(AllSongList)
#添加"最近"模块
add_subdirectory(AllRecent)
#添加"本地"模块
add_subdirectory(AllLocal)
#添加"已购"模块
add_subdirectory(AllPaid)
#添加"云盘"模块
add_subdirectory(AllCloudDisk)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE ALLMUSIC_LIBRARY)

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

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets MySearchLineEdit
                        MySlidingStackedWidget AllWidget AllLove AllSongList
                        AllRecent AllLocal AllPaid AllCloudDisk MyMenu)

