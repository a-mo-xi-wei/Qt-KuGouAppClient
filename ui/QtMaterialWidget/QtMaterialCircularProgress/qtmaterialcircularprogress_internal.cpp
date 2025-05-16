/**
* @file qtmaterialcircularprogress_internal.cpp
 * @brief 实现 QtMaterialCircularProgressDelegate 类，提供圆形进度条动画代理
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialcircularprogress_internal.h"

/**
 * @brief 构造函数，初始化动画代理
 * @param parent 指向 QtMaterialCircularProgress 的指针
 * @note 内部实现
 */
QtMaterialCircularProgressDelegate::QtMaterialCircularProgressDelegate(QtMaterialCircularProgress *parent)
    : QObject(parent),
      m_progress(parent),
      m_dashOffset(0),
      m_dashLength(89),
      m_angle(0)
{
    Q_ASSERT(parent); ///< 确保父对象有效
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialCircularProgressDelegate::~QtMaterialCircularProgressDelegate()
{
}