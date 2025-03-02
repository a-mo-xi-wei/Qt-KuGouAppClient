//
// Created by WeiWang on 24-11-2.
//

#include "MyStackedWidget.h"

MyStackedWidget::MyStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

}

void MyStackedWidget::resizeEvent(QResizeEvent *event) {
    QStackedWidget::resizeEvent(event);
    //for (int i = 0; i < this->count(); ++i) {
    //    auto widget = this->widget(i);
    //    widget->resize(this->size()-QSize(20,20));
    //    qDebug()<<"自己的宽度："<<this->width();
    //    qDebug() <<i+1<<" 号窗口宽度 "<< widget->width();
    //}
}

