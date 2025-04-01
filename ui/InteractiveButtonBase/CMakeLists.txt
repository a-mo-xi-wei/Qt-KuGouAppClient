cmake_minimum_required(VERSION 3.10)

project(InteractiveButtonBase VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE_BASE
    InteractiveButtonBase.h
    InteractiveButtonBase.cpp
)

set(SOURCE_FILE_WATER
    WaterZoomButton.h
    WaterZoomButton.cpp
)

set(SOURCE_FILES
    ${SOURCE_FILE_BASE}
    ${SOURCE_FILE_WATER}
)
#定义基类动态库
add_library(InteractiveButton SHARED ${SOURCE_FILES})
target_compile_definitions(InteractiveButton PRIVATE INTERACTIVEBUTTON_LIBRARY)
# 为基类动态库设置输出路径
if(WIN32)
    set_target_properties(InteractiveButton PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # DLL 生成到 bin/
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib   # 导入库 (LIB) 生成到 lib/
    )
else()
    set_target_properties(InteractiveButton PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib   # Linux/macOS 的共享库
    )
endif()

# 指定库的包含目录
target_include_directories(InteractiveButton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(InteractiveButton Qt6::Widgets)
