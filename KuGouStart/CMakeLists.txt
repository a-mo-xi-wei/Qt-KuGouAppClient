cmake_minimum_required(VERSION 3.10)

project(KuGouAppStart VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(SOURCE_FILE
    ${SOURCE_CPP}
    ${SOURCE_H}
    Res.qrc
    KuGouApp.ui
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

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia
                        RecommendForYou MusicRepository Channel StartVideo Live
                        SongList DailyRecommend MyCollection LocalDownload
                        MusicCloudDisk PurchasedMusic RecentlyPlayed AllMusic
                        ListenBook Search MyStackedWidget
                        BasicWidget MyScrollArea MySearchLineEdit UpToolButton
                        RippleButton VolumeToolBtn SliderWidget MainWindow )
