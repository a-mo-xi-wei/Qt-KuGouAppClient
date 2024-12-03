//
// Created by WeiWang on 24-12-3.
//
#include "SScrollArea.h"

#include <QScrollBar>

SScrollArea::SScrollArea(QWidget *parent) :
    QScrollArea(parent){

}

void SScrollArea::wheelEvent(QWheelEvent *event) {
    if (this->verticalScrollBar()) {
        emit wheelValue(this->verticalScrollBar()->value());
    }
    QScrollArea::wheelEvent(event);
}
