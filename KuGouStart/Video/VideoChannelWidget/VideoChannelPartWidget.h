//
// Created by WeiWang on 24-12-12.
//

#ifndef VIDEOCHANNELPARTWIDGET_H
#define VIDEOCHANNELPARTWIDGET_H

#include "VideoChannelBlock.h"

#include <QWidget>

class QLabel;

class VideoChannelPartWidget : public QWidget {
Q_OBJECT

public:
    explicit VideoChannelPartWidget(QWidget *parent = nullptr);

    void setTitleName(const QString &title);

    void addBlockWidget(VideoChannelBlock* block);

private:
    void initUi();


private:
    QLabel* m_titleLab{};   //标题
    QLabel* m_titleDeco{};  //标题旁边的 .MHz
    QWidget* m_tabWidget{}; //下面的容器

};

#endif //VIDEOCHANNELPARTWIDGET_H
