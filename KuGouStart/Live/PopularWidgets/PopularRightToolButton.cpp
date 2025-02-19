//
// Created by WeiWang on 25-2-18.
//

#include "PopularRightToolButton.h"

#include <QFile>
#include <QPainter>

PopularRightToolButton::PopularRightToolButton(QWidget *parent)
    : QToolButton(parent)
{
    this->setCursor(Qt::PointingHandCursor);
    // 设置普通状态样式
    this->setStyleSheet(R"(
        QToolButton {
            border-radius: 10px;
        }
        QToolButton:checked {
            border: 3px solid rgb(69, 164, 255);
        }
    )");
}

void PopularRightToolButton::setBackgroundImg(const QString &path) {
    const QString style = QString(R"(
        QToolButton {
            border-radius: 10px;
            background-image: url(%1);
            background-repeat: no-repeat;
            background-position: center;
        }
        QToolButton:checked{
            border: 3px solid rgb(69, 164, 255);
        }
    )").arg(path);
    // 合并正常样式和选中样式
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

