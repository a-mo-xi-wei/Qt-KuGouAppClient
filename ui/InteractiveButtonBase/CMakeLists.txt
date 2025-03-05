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
#定义基类动态库
add_library(ButtonBaseLibrary SHARED ${SOURCE_FILE_BASE})
target_compile_definitions(ButtonBaseLibrary PRIVATE BUTTONBASE_LIBRARY)
# 为基类动态库设置输出路径
if(WIN32)
    set_target_properties(ButtonBaseLibrary PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"   # DLL 生成到 bin/
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"   # 导入库 (LIB) 生成到 lib/
    )
else()
    set_target_properties(ButtonBaseLibrary PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"   # Linux/macOS 的共享库
    )
endif()

# 指定库的包含目录
target_include_directories(ButtonBaseLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ButtonBaseLibrary Qt6::Widgets)

set(SOURCE_FILE_WATER
        WaterZoomButton.h
        WaterZoomButton.cpp
)
#定义派生类动态库（继承基类）
add_library(ZoomButtonLibrary SHARED ${SOURCE_FILE_WATER})

# 同样为派生类动态库设置路径
if(WIN32)
    set_target_properties(ZoomButtonLibrary PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )
else()
    set_target_properties(ZoomButtonLibrary PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )
endif()

#指定库的包含目录
target_include_directories(ZoomButtonLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ZoomButtonLibrary PUBLIC ButtonBaseLibrary)
