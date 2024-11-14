//
// Created by WeiWang on 24-11-14.
//

#ifndef MUSICCLOUDDISK_H
#define MUSICCLOUDDISK_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MusicCloudDisk; }
QT_END_NAMESPACE

class MusicCloudDisk : public QWidget {
Q_OBJECT

public:
    explicit MusicCloudDisk(QWidget *parent = nullptr);
    ~MusicCloudDisk() override;

private:
    Ui::MusicCloudDisk *ui;
};


#endif //MUSICCLOUDDISK_H
