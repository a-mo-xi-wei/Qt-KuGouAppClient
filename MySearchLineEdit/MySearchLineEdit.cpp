//
// Created by WeiWang on 24-10-10.
//

#include "MySearchLineEdit.h"
#include<QPropertyAnimation>

MySearchLineEdit::MySearchLineEdit(QWidget *parent)
    :QLineEdit(parent)
    ,m_animation(new QPropertyAnimation(this,"minimumWidth",this))
    ,m_maxWidth(250)
{}

void MySearchLineEdit::setWidth(const int &width) {
    this->m_maxWidth = width;
}

void MySearchLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);
    // 动画开始的大小（当前控件大小）
    this->m_startWidth = this->width();
    //qDebug()<<this->m_startWidth;
    // 动画结束的大小，宽度变为 m_maxWidth
    this->m_endWidth = this->m_maxWidth;
    //qDebug()<<this->m_endWidth;
    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_startWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_endWidth);      // 动画的结束值
    this->m_animation->start();
}

void MySearchLineEdit::focusOutEvent(QFocusEvent *event) {
    QLineEdit::focusOutEvent(event);
    // 动画结束的大小，宽度还原
    this->m_endWidth = this->m_startWidth;
    // 动画开始的大小（当前控件大小）
    this->m_startWidth = this->width();
    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_startWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_endWidth);      // 动画的结束值
    this->m_animation->start();
}

