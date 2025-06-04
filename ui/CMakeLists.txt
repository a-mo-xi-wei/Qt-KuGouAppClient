cmake_minimum_required(VERSION 3.10)

project(Ui VERSION 1.0 LANGUAGES CXX)

#Material模块
add_subdirectory(QtMaterialWidget)

#ElaWidget模块
add_subdirectory(ElaWidgetTools)

#登录模块
add_subdirectory(LoginInterface)

#主窗口模块
add_subdirectory(MyWindow)

#自定义堆栈窗口模块
add_subdirectory(MySlidingStackedWidget)

#自定义流式布局
add_subdirectory(MyFlowLayout)

#我的遮罩模块
add_subdirectory(MyMaskWidget)

#自定义块窗口
add_subdirectory(MyBlockWidget)

#音乐单项模块
add_subdirectory(MusicItemWidget)

#我的按钮模块
add_subdirectory(MyButton)

#搜索框模块
add_subdirectory(MySearchLineEdit)

#滚动区域模块
add_subdirectory(MyScrollArea)

#菜单模块
add_subdirectory(MyMenu)

#复选框模块
add_subdirectory(MyCheckBox)

#对话框模块
add_subdirectory(MyDialog)
