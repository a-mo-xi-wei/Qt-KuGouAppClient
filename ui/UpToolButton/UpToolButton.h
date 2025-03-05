//
// Created by WeiWang on 24-10-16.
//

#ifndef UPWIDGET_H
#define UPWIDGET_H

#include <QToolButton>

#if defined(UPWIDGET_LIBRARY)
#define UPWIDGET_EXPORT Q_DECL_EXPORT
#else
#define UPWIDGET_EXPORT Q_DECL_IMPORT
#endif

class UPWIDGET_EXPORT UpToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit UpToolButton(QWidget *parent = nullptr);

    void initUi();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;
};


#endif //UPWIDGET_H
