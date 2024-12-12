//
// Created by WeiWang on 24-10-16.
//
#include "MyScrollArea.h"
#include <QWheelEvent>
#include <QScrollBar>

MyScrollArea::MyScrollArea(QWidget *parent)
    : QScrollArea(parent){
}

void MyScrollArea::setAnimating(const bool &animating) {
    this->m_isAnimating = animating;
}

void MyScrollArea::setIgnore(const bool &ignore) {
    this->m_ignore = ignore;
}

void MyScrollArea::setScrollAreaKind(const ScrollAreaKind &kind) {
    this->m_scrollAreaKind = kind;
}

void MyScrollArea::wheelEvent(QWheelEvent *event) {
    if (this->m_scrollAreaKind == ScrollAreaKind::Inside) {
        if (this->verticalScrollBar()) {
            emit wheelValue(this->verticalScrollBar()->value());
        }
    }
    if (!this->m_isAnimating) {
        // 如果没有动画进行，正常处理滚轮事件
        //qDebug() << "MyScrollArea接受滚轮事件";
        if (this->m_ignore) {
            event->ignore();
            return;
        }
        QScrollArea::wheelEvent(event);
    } else {
        // 如果正在进行动画，忽略滚轮事件
        event->ignore();
    }
}
