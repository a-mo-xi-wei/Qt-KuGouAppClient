cmake_minimum_required(VERSION 3.10)

project(KuGouClient VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILE
        KuGouClient.h
        KuGouClient.cpp
        KuGouClient.ui
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
#包含Ai对话模块
add_subdirectory(AiChat)
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
###        网络歌曲     ####
add_subdirectory(SearchResultWidget)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE} ${LINK_OBJS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/AiChat
        ${CMAKE_CURRENT_SOURCE_DIR}/AllMusic
        ${CMAKE_CURRENT_SOURCE_DIR}/Channel
        ${CMAKE_CURRENT_SOURCE_DIR}/DailyRecommend
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/Gallery
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/ListenRankWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend/ListenTableWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload/DownloadedWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenMyDownload/DownloadingWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecentlyPlay
        ${CMAKE_CURRENT_SOURCE_DIR}/ListenBook/ListenRecommend
        ${CMAKE_CURRENT_SOURCE_DIR}/Live
        ${CMAKE_CURRENT_SOURCE_DIR}/LocalDownload
        ${CMAKE_CURRENT_SOURCE_DIR}/MusicCloudDisk
        ${CMAKE_CURRENT_SOURCE_DIR}/MusicRepository
        ${CMAKE_CURRENT_SOURCE_DIR}/MyCollection
        ${CMAKE_CURRENT_SOURCE_DIR}/PurchasedMusic
        ${CMAKE_CURRENT_SOURCE_DIR}/RecentlyPlayed
        ${CMAKE_CURRENT_SOURCE_DIR}/RecommendForYou
        ${CMAKE_CURRENT_SOURCE_DIR}/Search
        ${CMAKE_CURRENT_SOURCE_DIR}/SearchResultWidget
        ${CMAKE_CURRENT_SOURCE_DIR}/SongList
        ${CMAKE_CURRENT_SOURCE_DIR}/Video
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Multimedia UiWidgets MyPlayer)
