//
// Created by WeiWang on 24-10-13.
//

#include "PlayWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

PlayWidget::PlayWidget(QWidget *parent)
    : QWidget(parent) {
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setObjectName("PlayWidget");
    this->setStyleSheet("QWidget#PlayWidget{margin:5px;}");
}

void PlayWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发送自定义的双击信号
    }
}

void PlayWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    // 先绘制阴影效果
    QRect shadowRect = rect().adjusted(5, 0, -4, -5);
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#eef2ff"))); // 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#87CEFA"))); // 结束颜色
    p.setBrush(gradient);
    QPainterPath path;
    int radius = 8; // 圆角半径

    path.moveTo(shadowRect.topLeft()); // 移动到左上角
    path.lineTo(shadowRect.topRight()); // 绘制上边的直线到右上角
    path.lineTo(shadowRect.bottomRight() - QPoint(0, radius)); // 到右下角的圆角前的点
    path.quadTo(shadowRect.bottomRight(), shadowRect.bottomRight() - QPoint(radius, 0)); // 右下角圆角
    path.lineTo(shadowRect.bottomLeft() + QPoint(radius, 0)); // 到左下角的圆角前的点
    path.quadTo(shadowRect.bottomLeft(), shadowRect.bottomLeft() - QPoint(0, radius)); // 左下角圆角
    path.lineTo(shadowRect.topLeft()); // 回到左上角
    path.closeSubpath(); // 关闭路径

    p.drawPath(path); // 绘制路径
}
