cmake_minimum_required(VERSION 3.10)

project(MyMaskWidget VERSION 1.0 LANGUAGES CXX)

file(GLOB REFRESH_H ${CMAKE_CURRENT_SOURCE_DIR}/RefreshMask/*.h)
file(GLOB REFRESH_CPP ${CMAKE_CURRENT_SOURCE_DIR}/RefreshMask/*.cpp)

file(GLOB SMASKWIDGET_H ${CMAKE_CURRENT_SOURCE_DIR}/SMaskWidget/*.h)
file(GLOB SMASKWIDGET_CPP ${CMAKE_CURRENT_SOURCE_DIR}/SMaskWidget/*.cpp)

set(SOURCE_FILE
        ${REFRESH_H}
        ${REFRESH_CPP}
        ${SMASKWIDGET_H}
        ${SMASKWIDGET_CPP}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYMASKWIDGET_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/RefreshMask
        ${CMAKE_CURRENT_SOURCE_DIR}/SMaskWidget
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Core MySpdlog QtMaterialWidget)
