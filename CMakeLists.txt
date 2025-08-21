cmake_minimum_required(VERSION 3.10)

include(cmake/common.cmake)

project(KuGouApp VERSION 1.0 LANGUAGES CXX
        HOMEPAGE_URL "https://gitee.com/a-mo-xi-wei/KuGouApp"
        DESCRIPTION "Qt KuGou App")

find_package(Qt6 REQUIRED COMPONENTS Widgets StateMachine Core Network Multimedia
        Core5Compat Sql Concurrent Gui)

file(GLOB QRC_FILE Res_Qrc/*.qrc)

if (WIN32)
    # 创建专用的资源文件目标，仅包含必要路径
    add_library(app_resources OBJECT Res_Qrc/Res/titleIcon.rc)

    # 只添加资源文件需要的路径（示例，根据实际需要调整）
    target_include_directories(app_resources PRIVATE
            Res_Qrc/Res
    )
endif ()

set(SOURCE_FILE
        AppController.h
        AppController.cpp
        main.cpp
        ${QRC_FILE}
        #${CMAKE_CURRENT_SOURCE_DIR}/Res_Qrc/Res/titleIcon.rc
        # 链接专用资源对象
        $<TARGET_OBJECTS:app_resources>
)

#ui目录
add_subdirectory(ui)

#util目录
add_subdirectory(util)

#客户端模块
add_subdirectory(KuGouClient)

#服务器模块
add_subdirectory(KuGouServer)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/KuGouClient
        ${CMAKE_CURRENT_SOURCE_DIR}/KuGouServer
)

target_link_libraries(${PROJECT_NAME} PRIVATE KuGouClient KuGouServer UiWidgets MySpdlog)

include(cmake/build_info.cmake)