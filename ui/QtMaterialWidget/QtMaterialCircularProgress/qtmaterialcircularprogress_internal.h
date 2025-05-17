/**
 * @file qtmaterialcircularprogress_internal.h
 * @brief 定义 QtMaterialCircularProgressDelegate 类，提供圆形进度条动画代理
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALCIRCULARPROGRESS_INTERNAL_H
#define QTMATERIALCIRCULARPROGRESS_INTERNAL_H

#include "qtmaterialcircularprogress.h"

/**
 * @class QtMaterialCircularProgressDelegate
 * @brief 圆形进度条动画代理类，管理虚线偏移、长度和旋转角度
 * @note 内部实现
 */
class QtMaterialCircularProgressDelegate : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal dashOffset WRITE setDashOffset READ dashOffset) ///< 虚线偏移属性
    Q_PROPERTY(qreal dashLength WRITE setDashLength READ dashLength) ///< 虚线长度属性
    Q_PROPERTY(int angle WRITE setAngle READ angle) ///< 旋转角度属性

public:
    /**
     * @brief 构造函数，初始化动画代理
     * @param parent 指向 QtMaterialCircularProgress 的指针
     */
    QtMaterialCircularProgressDelegate(QtMaterialCircularProgress *parent);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialCircularProgressDelegate();

    /**
     * @brief 设置虚线偏移
     * @param offset 虚线偏移
     */
    inline void setDashOffset(qreal offset);

    /**
     * @brief 获取虚线偏移
     * @return 虚线偏移
     */
    inline qreal dashOffset() const;

    /**
     * @brief 设置虚线长度
     * @param length 虚线长度
     */
    inline void setDashLength(qreal length);

    /**
     * @brief 获取虚线长度
     * @return 虚线长度
     */
    inline qreal dashLength() const;

    /**
     * @brief 设置旋转角度
     * @param angle 旋转角度
     */
    inline void setAngle(int angle);

    /**
     * @brief 获取旋转角度
     * @return 旋转角度
     */
    inline int angle() const;

private:
    Q_DISABLE_COPY(QtMaterialCircularProgressDelegate) ///< 禁用拷贝

    QtMaterialCircularProgress *const m_progress;      ///< 关联进度条
    qreal                             m_dashOffset;   ///< 虚线偏移
    qreal                             m_dashLength;   ///< 虚线长度
    int                               m_angle;        ///< 旋转角度
};

inline void QtMaterialCircularProgressDelegate::setDashOffset(qreal offset)
{
    m_dashOffset = offset;
    m_progress->update(); ///< 刷新进度条界面
}

inline qreal QtMaterialCircularProgressDelegate::dashOffset() const
{
    return m_dashOffset;
}

inline void QtMaterialCircularProgressDelegate::setDashLength(qreal length)
{
    m_dashLength = length;
    m_progress->update(); ///< 刷新进度条界面
}

inline qreal QtMaterialCircularProgressDelegate::dashLength() const
{
    return m_dashLength;
}

inline void QtMaterialCircularProgressDelegate::setAngle(int angle)
{
    m_angle = angle;
    m_progress->update(); ///< 刷新进度条界面
}

inline int QtMaterialCircularProgressDelegate::angle() const
{
    return m_angle;
}

#endif // QTMATERIALCIRCULARPROGRESS_INTERNAL_H