//
// Created by WeiWang on 24-10-16.
//

#include "UpToolButton.h"

#include<QFile>
UpToolButton::UpToolButton(QWidget *parent)
    :QToolButton(parent)
{
    initUi();
}

void UpToolButton::initUi() {
    this->setFixedSize(34,34);
    //设置样式
    this->setStyleSheet(QStringLiteral("border-radius:5px;background-color:rgba(177,177,177,200);"));

    this->setCursor(Qt::PointingHandCursor);
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-white.svg")));
    this->hide();
}

void UpToolButton::enterEvent(QEnterEvent *event) {
    QToolButton::enterEvent(event);
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-hover.svg")));
}

void UpToolButton::leaveEvent(QEvent *event) {
    QToolButton::leaveEvent(event);
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-white.svg")));
}
