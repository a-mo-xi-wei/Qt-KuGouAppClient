//
// Created by WeiWang on 25-1-27.
//

#ifndef DOWNLOADEDVIDEO_H
#define DOWNLOADEDVIDEO_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class DownloadedVideo; }
QT_END_NAMESPACE

class DownloadedVideo : public QWidget {
Q_OBJECT

public:
    explicit DownloadedVideo(QWidget *parent = nullptr);
    ~DownloadedVideo() override;

private:
    Ui::DownloadedVideo *ui;
};


#endif //DOWNLOADEDVIDEO_H
