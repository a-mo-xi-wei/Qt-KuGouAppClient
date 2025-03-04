//
// Created by WeiWang on 24-11-14.
//

#ifndef RECENTLYPLAYED_H
#define RECENTLYPLAYED_H

#include"RecentlySingleSong.h"
#include"RecentlySongList.h"
#include"RecentlyVideoWidget.h"
#include"RecentlySongChannel.h"
#include"RecentlyMVChannel.h"

#include <QWidget>

#if defined(RECENTLYPLAYED_LIBRARY)
#define RECENTLYPLAYED_EXPORT Q_DECL_EXPORT
#else
#define RECENTLYPLAYED_EXPORT Q_DECL_IMPORT
#endif

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyPlayed; }
QT_END_NAMESPACE

class RECENTLYPLAYED_EXPORT RecentlyPlayed : public QWidget {
Q_OBJECT

public:
    explicit RecentlyPlayed(QWidget *parent = nullptr);

    ~RecentlyPlayed() override;

private:
    void initUi();

    void initIndexLab();

    void initStackedWidget();

    void initSingleSong();

    void initSongList();

    void initVideoWidget();

    void initSongChannel();

    void initMVChannel();

private slots:
    void on_single_song_pushButton_clicked();

    void on_song_list_pushButton_clicked();

    void on_video_pushButton_clicked();

    void on_song_channel_pushButton_clicked();

    void on_MV_channel_pushButton_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    Ui::RecentlyPlayed *ui;
    std::unique_ptr<QButtonGroup>       m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<RecentlySingleSong>         m_singleSong{};
    std::unique_ptr<RecentlySongList>   m_songList{};
    std::unique_ptr<RecentlyVideoWidget>        m_videoWidget{};
    std::unique_ptr<RecentlySongChannel>        m_songChannel{};
    std::unique_ptr<RecentlyMVChannel>          m_mvChannel{};
};


#endif //RECENTLYPLAYED_H
