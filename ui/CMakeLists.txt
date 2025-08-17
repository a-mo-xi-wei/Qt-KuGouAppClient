cmake_minimum_required(VERSION 3.10)

project(Ui VERSION 1.0 LANGUAGES CXX)

#Material模块
add_subdirectory(QtMaterialWidget)

#ElaWidget模块
add_subdirectory(ElaWidgetTools)

#UiWidgets模块
add_subdirectory(UiWidgets)