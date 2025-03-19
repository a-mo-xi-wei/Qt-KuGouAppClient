#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include "MyMenu.h"
#include "SongOptionMenu/SongOptionMenu.h"

#include <QDateTime>
#include <QFrame>

#if defined(MUSICITEMWIDGET_LIBRARY)
#define MUSICITEMWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MUSICITEMWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QPointF;
class QTimer;

class QLabel;
class QToolButton;
class QSpacerItem;

typedef struct SongInformation {
    int index;
    QPixmap cover;
    QString songName;
    QString singer;
    QString duration;
    QString mediaPath;
    QDateTime addTime;
    int playCount;

    bool operator ==(const struct SongInformation &info) const {
        return info.songName == this->songName && info.singer == this->singer && info.duration == this->duration;
    }
} SongInfor;

class MUSICITEMWIDGET_EXPORT MusicItemWidget : public QFrame {
    Q_OBJECT

public:
    explicit MusicItemWidget(SongInfor info, QWidget *parent = nullptr);

    void setIndexText(const int &index) const;

    void setInterval(const int &timeInterval) const; // 设置定时器时间间隔，控制填充速度

    void setFillColor(const QColor &fillcolor); // 设置填充颜色

    void setRadius(const int &radius_); // 设置圆角半径

    void setInformation(const SongInfor &info);

    void setPlayState(const bool &state);

    void getMenuPosition(const QPoint &pos);

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    //smallWidget
    void onPlayToolBtnClicked();

    void onPlayNextToolBtnClicked();

    void onDownloadToolBtnClicked();

    void onCollectToolBtnClicked();

    void onMoreToolBtnClicked();

    //menu (中转)
    void onPlay();

    void onNextPlay();

    void onAddToPlayQueue();

    void onAddToNewSongList();

    void onAddToLove();

    void onAddToCollect();

    void onAddtoPlayList();

    void onDownload();

    void onShare();

    void onComment();

    void onSameSong();

    void onViewSongInfo();

    void onDeleteSong(const int &idx);

    void onOpenInFile();

    void onSearch();

    void onUpLoad();

signals:
    void play();

    void nextPlay();

    void addToPLayQueue();

    void addToNewSongList();

    void addToLove();

    void addToCollect();

    void addToPlayList();

    void download();

    void share();

    void comment();

    void sameSong();

    void viewSongInfo();

    void deleteSong(const int &idx);

    void openInFile();

    void search();

    void upload();

private:
    void initUi();

private:
    //基础控件
    QLabel *m_indexLab{};
    QLabel *m_coverLab{};
    QLabel *m_nameLab{};
    QLabel *m_singerLab{};
    QLabel *m_durationLab{};
    QToolButton *m_playToolBtn{};
    QToolButton *m_playNextToolBtn{};
    QToolButton *m_downloadToolBtn{};
    QToolButton *m_collectToolBtn{};
    QToolButton *m_moreToolBtn{};

    bool m_isPlaying = false;
    //菜单相关
    SongOptionMenu *m_songOptMenu{};
    QPoint m_menuPosition;
    //歌曲信息相关
    int m_index;
    QPixmap m_cover;
    QString m_name;
    QString m_singer;
    QString m_duration;

public:
    SongInfor m_information;

private:
    //涟漪效果相关
    int timeInterval = 10; // 定时器时间间隔，单位：ms
    QTimer *timer{}; // 定时器对象
    QPointF mouse_point; // 记录鼠标进入和离开时的坐标
    int max_radius; // 最大半径
    int radius = 0; // 绘制半径
    int radius_var = 2; // 半径每次改变的值（增大或减小）
    QColor fill_color; // 填充颜色
    int frame_radius = 0; // 绘制路径的圆角半径
};

#endif // MUSICITEMWIDGET_H
