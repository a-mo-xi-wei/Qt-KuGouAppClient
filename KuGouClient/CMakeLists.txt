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

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt6::Widgets Qt6::Multimedia MyWindow MyPlayer
        AllMusic Channel DailyRecommend ListenBook Live AiChat LocalDownload MusicCloudDisk
        MusicRepository MyCollection PurchasedMusic RecentlyPlayed RecommendForYou
        Search SongList StartVideo SearchResultWidget
)
