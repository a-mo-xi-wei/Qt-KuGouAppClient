//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVEMUSICPARTWIDGET_H
#define LIVEMUSICPARTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveMusicPartWidget;
}

QT_END_NAMESPACE

class LiveMusicPartWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveMusicPartWidget(QWidget *parent = nullptr);

    ~LiveMusicPartWidget() override;

private:
    Ui::LiveMusicPartWidget *ui;
};


#endif //LIVEMUSICPARTWIDGET_H
