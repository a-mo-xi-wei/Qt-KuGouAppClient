//
// Created by WeiWang on 24-10-16.
//

#ifndef UPWIDGET_H
#define UPWIDGET_H

#include <QToolButton>

class UpToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit UpToolButton(QWidget *parent = nullptr);

    void initUi();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;
};


#endif //UPWIDGET_H
