cmake_minimum_required(VERSION 3.10)

project(KuGouAppStart VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

#file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB QRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Res&Qrc/*.qrc)

set(SOURCE_FILE
    KuGouApp.h
    KuGouApp.cpp
    KuGouApp.ui
    main.cpp
    ${QRC_FILE}
    Res&Qrc/Res/titleIcon.rc
)

####       发现音乐       ####
#包含为你推荐模块
add_subdirectory(RecommendForYou)
#包含乐库模块
add_subdirectory(MusicRepository)
#包含频道模块
add_subdirectory(Channel)
#包含视频模块
add_subdirectory(Video)
#包含直播模块
add_subdirectory(Live)
#包含歌单模块
add_subdirectory(SongList)
#包含每日推荐模块
add_subdirectory(DailyRecommend)
####       我的音乐       ####
#包含我的收藏模块
add_subdirectory(MyCollection)
#包含本地与下载模块
add_subdirectory(LocalDownload)
#包含音乐云盘模块
add_subdirectory(MusicCloudDisk)
#包含已购音乐模块
add_subdirectory(PurchasedMusic)
#包含最近播放模块
add_subdirectory(RecentlyPlayed)
#包含默认列表模块
add_subdirectory(AllMusic)
####       标题栏       ####
#包含听书模块
add_subdirectory(ListenBook)
#包含搜索模块
add_subdirectory(Search)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia MainWindow BasicWidget
        AllMusic Channel DailyRecommend ListenBook Live LocalDownload MusicCloudDisk
        MusicRepository MyCollection PurchasedMusic RecentlyPlayed RecommendForYou
        Search SongList StartVideo VolumeToolBtn RefreshMask MaterialSnackbar)
