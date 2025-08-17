cmake_minimum_required(VERSION 3.10)
project(UiWidgets LANGUAGES CXX)

#DynamicBackground模块
add_subdirectory(DynamicBackground)

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

#歌词模块
add_subdirectory(LyricWidget)

set(UI_SOURCE
        ${DYNAMIC_BACKGROUND_SOURCES}
        ${LOGIN_INTERFACE_SOURCES}
        ${MYWINDOW_SOURCES}
        ${MYSLIDINGSTACKEDWIDGET_SOURCES}
        ${MYFLOWLAYOUT_SOURCES}
        ${MYMASKWIDGET_SOURCES}
        ${MYBLOCKWIDGET_SOURCES}
        ${MUSICITEMWIDGET_SOURCES}
        ${MYBUTTON_SOURCES}
        ${MYSEARCHLINEEDIT_SOURCES}
        ${MYSCROLLAREA_SOURCES}
        ${MYMENU_SOURCES}
        ${MYCHECKBOX_SOURCES}
        ${MYDIALOG_SOURCES}
        ${LYRICWIDGET_SOURCES}
)

add_library(UiWidgets SHARED ${UI_SOURCE})

target_compile_definitions(${PROJECT_NAME} PRIVATE UIWIDGETS_LIBRARY)

target_include_directories(UiWidgets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/DynamicBackground
        ${CMAKE_CURRENT_SOURCE_DIR}/LoginInterface
        ${CMAKE_CURRENT_SOURCE_DIR}/LoginInterface/smallWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow
        ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow/BasicWindow
        ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow/MyTrayIcon
        ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow/MainWindow
        ${CMAKE_CURRENT_SOURCE_DIR}/MyWindow/MainWindow/WaterDrop
        ${CMAKE_CURRENT_SOURCE_DIR}/MySlidingStackedWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyFlowLayout
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMaskWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMaskWidget/RefreshMask
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMaskWidget/SMaskWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyBlockWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MusicItemWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton/IconBesideTextToolButton
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton/InteractiveButtonBase
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton/RippleButton
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton/UpToolButton
        ${CMAKE_CURRENT_SOURCE_DIR}/MyButton/VolumeToolBtn
        ${CMAKE_CURRENT_SOURCE_DIR}/MySearchLineEdit
        ${CMAKE_CURRENT_SOURCE_DIR}/MyScrollArea
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu/ListenOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu/ListOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu/SortOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu/SongOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMenu/TitleOptionMenu
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCheckBox
        ${CMAKE_CURRENT_SOURCE_DIR}/MyDialog
        ${CMAKE_CURRENT_SOURCE_DIR}/MyDialog/AboutDialog
        ${CMAKE_CURRENT_SOURCE_DIR}/MyDialog/SpeedDialog
        ${CMAKE_CURRENT_SOURCE_DIR}/LyricWidget
)

target_link_libraries(UiWidgets PUBLIC Qt::Widgets Qt::Core5Compat Qt::Network Qt::Core
        QtMaterialWidget ElaWidgetTools dwmapi MyHttpClient MyAsyncThread SApp MySpdlog)