//
// Created by WeiWang on 25-2-17.
//

#ifndef LIVECOMMONPARTWIDGET_H
#define LIVECOMMONPARTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class LiveCommonPartWidget;
}

QT_END_NAMESPACE

class LiveCommonPartWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveCommonPartWidget(QWidget *parent = nullptr);

    ~LiveCommonPartWidget() override;

private:
    void setTitleName(const QString& name);

private:
    Ui::LiveCommonPartWidget *ui;
};


#endif //LIVECOMMONPARTWIDGET_H
