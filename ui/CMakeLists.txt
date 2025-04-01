cmake_minimum_required(VERSION 3.10)

project(Ui VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Window)

#自定义堆栈窗口模块
add_subdirectory(MySlidingStackedWidget)

#自定义流式布局
add_subdirectory(MyFlowLayout)

#遮罩模块
add_subdirectory(SMaskWidget)

#自定义块窗口
add_subdirectory(MyBlockWidget)

#音乐单项模块
add_subdirectory(MusicItemWidget)

#涟漪模块
add_subdirectory(RippleButton)

#交互模块
add_subdirectory(InteractiveButtonBase)

#音量调节模块
add_subdirectory(VolumeToolBtn)

#滑动条模块
add_subdirectory(SliderWidget)

#搜索框模块
add_subdirectory(MySearchLineEdit)

#回到顶部模块
add_subdirectory(UpToolButton)

#文字在前图标在后的按钮模块
add_subdirectory(IconBesideTextToolButton)

#滚动区域模块
add_subdirectory(MyScrollArea)

#菜单模块
add_subdirectory(MyMenu)

#复选框模块
add_subdirectory(MyCheckBox)

#对话框模块
add_subdirectory(MyDialog)

#Material模块
add_subdirectory(QtMaterialWidget)

#ElaWidget模块
add_subdirectory(ElaWidgetTools)

#遮罩刷新模块
add_subdirectory(RefreshMask)