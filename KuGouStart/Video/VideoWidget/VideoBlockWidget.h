//
// Created by WeiWang on 24-12-15.
//

#ifndef VIDEOBLOCKWIDGET_H
#define VIDEOBLOCKWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoBlockWidget; }
QT_END_NAMESPACE

class VideoBlockWidget : public QWidget {
Q_OBJECT

public:
    explicit VideoBlockWidget(QWidget *parent = nullptr);
    ~VideoBlockWidget() override;

private:
    Ui::VideoBlockWidget *ui;
};


#endif //VIDEOBLOCKWIDGET_H
