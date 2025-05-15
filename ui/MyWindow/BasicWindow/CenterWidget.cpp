/**
* @file CenterWidget.cpp
 * @brief 实现 CenterWidget 类，提供带线性渐变阴影的中心控件
 * @author WeiWang
 * @date 2024-09-18
 * @version 1.0
 */

#include "CenterWidget.h"
#include <QPainter>

/**
 * @brief 构造函数，初始化中心控件
 * @param parent 父控件指针，默认为 nullptr
 */
CenterWidget::CenterWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true); ///< 启用样式背景
    this->setObjectName("CenterWidget");
    this->setStyleSheet("QWidget#CenterWidget{margin:5px;}"); ///< 设置外边距
}

/**
 * @brief 重写绘制事件，绘制多段线性渐变矩形
 * @param ev 绘制事件
 */
void CenterWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen); ///< 无边框

    QRect shadowRect = rect().adjusted(5, 0, -5, 0); ///< 调整阴影区域
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomRight());
    gradient.setColorAt(0, QColor(QStringLiteral("#eef2ff"))); ///< 起始颜色
    gradient.setColorAt(0.25, QColor(QStringLiteral("#dff1ff"))); ///< 中间颜色
    gradient.setColorAt(0.5, QColor(QStringLiteral("#d8f9ff"))); ///< 中间颜色
    gradient.setColorAt(0.75, QColor(QStringLiteral("#dff1ff"))); ///< 中间颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); ///< 结束颜色
    p.setBrush(gradient);
    p.drawRect(shadowRect); ///< 绘制矩形

    // 以下为调试用透明度混合代码，当前未启用
    // p.setCompositionMode(QPainter::CompositionMode_Multiply);
    // gradient = QLinearGradient(rect().topRight(), rect().bottomLeft());
    // gradient.setColorAt(0, QColor("#eef2ff"));
    // gradient.setColorAt(0.25, QColor("#dff1ff"));
    // gradient.setColorAt(0.5, QColor("#e8ffd4"));
    // gradient.setColorAt(0.75, QColor("#dff1ff"));
    // gradient.setColorAt(1, QColor("#eef2ff"));
    // p.setBrush(gradient);
    // p.drawRoundedRect(rect(), 8, 8);
}