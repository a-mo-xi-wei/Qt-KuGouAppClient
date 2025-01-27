//
// Created by WeiWang on 25-1-27.
//

#ifndef DOWNLOADEDSONG_H
#define DOWNLOADEDSONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class DownloadedSong; }
QT_END_NAMESPACE

class DownloadedSong : public QWidget {
Q_OBJECT

public:
    explicit DownloadedSong(QWidget *parent = nullptr);
    ~DownloadedSong() override;

private:
    Ui::DownloadedSong *ui;
};


#endif //DOWNLOADEDSONG_H
