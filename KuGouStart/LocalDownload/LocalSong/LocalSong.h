//
// Created by WeiWang on 25-1-27.
//

#ifndef LOCALSONG_H
#define LOCALSONG_H

#include "MusicItemWidget.h"
#include "SortOptionMenu/SortOptionMenu.h"
#include "libhttp.h"

#include <QQueue>
#include <QWidget>

class QMediaPlayer;
class QScrollBar;

QT_BEGIN_NAMESPACE

namespace Ui {
    class LocalSong;
}

QT_END_NAMESPACE

class LocalSong : public QWidget {
    Q_OBJECT

public:
    explicit LocalSong(QWidget *parent = nullptr);

    ~LocalSong() override;

private:
    void initUi();

    void getMetaData();

    void loadNextSong();

    void getMenuPosition(const QPoint &pos);

    void MySort(std::function<bool(const MusicItemWidget *, const MusicItemWidget *)> comparator);

    void updateCurPlayIndex();

    void initMusicItem(MusicItemWidget *item);

    void fetchAndSyncServerSongList();

private slots:
    //ui相关
    void on_local_all_play_toolButton_clicked();

    void on_local_add_toolButton_clicked();

    void on_local_sort_toolButton_clicked();

public slots:
    //其他
    void setPlayIndex(const int &index);

    void onMaxScreenHandle(); //最大化的时候延伸高亮部分

public slots:
    //排序相关
    void onDefaultSort();

    void onAddTimeSort(const bool &down);

    void onSongNameSort(const bool &down);

    void onSingerSort(const bool &down);

    void onDurationSort(const bool &down);

    void onPlayCountSort(const bool &down);

    void onRandomSort();

public slots:
    //Item menu相关
    void onItemNextPlay();

    void onItemAddToPlayQueue();

    void onItemAddToNewSongList();

    void onItemAddToLove();

    void onItemAddToCollect();

    void onItemAddToPlayList();

    void onItemDownload();

    void onItemShare();

    void onItemComment();

    void onItemSameSong();

    void onItemViewSongInfo();

    void onItemDeleteSong(const int &idx);

    void onItemOpenInFile();

    void onItemSearch();

    void onItemUpLoad();

signals:
    void playMusic(const int &index);

    void startPlay();

    void addSongInfo(const SongInfor &info);

    void subSongInfo(const SongInfor &info);

    //同步(更新)Vec
    void syncSongInfo(QVector<SongInfor> &vec);

    //更新数量标签
    void updateCountLabel(const int &count);

protected:
    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::LocalSong *ui;
    std::unique_ptr<QMediaPlayer> m_player;
    QVector<SongInfor> m_locationMusicVector;
    QVector<SongInfor> m_lastLocationMusicVector; //方便求得之前的下标
    QVector<MusicItemWidget *> m_MusicItemVector; //存放item，方便排序

    QAction *m_searchAction; //专门为了设置图片
    QString m_mediaPath;
    QQueue<QString> m_songQueue;

    //固定颜色专用
    int m_curPlayIndex = -1;
    int m_setPlayIndex = -1;

    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    bool m_isSorting = false;
    QPoint m_menuPosition;

    //滚动条相关
    QWidget *m_parent{};
    //垂直滚动条
    QScrollBar *m_vScrollBar{};

    //发送网络请求
    CLibhttp m_libHttp;
};


#endif //LOCALSONG_H
