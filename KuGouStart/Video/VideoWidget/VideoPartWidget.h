//
// Created by WeiWang on 24-12-15.
//

#ifndef VIDEOPARTWIDGET_H
#define VIDEOPARTWIDGET_H

#include "VideoBlockWidget.h"

#include <QWidget>

class QLabel;

class VideoPartWidget : public QWidget {
Q_OBJECT

public:
    explicit VideoPartWidget(QWidget *parent = nullptr);

    void setTitleName(const QString &title)const;

    void addBlockWidget(const int& x, const int& y, VideoBlockWidget* block)const;

private:
    void initUi();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel* m_titleLab{};   //标题
    QWidget* m_tabWidget{}; //下面的容器
};


#endif //VIDEOPARTWIDGET_H
