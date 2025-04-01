cmake_minimum_required(VERSION 3.10)

project(MyCheckBox VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE_ANI
    AniCheckBox.h
    AniCheckBox.cpp
)

set(SOURCE_FILE_CHECK1
    CheckBox1.h
    CheckBox1.cpp
)

set(SOURCE_FILE
    ${SOURCE_FILE_ANI}
    ${SOURCE_FILE_CHECK1}
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

target_compile_definitions(${PROJECT_NAME} PRIVATE MYCHECKBOX_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets MySpdlog)

