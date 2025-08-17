cmake_minimum_required(VERSION 3.10)

project(KuGouServer VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED Core Sql WebSockets)

file(GLOB KUGOU_SERVER_NET_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/NetWork/*.cpp)
file(GLOB KUGOU_SERVER_NET_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/NetWork/*.h)

file(GLOB KUGOU_SERVER_KCP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/NetWork/kcp/*.cpp)
file(GLOB KUGOU_SERVER_KCP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/NetWork/kcp/*.h)

set(SOURCE_FILE
        KuGouServer.h
        KuGouServer.cpp
        ${KUGOU_SERVER_NET_HEADERS}
        ${KUGOU_SERVER_NET_SOURCES}
        ${KUGOU_SERVER_KCP_SOURCES}
        ${KUGOU_SERVER_KCP_HEADERS}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/NetWork)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Sql Qt6::WebSockets
        Common MyQsLog JQHttpServer MyDBPool)

# 动态库需处理 DLL 复制（仅 Windows）
if (WIN32)
    add_custom_command(TARGET KuGouServer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ZLIB_DLL}
            $<TARGET_FILE_DIR:KuGouServer>
    )
endif ()