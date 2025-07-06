/**
 * @file KuGouApp.h
 * @brief 酷狗音乐应用主窗口的头文件，定义 KuGouApp 类
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef KUGOUCLIENT_H
#define KUGOUCLIENT_H

#include "MainWindow.h"
#include "VideoPlayer/VideoPlayer.h"

// 发现音乐
#include "RecommendForYou.h"
#include "MusicRepository.h"
#include "Channel.h"
#include "Video.h"
#include "Live.h"
#include "AiChat.h"
#include "SongList.h"
#include "DailyRecommend.h"
// 我的音乐
#include "MyCollection.h"
#include "LocalDownload.h"
#include "MusicCloudDisk.h"
#include "PurchasedMusic.h"
#include "RecentlyPlayed.h"
#include "AllMusic.h"
// 标题栏
#include "ListenBook.h"
#include "Search.h"
#include "SpeedDialogState.h"

/**
 * @class QButtonGroup
 * @brief Qt 按钮组类，用于管理互斥按钮
 */
class QButtonGroup;

/**
 * @class QSizeGrip
 * @brief Qt 大小调整控件类，用于调整窗口大小
 */
class QSizeGrip;

/**
 * @class QPropertyAnimation
 * @brief Qt 属性动画类，用于窗口缩放动画
 */
class QPropertyAnimation;

/**
 * @class RefreshMask
 * @brief 自定义刷新遮罩类，用于显示加载状态
 */
class RefreshMask;

/**
 * @class QtMaterialSnackbar
 * @brief 材质设计风格的消息提示条类
 */
class QtMaterialSnackbar;

/**
 * @namespace Ui
 * @brief UI 命名空间，包含酷狗应用界面的 UI 类
 */
QT_BEGIN_NAMESPACE
namespace Ui {
    class KuGouClient;
}
QT_END_NAMESPACE

/**
 * @class KuGouClient
 * @brief 酷狗音乐应用主窗口类，管理音乐播放和界面切换
 */
