//
// Created by WeiWang on 25-2-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PopularRightToolButton.h" resolved

#include "PopularRightToolButton.h"
#include "ui_PopularRightToolButton.h"

#include <QFile>
#include <QPainter>



PopularRightToolButton::PopularRightToolButton(QWidget *parent)
    : QToolButton(parent)
    , ui(new Ui::PopularRightToolButton)
{
    ui->setupUi(this);
    this->setCursor(Qt::PointingHandCursor);
    this->setStyleSheet("border: 3px solid rgb(69, 164, 255);border-radius:10px;");
}

PopularRightToolButton::~PopularRightToolButton() {
    delete ui;
}

void PopularRightToolButton::setBackgroundImg(const QString &path) {
    const QString style = QString("border: 3px solid rgb(69, 164, 255);border-radius:10px; background-image: url(%1);background-repeat: no-repeat;background-position: center;").arg(path);
    this->setStyleSheet(style);

}

void PopularRightToolButton::setLeftBottomText(const QString &text) {
    this->m_leftBottomText = text;
    update();
}

void PopularRightToolButton::paintEvent(QPaintEvent *event) {
    QToolButton::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::white); // 设置文字颜色
    painter.setFont(QFont(this->fontInfo().family(), 13)); // 设置字体

    // 计算左下角的绘制位置

    painter.drawText(10, this->height() - 15, this->m_leftBottomText);

}

