//
// Created by WeiWang on 24-11-2.
//

#ifndef MYSTACKEDWIDGET_H
#define MYSTACKEDWIDGET_H

#include <QStackedWidget>

#if defined(MYSTACKEDWIDGET_LIBRARY)
#define MYSTACKEDWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MYSTACKEDWIDGET_EXPORT Q_DECL_IMPORT
#endif

class MYSTACKEDWIDGET_EXPORT MyStackedWidget : public QStackedWidget {
    Q_OBJECT

public:
    explicit MyStackedWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif //MYSTACKEDWIDGET_H
