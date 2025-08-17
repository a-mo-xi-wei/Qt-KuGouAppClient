/**
 * @file RippleButton.cpp
 * @brief 实现 RippleButton 类，提供带涟漪效果的按钮功能
 * @author [WeiWang]
 * @date 2025-05-13
 * @version 1.0
 */

#include "RippleButton.h"

#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QCoreApplication>
#include <QPainterPath>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QPointF>
#include <QTimer>

/**
 * @brief 构造函数，初始化涟漪按钮
 * @param parent 父控件指针，默认为 nullptr
 */
RippleButton::RippleButton(QWidget* parent)
    : QToolButton(parent),
    timer(new QTimer(this)),
    checkTimer(new QTimer(this)),
    fill_color(QStringLiteral("#DDDDDD")),
    m_effect(std::make_unique<QGraphicsDropShadowEffect>(this))
{
    timer->setInterval(timeInterval); // 设置定时器时间间隔
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height())); // 计算最大半径
    //设置按钮的阴影效果
    m_effect->setOffset(0, 0); //阴影的偏移量（右，下）
    m_effect->setColor(QColor(0, 0, 0)); //阴影的颜色
    m_effect->setBlurRadius(6); //控制阴影的模糊程度（光源距离）
    setGraphicsEffect(m_effect.get());
}

/**
 * @brief 设置涟漪动画速度
 * @param timeInterval 时间间隔（毫秒）
 */
void RippleButton::setSpeed(const int &timeInterval) const
{
    timer->setInterval(timeInterval);
}

/**
 * @brief 设置涟漪填充颜色
 * @param fillcolor 填充颜色
 */
void RippleButton::setFillColor(const QColor &fillcolor)
{
    fill_color = fillcolor;
}

/**
 * @brief 设置圆角半径
 * @param radius_ 圆角半径
 */
void RippleButton::setRadius(int radius_)
{
    frame_radius = radius_;
}

/**
 * @brief 设置按钮图标
 * @param ico 图标
 */
void RippleButton::setMyIcon(const QIcon &ico)
{
    m_ico = ico;
    setIcon(m_ico);
}

/**
 * @brief 设置按钮启用状态
 * @param flag 是否启用
 */
void RippleButton::setEnabled(const bool &flag)
{
    QToolButton::setEnabled(flag);
    if (flag)
    {
        setCursor(Qt::PointingHandCursor); // 启用时恢复为手形光标
    }
    else
    {
        setCursor(Qt::WaitCursor); // 禁用时强制为等待光标    //没有效果，不知道为什么。。。
    }
}

/**
 * @brief 鼠标进入事件
 * @param event 进入事件对象
 */
void RippleButton::enterEvent(QEnterEvent *event)
{
    QToolButton::enterEvent(event);
    if (!this->isEnabled())return;
    mouse_point = event->position(); // 记录鼠标进入坐标
    timer->disconnect(); // 断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发，半径增大
        radius += radius_var;
        if (radius > max_radius) {
            timer->stop(); // 达到最大半径，定时器停止
            return;
        }
        update(); // 调用绘制事件
    });
    timer->start(); // 定时器开始

    // 启动一个检测鼠标是否在按钮上的定时器
    checkTimer->disconnect();
    connect(checkTimer, &QTimer::timeout, this, [=] {
        // 检查鼠标是否在按钮内
        //qDebug()<<"本身区域："<<this->rect()<<"鼠标 ："<<this->mapFromGlobal(QCursor::pos());
        if (!this->rect().contains(this->mapFromGlobal(QCursor::pos()))) {
            checkTimer->stop(); // 停止定时器
            // 创建一个离开事件（QEvent::Leave）
            QEvent leaveEvent(QEvent::Leave);

            // 模拟鼠标从特定位置（例如 (0, 0)）离开的逻辑
            mouse_point = QPointF(0,0);

            // 手动触发 leaveEvent，发送到这个按钮
            QCoreApplication::sendEvent(this, &leaveEvent);
        }
    });
    checkTimer->start(1000); // 每1000毫秒检测一次
}

/**
 * @brief 鼠标离开事件
 * @param event 事件对象
 */
void RippleButton::leaveEvent(QEvent *ev)
{
    if (!isEnabled())
        return;
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发半径减小
        radius -= radius_var;
        if (radius < 0) {
            timer->stop(); // 减小到小于0时定时器停止
            radius = 0; // 确保半径不为负
            return;
        }
        update();
    });
    timer->start();
    QToolButton::leaveEvent(ev);
}

/**
 * @brief 绘制事件
 * @param event 绘图事件对象
 */
void RippleButton::paintEvent(QPaintEvent *event)
{
    if (mouse_point.isNull())
        mouse_point = QPointF(0, 0);
    if (!mouse_point.isNull() && radius > 0)
    {
        QPainter painter(this);
        QPainterPath path;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(fill_color));
        painter.setPen(Qt::NoPen);
        path.addRoundedRect(rect(), frame_radius, frame_radius);
        painter.setClipPath(path);
        painter.drawEllipse(mouse_point, radius, radius); // 画圆
        int iconSize = 12;
        m_ico.paint(&painter, (width() - iconSize) / 2, (height() - iconSize) / 2, iconSize, iconSize);
    }
    QToolButton::paintEvent(event);
}

/**
 * @brief 大小调整事件
 * @param event 大小调整事件对象
 */
void RippleButton::resizeEvent(QResizeEvent *event)
{
    QToolButton::resizeEvent(event);
    max_radius = static_cast<int>(qSqrt(width() * width() + height() * height()));
}