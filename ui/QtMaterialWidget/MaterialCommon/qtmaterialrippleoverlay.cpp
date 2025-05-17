/**
 * @file qtmaterialrippleoverlay.cpp
 * @brief 实现 QtMaterialRippleOverlay 类，提供水波纹覆盖层功能
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialrippleoverlay.h"
#include "qtmaterialripple.h"

#include <QPainter>

/**
 * @brief 构造函数，初始化水波纹覆盖层
 * @param parent 父控件指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialRippleOverlay::QtMaterialRippleOverlay(QWidget *parent)
    : QtMaterialOverlayWidget(parent),
      m_useClip(false)
{
    setAttribute(Qt::WA_TransparentForMouseEvents); ///< 忽略鼠标事件
    setAttribute(Qt::WA_NoSystemBackground); ///< 无系统背景
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialRippleOverlay::~QtMaterialRippleOverlay()
{
}

/**
 * @brief 添加水波纹对象
 * @param ripple 水波纹对象指针
 */
void QtMaterialRippleOverlay::addRipple(QtMaterialRipple *ripple)
{
    ripple->setOverlay(this); ///< 设置覆盖层
    m_ripples.push_back(ripple); ///< 添加到列表
    ripple->start(); ///< 启动动画

    // 连接销毁信号
    connect(this, SIGNAL(destroyed(QObject*)), ripple, SLOT(stop()));
    connect(this, SIGNAL(destroyed(QObject*)), ripple, SLOT(deleteLater()));
}

/**
 * @brief 添加水波纹效果
 * @param position 水波纹中心点
 * @param radius 水波纹结束半径
 */
void QtMaterialRippleOverlay::addRipple(const QPoint &position, qreal radius)
{
    QtMaterialRipple *ripple = new QtMaterialRipple(position);
    ripple->setRadiusEndValue(radius);
    addRipple(ripple); ///< 添加水波纹
}

/**
 * @brief 移除水波纹对象
 * @param ripple 水波纹对象指针
 */
void QtMaterialRippleOverlay::removeRipple(QtMaterialRipple *ripple)
{
    if (m_ripples.removeOne(ripple)) {
        delete ripple; ///< 删除水波纹
        update(); ///< 触发重绘
    }
}

/**
 * @brief 重写绘制事件，绘制水波纹效果
 * @param event 绘制事件
 */
void QtMaterialRippleOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿
    painter.setPen(Qt::NoPen); ///< 无边框

    if (m_useClip) {
        painter.setClipPath(m_clipPath); ///< 应用裁剪路径
    }

    // 绘制所有水波纹
    QList<QtMaterialRipple *>::const_iterator i;
    for (i = m_ripples.begin(); i != m_ripples.end(); ++i) {
        paintRipple(&painter, *i);
    }
}

/**
 * @brief 绘制单个水波纹
 * @param painter 画家对象
 * @param ripple 水波纹对象
 */
void QtMaterialRippleOverlay::paintRipple(QPainter *painter, QtMaterialRipple *ripple)
{
    const qreal radius = ripple->radius();
    const QPointF center = ripple->center();
    painter->setOpacity(ripple->opacity()); ///< 设置透明度
    painter->setBrush(ripple->brush()); ///< 设置画刷
    painter->drawEllipse(center, radius, radius); ///< 绘制椭圆
}