class KuGouClient : public MainWindow, public VideoPlayer::EventHandle {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针，默认为 nullptr
     */
    explicit KuGouClient(MainWindow *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源和日志
     */
    ~KuGouClient() override;

private:
    /**
     * @brief 初始化播放器
     * @note 设置播放器属性和信号连接
     */
    void initPlayer();

    /**
     * @brief 初始化字体资源
     * @note 加载应用程序所需的字体
     */
    void initFontRes();

    /**
     * @brief 初始化界面
     * @note 设置窗口属性、标题栏、堆栈窗口和播放控件
     */
    void initUi();

    /**
     * @brief 初始化堆栈窗口
     * @note 初始化所有界面组件并添加到堆栈
     */
    void initStackedWidget();

    /**
     * @brief 初始化搜索结果界面
     */
    void initSearchResultWidget();

    /**
     * @brief 初始化搜索结果音乐项
     * @param item 音乐项
     */
    void initSearchResultMusicItem(MusicItemWidget *item);

    /**
     * @brief 异步加载搜索结果里面的封面图片
     * @param item 音乐项
     * @param imageUrl 封面图片的网络路径
     */
    void loadCoverAsync(MusicItemWidget *item, const QString &imageUrl);

    /**
     * @brief 模板函数，初始化堆栈窗口组件
     * @tparam T 组件类型
     * @param component 组件智能指针
     * @param index 插入索引
     * @note 将组件插入到堆栈窗口的指定位置
     */
    template<typename T>
    void initComponent(std::unique_ptr<T>& component, const int& index);

    /**
     * @brief 初始化标题栏
     * @note 设置标题栏信号和交互
     */
    void initTitleWidget();

    /**
     * @brief 初始化播放控件
     * @note 设置播放按钮、进度条和快捷键
     */
    void initPlayWidget();

    /**
     * @brief 初始化菜单
     * @note 设置菜单按钮和互斥行为
     */
    void initMenu();

    /**
     * @brief 初始化角标控件
     * @note 设置窗口大小调整控件
     */
    void initCornerWidget();

    /**
     * @brief 更新窗口大小
     * @note 触发调整大小事件
     */
    void updateSize();

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     * @note 控制菜单按钮和标题栏的交互
     */
    void enableButton(const bool& flag);

protected:
    /**
     * @brief 鼠标按下事件
     * @param ev 鼠标事件
     * @note 处理窗口拖动
     */
    void mousePressEvent(QMouseEvent *ev) override;

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     * @note 处理窗口拖动和最大化恢复
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制窗口背景
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 更新角标位置和控件大小
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 事件处理
     * @param event 事件
     * @return 是否处理事件
     * @note 处理鼠标移动事件
     */
    bool event(QEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监控对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理进度条和封面标签的事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 显示事件
     * @param event 显示事件
     * @note 更新窗口大小
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 关闭事件
     * @param event 关闭事件
     * @note 处理窗口关闭
     */
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 菜单按钮槽函数
    /**
     * @brief 推荐按钮点击槽函数
     * @note 切换到推荐界面
     */
    void on_recommend_you_toolButton_clicked();

    /**
     * @brief 音乐库按钮点击槽函数
     * @note 切换到音乐库界面
     */
    void on_music_repository_toolButton_clicked();

    /**
     * @brief 频道按钮点击槽函数
     * @note 切换到频道界面
     */
    void on_channel_toolButton_clicked();

    /**
     * @brief 视频按钮点击槽函数
     * @note 切换到视频界面
     */
    void on_video_toolButton_clicked();

    /**
     * @brief 直播按钮点击槽函数
     * @note 切换到直播界面
     */
    void on_live_toolButton_clicked();

    /**
     * @brief AI 聊天按钮点击槽函数
     * @note 切换到 AI 聊天界面
     */
    void on_ai_chat_toolButton_clicked();

    /**
     * @brief 歌单按钮点击槽函数
     * @note 切换到歌单界面
     */
    void on_song_list_toolButton_clicked();

    /**
     * @brief 每日推荐按钮点击槽函数
     * @note 切换到每日推荐界面
     */
    void on_daily_recommend_toolButton_clicked();

    /**
     * @brief 我的收藏按钮点击槽函数
     * @note 切换到我的收藏界面
     */
    void on_my_collection_toolButton_clicked();

    /**
     * @brief 本地下载按钮点击槽函数
     * @note 切换到本地下载界面
     */
    void on_local_download_toolButton_clicked();

    /**
     * @brief 音乐云盘按钮点击槽函数
     * @note 切换到音乐云盘界面
     */
    void on_music_cloud_disk_toolButton_clicked();

    /**
     * @brief 已购音乐按钮点击槽函数
     * @note 切换到已购音乐界面
     */
    void on_purchased_music_toolButton_clicked();

    /**
     * @brief 最近播放按钮点击槽函数
     * @note 切换到最近播放界面
     */
    void on_recently_played_toolButton_clicked();

    /**
     * @brief 全部音乐按钮点击槽函数
     * @note 切换到全部音乐界面
     */
    void on_all_music_toolButton_clicked();

    /**
    * @brief 处理suggestBox选中项槽函数
     * @note 切换搜索结果界面
     */
    void handleSuggestBoxSuggestionClicked(const QString &suggestText, const QVariantMap &suggestData);

    // 播放控件槽函数
    /**
     * @brief 播放/暂停按钮点击槽函数
     * @note 切换播放或暂停状态
     */
    void on_play_or_pause_toolButton_clicked();

    /**
     * @brief 喜欢按钮点击槽函数
     * @note 添加到收藏
     */
    void on_love_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     * @note 下载当前音乐
     */
    void on_download_toolButton_clicked();

    /**
     * @brief 评论按钮点击槽函数
     * @note 打开评论界面
     */
    void on_comment_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 分享当前音乐
     */
    void on_share_toolButton_clicked();

    /**
     * @brief 更多按钮点击槽函数
     * @note 打开更多选项
     */
    void on_more_toolButton_clicked();

    /**
     * @brief 循环播放按钮点击槽函数
     * @note 切换循环播放模式
     */
    void on_circle_toolButton_clicked();

    /**
     * @brief 上一首按钮点击槽函数
     * @note 播放上一首音乐
     */
    void on_pre_toolButton_clicked();

    /**
     * @brief 下一首按钮点击槽函数
     * @note 播放下一首音乐
     */
    void on_next_toolButton_clicked();

    /**
     * @brief 速度选择按钮点击槽函数
     * @note 打开速度选择界面
     */
    void on_speed_pushButton_clicked();

    /**
     * @brief 音质选择按钮点击槽函数
     * @note 打开音质选择界面
     */
    void on_stander_pushButton_clicked();

    /**
     * @brief 音效按钮点击槽函数
     * @note 打开音效设置界面
     */
    void on_acoustics_pushButton_clicked();

    /**
     * @brief 一起听按钮点击槽函数
     * @note 邀请好友一起听
     */
    void on_erji_toolButton_clicked();

    /**
     * @brief 桌面歌词按钮点击槽函数
     * @note 打开桌面歌词
     */
    void on_lyrics_toolButton_clicked();

    /**
     * @brief 播放队列按钮点击槽函数
     * @note 打开播放队列
     */
    void on_song_queue_toolButton_clicked();

    /**
     * @brief 更新播放进度
     * @note 根据进度条调整播放位置
     */
    void updateProcess();

    /**
     * @brief 更新进度条范围
     * @param duration 总时长（毫秒）
     * @note 设置进度条最大值和时长标签
     */
    void updateSliderRange(const qint64& duration);

    /**
     * @brief 空格键暂停/播放槽函数
     * @note 处理空格键快捷键
     */
    void onKeyPause();

    /**
     * @brief 左箭头快退槽函数
     * @note 处理左箭头快捷键
     */
    void onKeyLeft();

    /**
     * @brief 右箭头快进槽函数
     * @note 处理右箭头快捷键
     */
    void onKeyRight();

    /**
     * @brief 标题栏堆栈窗口切换槽函数
     * @param index 目标索引
     * @param slide 是否使用滑动动画
     * @note 切换堆栈窗口并更新按钮状态
     */
    void onTitleCurrentStackChange(const int& index, const bool& slide);

    /**
     * @brief 左侧菜单显示槽函数
     * @param flag 是否显示
     * @note 控制菜单滚动区域的显示
     */
    void onLeftMenuShow(const bool& flag) const;

    /**
     * @brief 最大化窗口槽函数
     * @note 切换最大化和正常窗口状态
     */
    void onTitleMaxScreen();

public slots:
    /**
     * @brief 播放本地音乐槽函数
     * @param localPath 本地音乐路径
     * @note 播放指定的本地音乐文件
     */
    void onPlayLocalMusic(const QString& localPath);

signals:
    /**
     * @brief 最大化信号
     * @note 通知本地下载界面调整高亮条
     */
    void maxScreen();

    /**
     * @brief 当前播放歌曲名称改变信号
     * @param songName 歌曲名称
     * @note 通知歌曲名称更新
     */
    void curPlaySongNameChange(const QString& songName);

    /**
     * @brief 当前播放歌手改变信号
     * @param singer 歌手名称
     * @note 通知歌手名称更新
     */
    void curPlaySingerChange(const QString& singer);

protected:
    /**
     * @brief 视频文件打开失败回调
     * @param code 错误码
     * @note 输出错误信息
     */
    void onOpenVideoFileFailed(const int &code) override {
        qWarning() << "Open video file failed with code: " << code;
    }

    /**
     * @brief SDL 打开失败回调
     * @param code 错误码
     * @note 输出错误信息
     */
    void onOpenSdlFailed(const int &code) override {
        qWarning() << "Open SDL failed with code: " << code;
    }

    /**
     * @brief 总时长改变回调
     * @param uSec 总时长（微秒）
     * @note 输出时长信息
     */
    void onTotalTimeChanged(const int64_t &uSec) override {
        qDebug() << "Total time: " << uSec / 1000000 << " seconds";
    }

    /**
     * @brief 播放器状态改变回调
     * @param state 播放器状态
     * @param hasVideo 是否有视频
     * @param hasAudio 是否有音频
     * @note 输出状态信息
     */
    void onPlayerStateChanged(const VideoPlayer::State &state, const bool &hasVideo, const bool &hasAudio) override {
        qDebug() << "Player state changed to: " << state << " (Has audio: " << hasAudio << ")";
    }

    /**
     * @brief 显示视频帧回调
     * @param videoFrame 视频帧指针
     * @note 未实现，仅支持音频播放
     */
    void onDisplayVideo(VideoRawFramePtr videoFrame) override {
        // 不播放视频，无需实现
    }

private:
    Ui::KuGouClient *ui;                                     ///< UI 界面指针
    VideoPlayer* m_player;                                   ///< 音频播放器实例
    std::unique_ptr<QButtonGroup>       m_menuBtnGroup;      ///< 菜单按钮组
    std::unique_ptr<QSizeGrip>          m_sizeGrip;          ///< 窗口大小调整控件
    std::unique_ptr<QPropertyAnimation> m_animation;         ///< 窗口缩放动画
    std::unique_ptr<RefreshMask>        m_refreshMask;       ///< 刷新遮罩
    std::unique_ptr<QtMaterialSnackbar> m_snackbar;          ///< 消息提示条
    SpeedDialogState                    m_speedDialogState;  ///< 状态管理对象
    // 堆栈窗口组件
    std::unique_ptr<RecommendForYou>    m_recommendForYou; ///< 推荐界面
    std::unique_ptr<MusicRepository>    m_musicRepository; ///< 音乐库界面
    std::unique_ptr<Channel>            m_channel;         ///< 频道界面
    std::unique_ptr<Video>              m_video;           ///< 视频界面
    std::unique_ptr<Live>               m_live;            ///< 直播界面
    std::unique_ptr<AiChat>             m_aiChat;          ///< AI 聊天界面
    std::unique_ptr<SongList>           m_songList;        ///< 歌单界面
    std::unique_ptr<DailyRecommend>     m_dailyRecommend;  ///< 每日推荐界面
    std::unique_ptr<MyCollection>       m_collection;      ///< 我的收藏界面
    std::unique_ptr<LocalDownload>      m_localDownload;   ///< 本地下载界面
    std::unique_ptr<MusicCloudDisk>     m_musicCloudDisk;  ///< 音乐云盘界面
    std::unique_ptr<PurchasedMusic>     m_purchasedMusic;  ///< 已购音乐界面
    std::unique_ptr<RecentlyPlayed>     m_recentlyPlayed;  ///< 最近播放界面
    std::unique_ptr<AllMusic>           m_allMusic;        ///< 全部音乐界面
    std::unique_ptr<ListenBook>         m_listenBook;      ///< 听书界面
    std::unique_ptr<Search>             m_search;          ///< 搜索界面
    std::unique_ptr<QWidget>            m_searchResultWidget;    ///< 搜索结果界面
    QVector<MusicItemWidget *>          m_searchMusicItemVector; ///< 音乐项容器
    // 窗口缩放相关
    bool m_isTransForming = false;                      ///< 是否正在执行缩放动画
    bool m_isSingleCircle = false;                      ///< 是否单曲循环
    bool m_isMaxScreen = false;                         ///< 是否最大化
    QMetaObject::Connection mediaStatusConnection;      ///< 播放结束信号连接
    QPoint m_pressPos;                                  ///< 鼠标按下位置
    QString m_maxBtnStyle;                              ///< 最大化按钮样式
    QRect m_startGeometry;                              ///< 动画起始几何形状
    QRect m_endGeometry;                                ///< 动画结束几何形状
    QRect m_normalGeometry;                             ///< 正常窗口几何形状
    QString m_musicTitle;                               ///< 当前歌曲标题
    QString m_musicArtist;                              ///< 当前歌曲艺术家
    // 服务器交互
    CLibhttp                             m_libHttp;                 ///< HTTP 请求库
};

#endif // KUGOUCLIENT_H
