cmake_minimum_required(VERSION 3.10)

project(MyCheckBox VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCE_FILE_ANI
    AniCheckBox.h
    AniCheckBox.cpp
)

add_library(AniCheckBox SHARED ${SOURCE_FILE_ANI})
target_compile_definitions(AniCheckBox PRIVATE ANICHECKBOX_LIBRARY)

if(WIN32)
    # Windows: DLL → bin/, 导入库 → lib/
    set_target_properties(AniCheckBox PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ARCHIVE_OUTPUT_DIRECTORY {CMAKE_BINARY_DIR}/lib
    )
else()
    # Linux/macOS: 共享库 → lib/
    set_target_properties(AniCheckBox PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()
# 指定库的包含目录
target_include_directories(AniCheckBox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AniCheckBox Qt6::Widgets MySpdlog)

set(SOURCE_FILE_CHECK1
    CheckBox1.h
    CheckBox1.cpp
)
add_library(CheckBox1 SHARED ${SOURCE_FILE_CHECK1})
target_compile_definitions(CheckBox1 PRIVATE CHECKBOX1_LIBRARY)

if(WIN32)
    # Windows: DLL → bin/, 导入库 → lib/
    set_target_properties(CheckBox1 PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ARCHIVE_OUTPUT_DIRECTORY {CMAKE_BINARY_DIR}/lib
    )
else()
    # Linux/macOS: 共享库 → lib/
    set_target_properties(CheckBox1 PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()
target_include_directories(CheckBox1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CheckBox1 PUBLIC AniCheckBox)
