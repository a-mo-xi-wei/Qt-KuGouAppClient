//
// Created by WeiWang on 24-11-14.
//

#ifndef RECENTLYPLAYED_H
#define RECENTLYPLAYED_H

#include"SingleSong.h"
#include"RecentlySongList.h"
#include"RecentlyVideoWidget.h"
#include"SongChannel.h"
#include"MVChannel.h"

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyPlayed; }
QT_END_NAMESPACE

class RecentlyPlayed : public QWidget {
Q_OBJECT

public:
    explicit RecentlyPlayed(QWidget *parent = nullptr);

    ~RecentlyPlayed() override;

    void initStackedWidet();

    void initSingleSong();

    void initSongList();

    void initVideoWidget();

    void initSongChannel();

    void initMVChannel();

private slots:
    void on_singleSong_pushButton_clicked();

    void on_songList_pushButton_clicked();

    void on_video_pushButton_clicked();

    void on_song_channel_pushButton_clicked();

    void on_MV_channel_pushButton_clicked();

private:
    Ui::RecentlyPlayed *ui;
    std::unique_ptr<QButtonGroup>       m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<SingleSong>         m_singleSong{};
    std::unique_ptr<RecentlySongList>   m_songList{};
    std::unique_ptr<RecentlyVideoWidget>        m_videoWidget{};
    std::unique_ptr<SongChannel>        m_songChannel{};
    std::unique_ptr<MVChannel>          m_mvChannel{};
};


#endif //RECENTLYPLAYED_H
