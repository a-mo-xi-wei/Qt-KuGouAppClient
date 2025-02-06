cmake_minimum_required(VERSION 3.10)

project(KuGouApp VERSION 1.0 LANGUAGES CXX)

#主窗口
add_subdirectory(MainWindow)

#自定义窗口模块
add_subdirectory(BasicWidget)

#自定义堆栈窗口模块
add_subdirectory(MyStackedWidget)

#自定义流式布局
add_subdirectory(MyFlowLayout)

#自定义块窗口
add_subdirectory(MyBlockWidget)

#音乐单项模块
add_subdirectory(MusicItemWidget)

#音乐表单模块
add_subdirectory(MusicTableWidget)

#遮罩模块
add_subdirectory(SMaskWidget)

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

#开始模块
add_subdirectory(KuGouStart)
