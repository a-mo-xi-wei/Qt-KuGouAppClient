//
// Created by WeiWang on 24-10-10.
//

#ifndef LOCALDOWNLOAD_H
#define LOCALDOWNLOAD_H

#include "LocalSong/LocalSong.h"
#include "DownloadedSong/DownloadedSong.h"
#include "DownloadedVideo/DownloadedVideo.h"
#include "Downloading/Downloading.h"

class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class LocalDownload;
}

QT_END_NAMESPACE

class LocalDownload : public QWidget {
    Q_OBJECT

public:
    explicit LocalDownload(QWidget *parent = nullptr);

    ~LocalDownload() override;

public slots:
    void setPlayIndex(const int &index);

    void onMaxScreenHandle();

private:
    void initStackedWidget();

    void initUi();

    void initLocalSong();

    void initDownloadedSong();

    void initDownloadedVideo();

    void initDownloading();

    void enableButton(const bool& flag) const;

private slots:
    //ui相关
    void on_local_music_pushButton_clicked();

    void on_downloaded_music_pushButton_clicked();

    void on_downloaded_video_pushButton_clicked();

    void on_downloading_pushButton_clicked();

    //数量标签变化
    void local_music_label_changed(const int& num);

signals:
    void playMusic(const int& index);

    void startPlay();

    void addSongInfo(const SongInfor &info);

    void subSongInfo(const SongInfor &info);

    //同步(更新)Vec
    void syncSongInfo(QVector<SongInfor> &vec);

private:
    Ui::LocalDownload *ui;
    std::unique_ptr<QButtonGroup>       m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<LocalSong>          m_localSong{};
    std::unique_ptr<DownloadedSong>     m_downloadedSong{};
    std::unique_ptr<DownloadedVideo>    m_downloadedVideo{};
    std::unique_ptr<Downloading>        m_downloading{};
};

#endif //LOCALDOWNLOAD_H
