//
// Created by WeiWang on 24-11-2.
//

#ifndef MYSTACKEDWIDGET_H
#define MYSTACKEDWIDGET_H

#include <QStackedWidget>

class MyStackedWidget : public QStackedWidget {
Q_OBJECT

public:
    explicit MyStackedWidget(QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif //MYSTACKEDWIDGET_H
