//
// Created by WeiWang on 24-9-18.
//

#include "TitleWidget.h"
#include<QPainter>
#include <QPainterPath>

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent) {
    this->setObjectName("TitleWidget");
    this->setStyleSheet("QWidget#TitleWidget{margin:5px;}");
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发送自定义的双击信号
    }
}

void TitleWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    // 先绘制阴影效果
    QRect shadowRect = rect().adjusted(5, 5, -4, 2);
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#87CEFA"))); // 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); // 结束颜色
    p.setBrush(gradient);
    //p.drawRoundedRect(shadowRect, 8, 8);
    // 创建一个 QPainterPath，只在左上和右上角有圆角
    QPainterPath path;
    int radius = 8; // 圆角半径

    path.moveTo(shadowRect.topLeft() + QPoint(radius, 0)); // 从左上角的圆角开始
    path.lineTo(shadowRect.topRight() - QPoint(radius, 0)); // 右上角前的直线部分
    path.quadTo(shadowRect.topRight(), shadowRect.topRight() + QPoint(0, radius)); // 右上角的圆角
    path.lineTo(shadowRect.bottomRight()); // 右侧的直线部分到底
    path.lineTo(shadowRect.bottomLeft()); // 底部的直线部分到左侧
    path.lineTo(shadowRect.topLeft() + QPoint(0, radius)); // 左侧的直线部分
    path.quadTo(shadowRect.topLeft(), shadowRect.topLeft() + QPoint(radius, 0)); // 左上角的圆角
    path.closeSubpath();

    p.drawPath(path); // 绘制路径
}
