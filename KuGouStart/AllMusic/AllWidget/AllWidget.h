//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLWIDGET_H
#define ALLWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class AllWidget; }
QT_END_NAMESPACE

class AllWidget : public QWidget {
Q_OBJECT

public:
    explicit AllWidget(QWidget *parent = nullptr);
    ~AllWidget() override;

private:
    Ui::AllWidget *ui;
};


#endif //ALLWIDGET_H