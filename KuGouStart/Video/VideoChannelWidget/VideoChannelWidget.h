//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOCHANNELWIDGET_H
#define VIDEOCHANNELWIDGET_H

#include"UpToolButton.h"

#include <QWidget>

class QScrollBar;
class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoChannelWidget;
}

QT_END_NAMESPACE

class VideoChannelWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoChannelWidget(QWidget *parent = nullptr);

    ~VideoChannelWidget() override;

private:
    Ui::VideoChannelWidget *ui;
};


#endif //VIDEOCHANNELWIDGET_H
