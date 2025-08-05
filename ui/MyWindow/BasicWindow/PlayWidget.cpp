/**
* @file PlayWidget.cpp
 * @brief 实现 PlayWidget 类，提供带圆角阴影和双击信号的播放控件
 * @author WeiWang
 * @date 2024-10-13
 * @version 1.0
 */

#include "PlayWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

/**
 * @brief 构造函数，初始化播放控件
 * @param parent 父控件指针，默认为 nullptr
 */
PlayWidget::PlayWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    this->setObjectName("PlayWidget");
    this->setStyleSheet("QWidget#PlayWidget{font-family: 'TaiwanPearl';margin:5px;}"); ///< 设置外边距
}

/**
 * @brief 重写鼠标双击事件，触发双击信号
 * @param event 鼠标事件
 */
void PlayWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        emit doubleClicked(); ///< 左键双击时发出自定义信号
    }
}

/**
 * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
 * @param ev 绘制事件
 */
void PlayWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen); ///< 无边框

    QRect shadowRect = rect().adjusted(5, 0, -4, -5); ///< 调整阴影区域

    // QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    // gradient.setColorAt(0, QColor(QStringLiteral("#eef2ff"))); ///< 起始颜色
    // gradient.setColorAt(1, QColor(QStringLiteral("#87CEFA"))); ///< 结束颜色
    // p.setBrush(gradient);

    QPainterPath path;
    int radius = 8; ///< 圆角半径

    // 构建圆角矩形路径（左下和右下角有圆角）
    path.moveTo(shadowRect.topLeft());
    path.lineTo(shadowRect.topRight());
    path.lineTo(shadowRect.bottomRight() - QPoint(0, radius));
    path.quadTo(shadowRect.bottomRight(), shadowRect.bottomRight() - QPoint(radius, 0));
    path.lineTo(shadowRect.bottomLeft() + QPoint(radius, 0));
    path.quadTo(shadowRect.bottomLeft(), shadowRect.bottomLeft() - QPoint(0, radius));
    path.lineTo(shadowRect.topLeft());
    path.closeSubpath();

    p.setClipPath(path);               // 限制绘制范围为圆角区域
    p.drawPath(path); ///< 绘制路径
}