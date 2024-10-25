//
// Created by WeiWang on 24-10-10.
//

#ifndef LOCALDOWNLOAD_H
#define LOCALDOWNLOAD_H

#include"MusicItemWidget.h"

#include<QQueue>
#include<QWidget>
class QMediaPlayer;

QT_BEGIN_NAMESPACE
namespace Ui { class LocalDownload; }
QT_END_NAMESPACE

class LocalDownload : public QWidget {
Q_OBJECT

public:
    explicit LocalDownload(QWidget *parent = nullptr);

    ~LocalDownload() override;

    void init();

    void getMetaData();

    void loadNextSong();

    void getMenuPosition(const QPoint& pos);

    void MySort(std::function<bool(const MusicItemWidget*, const MusicItemWidget*)> comparator);

    void updateCurPlayIndex();
private slots:
    void on_local_play_toolButton_clicked();

    void on_local_add_toolButton_clicked();

    void on_local_music_pushButton_clicked();

    void on_downloaded_music_pushButton_clicked();

    void on_downloaded_video_pushButton_clicked();

    void on_downloading_pushButton_clicked();

    void on_local_sort_toolButton_clicked();

public slots:
    void setPlayIndex(const int& index);

    void onDefaultSort();

    void onAddTimeSort(const bool& down);

    void onSongNameSort(const bool& down);

    void onSingerSort(const bool& down);

    void onDurationSort(const bool& down);

    void onPlayCountSort(const bool& down);

    void onRandomSort();

signals:
    void playMusic(int index);

    void startPlay();

    void addSongInfo(const SongInfor& info);

    void syncSongInfo(QVector<SongInfor>& vec);

private:
    Ui::LocalDownload *ui;
    std::unique_ptr<QMediaPlayer>m_player;
    QVector<SongInfor> m_locationMusicVector;
    QVector<SongInfor> m_lastLocationMusicVector;//方便求得之前的下标
    QVector<MusicItemWidget*> m_MusicItemVector;//存放item，方便排序

    QAction* m_searchAction;//专门为了设置图片
    QString m_mediaPath;
    QQueue<QString> m_songQueue;

    //固定颜色专用
    int m_curPlatIndex = -1;
    int m_setPlayIndex = -1;

    //菜单相关
    MyMenu* m_sortOptMenu{};
    QPoint m_menuPosition;
};


#endif //LOCALDOWNLOAD_H
