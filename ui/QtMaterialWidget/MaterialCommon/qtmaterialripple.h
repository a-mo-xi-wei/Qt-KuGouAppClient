/**
 * @file qtmaterialripple.h
 * @brief 定义 QtMaterialRipple 类，提供水波纹动画效果
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALRIPPLE_H
#define QTMATERIALRIPPLE_H

#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QPoint>
#include <QBrush>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialRippleOverlay;

/**
 * @class QtMaterialRipple
 * @brief 水波纹动画类，继承自 QParallelAnimationGroup，管理半径和透明度动画
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialRipple : public QParallelAnimationGroup
{
    Q_OBJECT

    Q_PROPERTY(qreal radius WRITE setRadius READ radius) ///< 半径属性
    Q_PROPERTY(qreal opacity WRITE setOpacity READ opacity) ///< 透明度属性

public:
    /**
     * @brief 构造函数，初始化水波纹动画
     * @param center 水波纹中心点
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit QtMaterialRipple(const QPoint &center, QObject *parent = 0);

    /**
     * @brief 构造函数，初始化水波纹动画并关联覆盖层
     * @param center 水波纹中心点
     * @param overlay 水波纹覆盖层
     * @param parent 父对象指针，默认为 nullptr
     */
    QtMaterialRipple(const QPoint &center, QtMaterialRippleOverlay *overlay, QObject *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialRipple() override;

    /**
     * @brief 设置水波纹覆盖层
     * @param overlay 覆盖层指针
     */
    inline void setOverlay(QtMaterialRippleOverlay *overlay);

    /**
     * @brief 设置水波纹半径
     * @param radius 半径值
     */
    void setRadius(qreal radius);

    /**
     * @brief 获取水波纹半径
     * @return 半径值
     */
    inline qreal radius() const;

    /**
     * @brief 设置水波纹透明度
     * @param opacity 透明度值
     */
    void setOpacity(qreal opacity);

    /**
     * @brief 获取水波纹透明度
     * @return 透明度值
     */
    inline qreal opacity() const;

    /**
     * @brief 设置水波纹颜色
     * @param color 颜色值
     */
    void setColor(const QColor &color);

    /**
     * @brief 获取水波纹颜色
     * @return 颜色值
     */
    inline QColor color() const;

    /**
     * @brief 设置水波纹画刷
     * @param brush 画刷对象
     */
    void setBrush(const QBrush &brush);

    /**
     * @brief 获取水波纹画刷
     * @return 画刷对象
     */
    inline QBrush brush() const;

    /**
     * @brief 获取水波纹中心点
     * @return 中心点坐标
     */
    inline QPoint center() const;

    /**
     * @brief 获取半径动画对象
     * @return 半径动画指针
     */
    inline QPropertyAnimation *radiusAnimation() const;

    /**
     * @brief 获取透明度动画对象
     * @return 透明度动画指针
     */
    inline QPropertyAnimation *opacityAnimation() const;

    /**
     * @brief 设置透明度动画起始值
     * @param value 起始值
     */
    inline void setOpacityStartValue(qreal value);

    /**
     * @brief 设置透明度动画结束值
     * @param value 结束值
     */
    inline void setOpacityEndValue(qreal value);

    /**
     * @brief 设置半径动画起始值
     * @param value 起始值
     */
    inline void setRadiusStartValue(qreal value);

    /**
     * @brief 设置半径动画结束值
     * @param value 结束值
     */
    inline void setRadiusEndValue(qreal value);

    /**
     * @brief 设置动画持续时间
     * @param msecs 持续时间（毫秒）
     */
    inline void setDuration(int msecs);

protected slots:
    /**
     * @brief 销毁水波纹对象
     */
    void destroy();

private:
    Q_DISABLE_COPY(QtMaterialRipple) ///< 禁用拷贝

    /**
     * @brief 创建属性动画
     * @param property 属性名称
     * @param easing 缓动曲线，默认为 OutQuad
     * @param duration 动画持续时间，默认为 800 毫秒
     * @return 动画对象指针
     */
    QPropertyAnimation *animate(const QByteArray &property,
                                const QEasingCurve &easing = QEasingCurve::OutQuad,
                                int duration = 800);

    /**
     * @brief 初始化水波纹动画
     */
    void init();

    QtMaterialRippleOverlay  *m_overlay;          ///< 水波纹覆盖层
    QPropertyAnimation *const m_radiusAnimation;  ///< 半径动画
    QPropertyAnimation *const m_opacityAnimation; ///< 透明度动画
    qreal                     m_radius;           ///< 当前半径
    qreal                     m_opacity;          ///< 当前透明度
    QPoint                    m_center;           ///< 水波纹中心点
    QBrush                    m_brush;            ///< 画刷
};

inline void QtMaterialRipple::setOverlay(QtMaterialRippleOverlay *overlay)
{
    m_overlay = overlay; ///< 设置覆盖层
}

inline qreal QtMaterialRipple::radius() const
{
    return m_radius; ///< 返回半径
}

inline qreal QtMaterialRipple::opacity() const
{
    return m_opacity; ///< 返回透明度
}

inline QColor QtMaterialRipple::color() const
{
    return m_brush.color(); ///< 返回颜色
}

inline QBrush QtMaterialRipple::brush() const
{
    return m_brush; ///< 返回画刷
}

inline QPoint QtMaterialRipple::center() const
{
    return m_center; ///< 返回中心点
}

inline QPropertyAnimation *QtMaterialRipple::radiusAnimation() const
{
    return m_radiusAnimation; ///< 返回半径动画
}

inline QPropertyAnimation *QtMaterialRipple::opacityAnimation() const
{
    return m_opacityAnimation; ///< 返回透明度动画
}

inline void QtMaterialRipple::setOpacityStartValue(qreal value)
{
    m_opacityAnimation->setStartValue(value); ///< 设置透明度动画起始值
}

inline void QtMaterialRipple::setOpacityEndValue(qreal value)
{
    m_opacityAnimation->setEndValue(value); ///< 设置透明度动画结束值
}

inline void QtMaterialRipple::setRadiusStartValue(qreal value)
{
    m_radiusAnimation->setStartValue(value); ///< 设置半径动画起始值
}

inline void QtMaterialRipple::setRadiusEndValue(qreal value)
{
    m_radiusAnimation->setEndValue(value); ///< 设置半径动画结束值
}

inline void QtMaterialRipple::setDuration(int msecs)
{
    m_radiusAnimation->setDuration(msecs); ///< 设置半径动画持续时间
    m_opacityAnimation->setDuration(msecs); ///< 设置透明度动画持续时间
}

#endif // QTMATERIALRIPPLE_H