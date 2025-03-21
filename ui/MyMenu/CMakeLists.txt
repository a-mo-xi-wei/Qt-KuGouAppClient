cmake_minimum_required(VERSION 3.10)

project(MyMenu VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

#file(GLOB SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#file(GLOB SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

#更加地通用
file(GLOB_RECURSE SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    MenuBtn.hpp
    ${SRC_CPP}
    ${SRC_H}
    menu.css
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE
        MYWIDGETMENU_LIBRARY
        BASEMENU_LIBRARY
        GLOPTIONMENU_LIBRARY
        LISTENOPTIONMENU_LIBRARY
        LISTOPTIONMENU_LIBRARY
        SONGOPTIONMENU_LIBRARY
        SORTOPTIONMENU_LIBRARY
        TITLEOPTIONMENU_LIBRARY
        TRAYICONMENU_LIBRARY
)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/ListOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SortOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/SongOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/TitleOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/GLOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/TrayIconMenu)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MySpdlog)
