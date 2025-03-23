//
// Created by WeiWang on 24-11-15.
//

#ifndef COLLECTVIDEO_H
#define COLLECTVIDEO_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CollectVideo; }
QT_END_NAMESPACE

class CollectVideo : public QWidget {
Q_OBJECT

public:
    explicit CollectVideo(QWidget *parent = nullptr);
    ~CollectVideo() override;

private:
    Ui::CollectVideo *ui;
};


#endif //COLLECTVIDEO_H
