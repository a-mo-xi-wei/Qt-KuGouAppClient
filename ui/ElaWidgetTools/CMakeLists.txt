cmake_minimum_required(VERSION 3.10)

project(ElaWidgetTools VERSION 1.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

file(GLOB COMMON_H ${CMAKE_CURRENT_SOURCE_DIR}/ElaCommon/*.h)
file(GLOB COMMON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/ElaCommon/*.cpp)

file(GLOB FLOWLAYOUT_H ${CMAKE_CURRENT_SOURCE_DIR}/ElaFlowLayout/*.h)
file(GLOB FLOWLAYOUT_CPP ${CMAKE_CURRENT_SOURCE_DIR}/ElaFlowLayout/*.cpp)

file(GLOB MESSAGEBUTTON_H ${CMAKE_CURRENT_SOURCE_DIR}/ElaMessageButton/*.h)
file(GLOB MESSAGEBUTTON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/ElaMessageButton/*.cpp)

set(SOURCE_FILE
        ${COMMON_H}
        ${COMMON_CPP}
        ${FLOWLAYOUT_H}
        ${FLOWLAYOUT_CPP}
        ${MESSAGEBUTTON_H}
        ${MESSAGEBUTTON_CPP}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE ELAWIDGETTOOLS_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ElaCommon
        ${CMAKE_CURRENT_SOURCE_DIR}/ElaFlowLayout
        ${CMAKE_CURRENT_SOURCE_DIR}/ElaMessageButton
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Core MySpdlog)
