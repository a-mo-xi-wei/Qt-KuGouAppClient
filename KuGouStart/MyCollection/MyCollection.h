//
// Created by WeiWang on 24-11-14.
//

#ifndef MYCOLLECTION_H
#define MYCOLLECTION_H

#include"SingleSong/SingleSong.h"
#include"SongListWidget/SongListWidget.h"
#include"SpecialAlbum/SpecialAlbum.h"
#include"CollectVideo/CollectVideo.h"
#include"SingerWidget/SingerWidget.h"
#include"DeviceWidget/DeviceWidget.h"

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class MyCollection; }
QT_END_NAMESPACE

class MyCollection : public QWidget {
Q_OBJECT

public:
    explicit MyCollection(QWidget *parent = nullptr);

    ~MyCollection() override;

private:
    void initStackedWidget();

    void initUi();

    void initIndexLab();

    void initSingleSong();

    void initSongList();

    void initSpecialAlbum();

    void initCollectVideo();

    void initSinger();

    void initDevice();

    void enableButton(const bool& flag) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void on_singleSong_pushButton_clicked();

    void on_songList_pushButton_clicked();

    void on_specialAlbum_pushButton_clicked();

    void on_collectVideo_pushButton_clicked();

    void on_singer_pushButton_clicked();

    void on_device_pushButton_clicked();

private:
    Ui::MyCollection *ui;
    std::unique_ptr<QButtonGroup>   m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<SingleSong>     m_singleSong{};
    std::unique_ptr<SongListWidget> m_songList{};
    std::unique_ptr<SpecialAlbum>   m_specialAlbum{};
    std::unique_ptr<CollectVideo>   m_collectVideo{};
    std::unique_ptr<SingerWidget>   m_singerWidget{};
    std::unique_ptr<DeviceWidget>   m_deviceWidget{};

};


#endif //MYCOLLECTION_H
