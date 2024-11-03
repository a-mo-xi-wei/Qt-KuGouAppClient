//
// Created by WeiWang on 24-11-2.
//

#include "MyStackedWidget.h"

MyStackedWidget::MyStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

}

void MyStackedWidget::resizeEvent(QResizeEvent *event) {
    QStackedWidget::resizeEvent(event);
    //for (int i = 0; i < this->count(); ++i) {
    //    auto widget = this->widget(i);
    //    qDebug() <<i+1<<" 号窗口大小 "<< widget->width();
    //}
}

