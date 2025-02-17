//
// Created by WeiWang on 25-2-17.
//

#ifndef POPULARRIGHTWIDGET_H
#define POPULARRIGHTWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class PopularRightWidget; }
QT_END_NAMESPACE

class PopularRightWidget : public QWidget {
Q_OBJECT

public:
    explicit PopularRightWidget(QWidget *parent = nullptr);
    ~PopularRightWidget() override;

private:
    Ui::PopularRightWidget *ui;
};


#endif //POPULARRIGHTWIDGET_H
