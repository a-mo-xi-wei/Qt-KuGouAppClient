/**
 * @file WaterDrop.cpp
 * @brief 实现 WaterDrop 类，提供水滴动画效果
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#include "WaterDrop.h"
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

/**
 * @brief 水滴半径常量
 */
#define WATER_DROP_RADIUS 20

/**
 * @brief 构造函数，初始化水滴动画控件
 * @param parent 父控件指针，默认为 nullptr
 */
WaterDrop::WaterDrop(QWidget *parent)
    : QWidget(parent)
    , m_waterDropAnimation(new QVariantAnimation(this))
    , m_animationRadius(0)
{
    this->setFixedSize(QSize(WATER_DROP_RADIUS * 2, WATER_DROP_RADIUS * 2)); ///< 设置固定尺寸
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); ///< 无边框工具窗口
    this->setAttribute(Qt::WA_TranslucentBackground); ///< 透明背景
    this->setAttribute(Qt::WA_DeleteOnClose); ///< 关闭时自动删除

    // 初始化动画
    m_waterDropAnimation->setStartValue(0);
    m_waterDropAnimation->setEndValue(WATER_DROP_RADIUS);
    m_waterDropAnimation->setDuration(300);

    // 连接动画信号
    connect(m_waterDropAnimation, &QVariantAnimation::valueChanged, this, &WaterDrop::onRadiusChanged);
    connect(m_waterDropAnimation, &QVariantAnimation::finished, this, &WaterDrop::close);
    m_waterDropAnimation->start(); ///< 启动动画
}

/**
 * @brief 移动水滴到指定位置
 * @param point 目标位置（全局坐标）
 */
void WaterDrop::move(const QPoint &point)
{
    const QPoint translatePoint = point - QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS); ///< 调整到中心
    QWidget::move(translatePoint);
}

/**
 * @brief 重写绘制事件，绘制水滴动画
 * @param event 绘制事件
 */
void WaterDrop::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿
    QPen pen(QColor(0, 187, 255, 120 - m_animationRadius * 2)); ///< 动态透明度
    pen.setWidthF(3);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush); ///< 无填充

    // 绘制圆形路径
    QPainterPath hidePath;
    hidePath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS),
                        m_animationRadius, m_animationRadius);
    painter.drawPath(hidePath);
}

/**
 * @brief 处理动画半径变化
 * @param value 动画值
 */
void WaterDrop::onRadiusChanged(QVariant value)
{
    m_animationRadius = value.toInt();
    update(); ///< 触发重绘
}