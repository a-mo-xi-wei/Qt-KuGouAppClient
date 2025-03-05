cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#主窗口
add_subdirectory(window/MainWindow)

#自定义窗口模块
add_subdirectory(window/BasicWidget)

#自定义堆栈窗口模块
add_subdirectory(ui/MyStackedWidget)

#自定义流式布局
add_subdirectory(ui/MyFlowLayout)

#自定义块窗口
add_subdirectory(ui/MyBlockWidget)

#音乐单项模块
add_subdirectory(ui/MusicItemWidget)

#音乐表单模块
add_subdirectory(ui/MusicTableWidget)

#遮罩模块
add_subdirectory(ui/SMaskWidget)

#涟漪模块
add_subdirectory(ui/RippleButton)

#交互模块
add_subdirectory(ui/InteractiveButtonBase)

#音量调节模块
add_subdirectory(ui/VolumeToolBtn)

#滑动条模块
add_subdirectory(ui/SliderWidget)

#搜索框模块
add_subdirectory(ui/MySearchLineEdit)

#回到顶部模块
add_subdirectory(ui/UpToolButton)

#文字在前图标在后的按钮模块
add_subdirectory(ui/IconBesideTextToolButton)

#滚动区域模块
add_subdirectory(ui/MyScrollArea)

#菜单模块
add_subdirectory(ui/MyMenu)

#复选框模块
add_subdirectory(ui/MyCheckBox)

#开始模块
add_subdirectory(KuGouStart)

#异步线程模块
add_subdirectory(util/MyAsyncThread)

#日志模块
add_subdirectory(util/MySpdlog)