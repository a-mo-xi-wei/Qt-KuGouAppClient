cmake_minimum_required(VERSION 3.10)

project(MyDialog VERSION 1.0 LANGUAGES CXX)

file(GLOB ABOUTDIALOG_H ${CMAKE_CURRENT_SOURCE_DIR}/AboutDialog/*.h)
file(GLOB ABOUTDIALOG_CPP ${CMAKE_CURRENT_SOURCE_DIR}/AboutDialog/*.cpp)

set(SOURCE_FILE
    ${ABOUTDIALOG_H}
    ${ABOUTDIALOG_CPP}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYDIALOG_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/AboutDialog
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network QtMaterialWidget
        MySpdlog MyButton ElaWidgetTools MyAsyncThread MyHttpClient)
