/**
 * @file qtmaterialrippleoverlay.h
 * @brief 定义 QtMaterialRippleOverlay 类，提供水波纹覆盖层功能
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALRIPPLEOVERLAY_H
#define QTMATERIALRIPPLEOVERLAY_H

#include <QPainterPath>
#include "qtmaterialoverlaywidget.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialRipple;

/**
 * @class QtMaterialRippleOverlay
 * @brief 水波纹覆盖层类，继承自 QtMaterialOverlayWidget，管理多个水波纹效果
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialRippleOverlay : public QtMaterialOverlayWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化水波纹覆盖层
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialRippleOverlay(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialRippleOverlay() override;

    /**
     * @brief 添加水波纹对象
     * @param ripple 水波纹对象指针
     */
    void addRipple(QtMaterialRipple *ripple);

    /**
     * @brief 添加水波纹效果
     * @param position 水波纹中心点
     * @param radius 水波纹结束半径，默认为 300
     */
    void addRipple(const QPoint &position, qreal radius = 300);

    /**
     * @brief 移除水波纹对象
     * @param ripple 水波纹对象指针
     */
    void removeRipple(QtMaterialRipple *ripple);

    /**
     * @brief 设置是否启用裁剪
     * @param enable 是否启用
     */
    inline void setClipping(bool enable);

    /**
     * @brief 检查是否启用裁剪
     * @return 是否启用裁剪
     */
    inline bool hasClipping() const;

    /**
     * @brief 设置裁剪路径
     * @param path 裁剪路径
     */
    inline void setClipPath(const QPainterPath &path);

protected:
    /**
     * @brief 重写绘制事件，绘制水波纹效果
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 获取水波纹对象列表
     * @return 水波纹对象列表
     */
    inline QList<QtMaterialRipple *> ripples() const;

private:
    Q_DISABLE_COPY(QtMaterialRippleOverlay) ///< 禁用拷贝

    /**
     * @brief 绘制单个水波纹
     * @param painter 画家对象
     * @param ripple 水波纹对象
     */
    void paintRipple(QPainter *painter, QtMaterialRipple *ripple);

    QList<QtMaterialRipple *> m_ripples; ///< 水波纹对象列表
    QPainterPath              m_clipPath; ///< 裁剪路径
    bool                      m_useClip;  ///< 裁剪标志
};

inline void QtMaterialRippleOverlay::setClipping(bool enable)
{
    m_useClip = enable; ///< 设置裁剪标志
    update(); ///< 触发重绘
}

inline bool QtMaterialRippleOverlay::hasClipping() const
{
    return m_useClip; ///< 返回裁剪标志
}

inline void QtMaterialRippleOverlay::setClipPath(const QPainterPath &path)
{
    m_clipPath = path; ///< 设置裁剪路径
    update(); ///< 触发重绘
}

inline QList<QtMaterialRipple *> QtMaterialRippleOverlay::ripples() const
{
    return m_ripples; ///< 返回水波纹列表
}

#endif // QTMATERIALRIPPLEOVERLAY_H