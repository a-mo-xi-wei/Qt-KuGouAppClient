cmake_minimum_required(VERSION 3.10)

project(QtMaterialWidget VERSION 1.0 LANGUAGES CXX)

# 收集各个模块的源文件
file(GLOB MATERIAL_COMMON_H ${CMAKE_CURRENT_SOURCE_DIR}/MaterialCommon/*.h)
file(GLOB MATERIAL_COMMON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/MaterialCommon/*.cpp)

file(GLOB QTMATERIAL_DIALOG_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialDialog/*.h)
file(GLOB QTMATERIAL_DIALOG_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialDialog/*.cpp)

file(GLOB QTMATERIAL_PROGRESS_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialCircularProgress/*.h)
file(GLOB QTMATERIAL_PROGRESS_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialCircularProgress/*.cpp)

file(GLOB QTMATERIAL_SNACKBAR_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSnackbar/*.h)
file(GLOB QTMATERIAL_SNACKBAR_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSnackbar/*.cpp)

file(GLOB QTMATERIAL_TEXTFIELD_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialTextField/*.h)
file(GLOB QTMATERIAL_TEXTFIELD_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialTextField/*.cpp)

file(GLOB QTMATERIAL_BUTTON_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialButton/*.h)
file(GLOB QTMATERIAL_BUTTON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialButton/*.cpp)

file(GLOB QTMATERIAL_Slider_H ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSlider/*.h)
file(GLOB QTMATERIAL_Slider_CPP ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSlider/*.cpp)

# 合并所有源文件
set(SOURCE_FILE
        ${MATERIAL_COMMON_H}
        ${MATERIAL_COMMON_CPP}
        ${QTMATERIAL_DIALOG_H}
        ${QTMATERIAL_DIALOG_CPP}
        ${QTMATERIAL_PROGRESS_H}
        ${QTMATERIAL_PROGRESS_CPP}
        ${QTMATERIAL_SNACKBAR_H}
        ${QTMATERIAL_SNACKBAR_CPP}
        ${QTMATERIAL_TEXTFIELD_H}
        ${QTMATERIAL_TEXTFIELD_CPP}
        ${QTMATERIAL_BUTTON_H}
        ${QTMATERIAL_BUTTON_CPP}
        ${QTMATERIAL_Slider_H}
        ${QTMATERIAL_Slider_CPP}
)

# 创建共享库
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILE})

# 设置库属性
target_compile_definitions(${PROJECT_NAME} PRIVATE QTMATERIALWIDGET_LIBRARY)

# 配置包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/MaterialCommon
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialDialog
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialCircularProgress
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSnackbar
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialTextField
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialButton
        ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialSlider
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Widgets
        Qt6::StateMachine
)