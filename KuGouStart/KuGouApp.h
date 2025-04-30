#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include "MainWindow.h"
#include "MusicPlayer.h"
//发现音乐
#include "RecommendForYou.h"
#include "MusicRepository.h"
#include "Channel.h"
#include "Video.h"
#include "Live.h"
#include "AiChat.h"
#include "SongList.h"
#include "DailyRecommend.h"
//我的音乐
#include "MyCollection.h"
#include "LocalDownload.h"
#include "MusicCloudDisk.h"
#include "PurchasedMusic.h"
#include "RecentlyPlayed.h"
#include "AllMusic.h"
//标题栏
#include "ListenBook.h"
#include "Search.h"
#include <ui_KuGouApp.h>

class QButtonGroup;
class QSizeGrip;
class QPropertyAnimation;
class RefreshMask;
class QtMaterialSnackbar;

QT_BEGIN_NAMESPACE

namespace Ui {
    class KuGouApp;
}

QT_END_NAMESPACE

class KuGouApp : public MainWindow {
    Q_OBJECT

public:
    explicit KuGouApp(MainWindow *parent = nullptr);

    ~KuGouApp() override;

private:
    void initFontRes();

    void initUi();

    void initStackedWidget();

    template<typename T>
    void initComponent(std::unique_ptr<T>& component, const int& index) {
        component = std::make_unique<T>(ui->stackedWidget);
        // 插入到指定下标位置
        ui->stackedWidget->insertWidget(index, component.get());
    }

    void initTitleWidget();

    void initPlayWidget();

    void initMenu();

    void initCornerWidget();

    int getCurrentIndex(int index);    //通过在旧的vector里面的下标，来找新的下标

    void update_cover_singer_song_HLayout();

    void updateSize();

    void enableButton(const bool& flag);

private:
    void addOrderIndex();

    void subOrderIndex();

    void addSongIndex();

    void subSongIndex();

protected:
    void mousePressEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    bool event(QEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private slots:
    //menu
    void on_recommend_you_toolButton_clicked();

    void on_music_repository_toolButton_clicked();

    void on_channel_toolButton_clicked();

    void on_video_toolButton_clicked();

    void on_live_toolButton_clicked();

    void on_ai_chat_toolButton_clicked();

    void on_song_list_toolButton_clicked();

    void on_daily_recommend_toolButton_clicked();

    void on_my_collection_toolButton_clicked();

    void on_local_download_toolButton_clicked();

    void on_music_cloud_disk_toolButton_clicked();

    void on_purchased_music_toolButton_clicked();

    void on_recently_played_toolButton_clicked();

    void on_all_music_toolButton_clicked();

    //playWidget
    void on_play_or_pause_toolButton_clicked();

    void on_love_toolButton_clicked();

    void on_download_toolButton_clicked();

    void on_comment_toolButton_clicked();

    void on_share_toolButton_clicked();

    void on_more_toolButton_clicked();

    void on_circle_toolButton_clicked();

    void on_pre_toolButton_clicked();

    void on_next_toolButton_clicked();

    void on_stander_pushButton_clicked();

    void on_acoustics_pushButton_clicked();

    void on_erji_toolButton_clicked();

    void on_lyrics_toolButton_clicked();

    void on_song_queue_toolButton_clicked();

private slots:
    void setPlayMusic(int &index);

    void updateProcess();

    void updateSliderRange(const qint64& duration);

    void onPlayMusic(int index);

    void onStartPlay();

    void onAddSongInfo(const SongInfor &info);

    void onSubSongInfo(const SongInfor &info);

    void onKeyPause();

    void onKeyLeft();

    void onKeyRight();

    void onTitleCurrentStackChange(const int& index,const bool& slide);

    void onLeftMenuShow(const bool& flag) const;

    void onTitleMaxScreen();

    //接收重排信号
    void onSyncSongInfoVector(QVector<SongInfor>& vec);

signals:
    void setPlayIndex(const int& index);

    void maxScreen();       //发出最大化信号给LocalDown，让正在播放的高亮条延伸

    void curPlaySongNameChange(const QString& songName);

    void curPlaySingerChange(const QString& singer);

private:
    Ui::KuGouApp *ui;
    MusicPlayer*                        m_player{};
    std::unique_ptr<QButtonGroup>       m_menuBtnGroup{};
    std::unique_ptr<QSizeGrip>          m_sizeGrip{};
    std::unique_ptr<QPropertyAnimation> m_animation{};  //专门用于窗口的缩放动画
    //堆栈窗口
    std::unique_ptr<RecommendForYou>    m_recommendForYou{};
    std::unique_ptr<MusicRepository>    m_musicRepository{};
    std::unique_ptr<Channel>            m_channel{};
    std::unique_ptr<Video>              m_video{};
    std::unique_ptr<Live>               m_live{};
    std::unique_ptr<AiChat>             m_aiChat{};
    std::unique_ptr<SongList>           m_songList{};
    std::unique_ptr<DailyRecommend>     m_dailyRecommend{};
    std::unique_ptr<MyCollection>       m_collection{};
    std::unique_ptr<LocalDownload>      m_localDownload{};
    std::unique_ptr<MusicCloudDisk>     m_musicCloudDisk{};
    std::unique_ptr<PurchasedMusic>     m_purchasedMusic{};
    std::unique_ptr<RecentlyPlayed>     m_recentlyPlayed{};
    std::unique_ptr<AllMusic>           m_allMusic{};
    std::unique_ptr<ListenBook>         m_listenBook{};
    std::unique_ptr<Search>             m_search{};
    //刷新遮罩
    std::unique_ptr<RefreshMask>        m_refreshMask{};
    std::unique_ptr<QtMaterialSnackbar> m_snackbar{};
    //窗口缩放相关
    bool                        m_isTransForming = false; //专门用于在窗口缩放动画播放时，禁用拖动事件
    bool                        m_isSingleCircle = false;
    bool                        m_isMaxScreen = false;
    QMetaObject::Connection     mediaStatusConnection;

    QPoint                      m_pressPos;
    QString                     m_maxBtnStyle;
    QRect                       m_startGeometry; // 获取当前窗口的几何形状(正常状态)
    QRect                       m_endGeometry;
    QRect                       m_normalGeometry;//非最大化状态

    bool                    m_isOrderPlay = false;//专门提供给开始播放按钮
    int                     m_orderIndex = 0;
    QVector<SongInfor>      m_songInfoVector;
    QVector<SongInfor>      m_lastSongInfoVector;//保留上一次排序的结果
    int                     m_songIndex = -1;//播放的歌曲的下标
};
#endif // KUGOUAPP_H
