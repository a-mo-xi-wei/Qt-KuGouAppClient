#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include<memory>
#include<QUrl>
#include<QEasingCurve>

#include"MainWindow.h"
#include"RecommendForYou.h"
#include"LocalDownload.h"
#include"UpToolButton.h"
//#include"MyMenu.h"//直接使用title的Menu
#include"TitleWidget.h"

class QMediaPlayer;
class QAudioOutput;
class QButtonGroup;
class QSizeGrip;
class QPropertyAnimation;
class QScrollBar;

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
    void initUi();

    void initCommendForYouWidget();

    void initLocalDownload();

    void initTitleWidget();

    void initPlayWidget();

    void initMenu();

    void initCornerWidget();

    int getCurrentIndex(int index);    //通过在旧的vector里面的下标，来找新的下标
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

private slots:
    void on_title_return_toolButton_clicked();

    void on_title_refresh_toolButton_clicked();

    void on_title_music_pushButton_clicked();

    void on_title_live_pushButton_clicked();

    void on_title_listenBook_pushButton_clicked();

    void on_title_found_pushButton_clicked();

    void on_recommend_toolButton_clicked();

    void on_local_download_toolButton_clicked();

    void on_play_or_pause_toolButton_clicked();

    void on_menu_toolButton_clicked();

    void on_min_toolButton_clicked();

    void on_max_toolButton_clicked();

    void on_close_toolButton_clicked();

    void on_love_toolButton_clicked();

    void on_download_toolButton_clicked();

    void on_comment_toolButton_clicked();

    void on_share_toolButton_clicked();

    void on_more_toolButton_clicked();

    void on_circle_toolButton_clicked();

    void on_pre_toolButton_clicked();

    void on_next_toolButton_clicked();

public slots:
    void setPlayMusic(int &index);

    void updateProcess();

    void updateSliderRange(const qint64& duration);

    void onPlayMusic(int index);

    void onStartPlay();

    void onAddSongInfo(const SongInfor &info);

    void onUpBtnClicked();

    void onScrollBarValueChanged(const int& value);

    void onUpBtnShowOrNot();

    void onKeyPause();

    void onKeyLeft();

    void onKeyRight();

signals:
    void setPlayIndex(const int& index);
public slots:
    //接收重排信号
    void onSyncSongInfoVector(QVector<SongInfor>& vec);

private:
    Ui::KuGouApp *ui;
    std::unique_ptr<QMediaPlayer>       m_player{};
    std::unique_ptr<QAudioOutput>       m_audioOutput{};
    std::unique_ptr<QButtonGroup>       m_menuBtnGroup{};
    std::unique_ptr<QSizeGrip>          m_sizeGrip{};
    std::unique_ptr<UpToolButton>       m_upBtn{};
    std::unique_ptr<QPropertyAnimation> m_animation{};  //专门用于窗口的缩放动画
    std::unique_ptr<RecommendForYou>    m_recommendForYou{};
    std::unique_ptr<LocalDownload>      m_localDownload{};
    //标题菜单相关
    TitleWidget*                m_title{};
    //窗口缩放相关
    bool                        m_isTransForming = false; //专门用于在窗口缩放动画播放时，禁用拖动事件
    bool                        m_isSingleCircle = false;
    bool                        m_isMaxScreen = false;
    QMetaObject::Connection     mediaStatusConnection;

    bool                        m_isPlaying = false;
    QPoint                      m_pressPos;
    QString                     m_maxBtnStyle;
    QRect                       startGeometry; // 获取当前窗口的几何形状(正常状态)
    QRect                       endGeometry;

    bool                    m_isOrderPlay = false;//专门提供给开始播放按钮
    int                     m_orderIndex = 0;
    QVector<SongInfor>      m_songInfoVector;
    QVector<SongInfor>      m_lastSongInfoVector;//保留上一次排序的结果
    int                     m_songIndex = 0;//播放的歌曲的下标

    QEasingCurve            m_curves = QEasingCurve::OutBounce;
    QScrollBar*             m_vScrollBar{};

    //专门处理回到最顶部按钮
    QTimer*                 m_scrollBarTimer;  // 定时器
    int                     m_scrollValue;  // 存储最新的滚动值
};
#endif // KUGOUAPP_H
