//
// Created by WeiWang on 24-11-12.
//

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoWidget; }
QT_END_NAMESPACE

class VideoWidget : public QWidget {
Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget() override;

private:
    Ui::VideoWidget *ui;
};


#endif //VIDEOWIDGET_H
