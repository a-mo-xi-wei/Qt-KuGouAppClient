//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVEBIGLEFTWIDGET_H
#define LIVEBIGLEFTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveBigLeftWidget;
}

QT_END_NAMESPACE

class LiveBigLeftWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveBigLeftWidget(QWidget *parent = nullptr);

    ~LiveBigLeftWidget() override;

private:
    Ui::LiveBigLeftWidget *ui;
};


#endif //LIVEBIGLEFTWIDGET_H
