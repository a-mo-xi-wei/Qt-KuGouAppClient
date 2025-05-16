/**
 * @file qtmaterialripple.cpp
 * @brief 实现 QtMaterialRipple 类，提供水波纹动画效果
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialripple.h"
#include "qtmaterialrippleoverlay.h"

/**
 * @brief 构造函数，初始化水波纹动画
 * @param center 水波纹中心点
 * @param parent 父对象指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialRipple::QtMaterialRipple(const QPoint &center, QObject *parent)
    : QParallelAnimationGroup(parent),
      m_overlay(0),
      m_radiusAnimation(animate("radius")),
      m_opacityAnimation(animate("opacity")),
      m_radius(0),
      m_opacity(0),
      m_center(center)
{
    init(); ///< 初始化动画
}

/**
 * @brief 构造函数，初始化水波纹动画并关联覆盖层
 * @param center 水波纹中心点
 * @param overlay 水波纹覆盖层
 * @param parent 父对象指针，默认为 nullptr
 */
QtMaterialRipple::QtMaterialRipple(const QPoint &center,
                                   QtMaterialRippleOverlay *overlay,
                                   QObject *parent)
    : QParallelAnimationGroup(parent),
      m_overlay(overlay),
      m_radiusAnimation(animate("radius")),
      m_opacityAnimation(animate("opacity")),
      m_radius(0),
      m_opacity(0),
      m_center(center)
{
    init(); ///< 初始化动画
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialRipple::~QtMaterialRipple()
{
}

/**
 * @brief 设置水波纹半径
 * @param radius 半径值
 */
void QtMaterialRipple::setRadius(qreal radius)
{
    Q_ASSERT(m_overlay);

    if (m_radius == radius) {
        return;
    }
    m_radius = radius;
    m_overlay->update(); ///< 通知覆盖层更新
}

/**
 * @brief 设置水波纹透明度
 * @param opacity 透明度值
 */
void QtMaterialRipple::setOpacity(qreal opacity)
{
    Q_ASSERT(m_overlay);

    if (m_opacity == opacity) {
        return;
    }
    m_opacity = opacity;
    m_overlay->update(); ///< 通知覆盖层更新
}

/**
 * @brief 设置水波纹颜色
 * @param color 颜色值
 */
void QtMaterialRipple::setColor(const QColor &color)
{
    if (m_brush.color() == color) {
        return;
    }
    m_brush.setColor(color);
    if (m_overlay) {
        m_overlay->update(); ///< 通知覆盖层更新
    }
}

/**
 * @brief 设置水波纹画刷
 * @param brush 画刷对象
 */
void QtMaterialRipple::setBrush(const QBrush &brush)
{
    m_brush = brush;
    if (m_overlay) {
        m_overlay->update(); ///< 通知覆盖层更新
    }
}

/**
 * @brief 销毁水波纹对象
 */
void QtMaterialRipple::destroy()
{
    Q_ASSERT(m_overlay);

    m_overlay->removeRipple(this); ///< 从覆盖层移除水波纹
}

/**
 * @brief 创建属性动画
 * @param property 属性名称
 * @param easing 缓动曲线，默认为 OutQuad
 * @param duration 动画持续时间，默认为 800 毫秒
 * @return 动画对象指针
 * @note 内部实现
 */
QPropertyAnimation *QtMaterialRipple::animate(const QByteArray &property,
                                              const QEasingCurve &easing,
                                              int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation;
    animation->setTargetObject(this);
    animation->setPropertyName(property);
    animation->setEasingCurve(easing);
    animation->setDuration(duration);
    addAnimation(animation); ///< 添加到并行动画组
    return animation;
}

/**
 * @brief 初始化水波纹动画
 * @note 内部实现
 */
void QtMaterialRipple::init()
{
    setOpacityStartValue(0.5); ///< 设置透明度起始值
    setOpacityEndValue(0); ///< 设置透明度结束值
    setRadiusStartValue(0); ///< 设置半径起始值
    setRadiusEndValue(300); ///< 设置半径结束值

    m_brush.setColor(Qt::black); ///< 设置默认颜色
    m_brush.setStyle(Qt::SolidPattern); ///< 设置实心填充

    connect(this, SIGNAL(finished()), this, SLOT(destroy())); ///< 动画结束时销毁
}