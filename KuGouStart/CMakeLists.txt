cmake_minimum_required(VERSION 3.10)

project(KuGouAppStart VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools Multimedia)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SOURCE_CPP}
    ${SOURCE_H}
    "Res.qrc"
    "KuGouApp.ui"
)
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../BasicWidget
    ${PROJECT_SOURCE_DIR}/../RippleButton
    ${PROJECT_SOURCE_DIR}/../VolumeToolBtn
    ${PROJECT_SOURCE_DIR}/../SliderWidget
    ${PROJECT_SOURCE_DIR}/../MyScrollArea
    ${PROJECT_SOURCE_DIR}/../MySearchLineEdit
    ${PROJECT_SOURCE_DIR}/../UpToolButton
    ${PROJECT_SOURCE_DIR}/../LocalDownload
    ${PROJECT_SOURCE_DIR}/../MainWindow
    ${PROJECT_SOURCE_DIR}/../MyStackedWidget)

#包含为你推荐模块
add_subdirectory(RecommendForYou)
#包含本地与下载模块
add_subdirectory(LocalDownload)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::UiTools Qt6::Multimedia
                        RecommendForYou LocalDownload BasicWidget MyScrollArea
                        MySearchLineEdit UpToolButton RippleButton VolumeToolBtn
                        SliderWidget MainWindow MyStackedWidget)
