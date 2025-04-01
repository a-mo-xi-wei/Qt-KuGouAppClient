//
// Created by WeiWang on 24-10-16.
//

#ifndef UPTOOLBUTTON_H
#define UPTOOLBUTTON_H

#include <QToolButton>

#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

class MYBUTTON_EXPORT UpToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit UpToolButton(QWidget *parent = nullptr);

    void initUi();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;
};


#endif //UPTOOLBUTTON_H
