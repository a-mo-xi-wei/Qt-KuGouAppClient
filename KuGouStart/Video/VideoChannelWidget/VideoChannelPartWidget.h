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

    void setTitleName(const QString &title)const;

    void addBlockWidget(VideoChannelBlock* block)const;

private:
    void initUi();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel* m_titleLab{};   //标题
    QWidget* m_tabWidget{}; //下面的容器

};

#endif //VIDEOCHANNELPARTWIDGET_H
