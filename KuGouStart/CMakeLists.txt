cmake_minimum_required(VERSION 3.10)

project(KuGouAppStart VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

file(GLOB_RECURSE SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_UI ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
file(GLOB QRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Res&Qrc/*.qrc)

set(SOURCE_FILE
    ${SOURCE_H}
    ${SOURCE_CPP}
    ${SOURCE_UI}
    ${QRC_FILE}
    Res&Qrc/Res/titleIcon.rc
)

#[[
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
]]

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllCloudDisk
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllLocal
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllLove
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllPaid
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllRecent
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllSongList
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic/AllWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Channel
        ${CMAKE_CURRENT_SOURCE_DIR}/DailyRecommend
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload/DownloadedWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload/DownloadingWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecentlyPlay
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/Gallery
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/ListenRankWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/ListenTableWidget

        ${CMAKE_CURRENT_SOURCE_DIR}/Live
        ${CMAKE_CURRENT_SOURCE_DIR}/Live/LiveBigLeftWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Live/LiveBlockWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Live/LiveCommonPartWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Live/LiveMusicPartWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Live/PopularWidgets

        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload/LocalSong
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload/DownloadedSong
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload/DownloadedVideo
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload/Downloading

        ${CMAKE_CURRENT_SOURCE_DIR}/MusicCloudDisk
        ${CMAKE_CURRENT_SOURCE_DIR}/MusicCloudDisk/UploadedSong
        ${CMAKE_CURRENT_SOURCE_DIR}/MusicCloudDisk/UploadingSong

        ${CMAKE_CURRENT_SOURCE_DIR}/MusicRepository

        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/CollectVideo
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/DeviceWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/SingerWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/SingleSong
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/SongListWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection/SpecialAlbum

        ${CMAKE_CURRENT_SOURCE_DIR}/PurchasedMusic
        ${CMAKE_CURRENT_SOURCE_DIR}/PurchasedMusic/PaidSingle
        ${CMAKE_CURRENT_SOURCE_DIR}/PurchasedMusic/PurchasedAlbums
        ${CMAKE_CURRENT_SOURCE_DIR}/PurchasedMusic/PurchasedVideos

        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed/RecentlyMVChannel
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed/RecentlySingleSong
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed/RecentlySongChannel
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed/RecentlySongList
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed/RecentlyVideoWidget

        ${CMAKE_CURRENT_SOURCE_DIR}/RecommendForYou
        ${CMAKE_CURRENT_SOURCE_DIR}/RecommendForYou/AdvertiseBoardWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/RecommendForYou/GuessLoveWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Search
        ${CMAKE_CURRENT_SOURCE_DIR}/SongList
        ${CMAKE_CURRENT_SOURCE_DIR}/Video
        ${CMAKE_CURRENT_SOURCE_DIR}/Video/MVWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Video/MVWidget/SliderBoard
        ${CMAKE_CURRENT_SOURCE_DIR}/Video/VideoChannelWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/Video/VideoWidget
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia MySlidingStackedWidget
            UpToolButton BasicWidget MyScrollArea MySearchLineEdit MusicItemWidget
            RippleButton VolumeToolBtn SliderWidget MainWindow MyHttpClient MyBlockWidget
            ZoomButtonLibrary IconBesideTextToolButton MyAsyncThread MyMenu MyFlowLayout
            CheckBox1 MusicTableWidget)
