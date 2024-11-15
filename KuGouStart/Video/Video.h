//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEO_H
#define VIDEO_H

#include"VideoChannelWidget.h"
#include"VideoWidget.h"
#include"MVWidget.h"

#include <QWidget>
class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class Video; }
QT_END_NAMESPACE

class Video : public QWidget {
Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);

    ~Video() override;

    void initStackedWidget();

    void initVideoChannelWidget();

    void initVideoWidget();

    void initMVWidget();

private slots:
    void on_video_channel_pushButton_clicked();

    void on_MV_pushButton_clicked();

    void on_video_pushButton_clicked();

private:
    Ui::Video *ui;
    std::unique_ptr<QButtonGroup>       m_buttonGroup{};
    //stackedWidget
    std::unique_ptr<VideoChannelWidget> m_videoChannelWidget{};
    std::unique_ptr<VideoWidget>        m_videoWidget{};
    std::unique_ptr<MVWidget>           m_MVWidget{};
};


#endif //VIDEO_H
