cmake_minimum_required(VERSION 3.10)

project(LoginInterface VERSION 1.0 LANGUAGES CXX)

file(GLOB SOURCE_SMALL_CPP ${CMAKE_CURRENT_SOURCE_DIR}/smallWidget/*.cpp)
file(GLOB SOURCE_SMALL_H ${CMAKE_CURRENT_SOURCE_DIR}/smallWidget/*.h)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SOURCE_SMALL_H}
    ${SOURCE_SMALL_CPP}
    ${SOURCE_H}
    ${SOURCE_CPP}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE LOGIN_REGISTER_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/smallWidget
)

target_link_libraries(${PROJECT_NAME} Qt6::Network Qt6::Widgets QtMaterialWidget ElaWidgetTools dwmapi)
