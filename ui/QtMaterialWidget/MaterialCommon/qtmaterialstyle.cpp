/**
* @file qtmaterialstyle.cpp
 * @brief 实现 QtMaterialStyle 类，提供 Material Design 风格支持
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialstyle.h"
#include "qtmaterialtheme.h"

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialStyle 的指针
 * @note 内部实现
 */
QtMaterialStylePrivate::QtMaterialStylePrivate(QtMaterialStyle *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialStylePrivate::~QtMaterialStylePrivate()
{
}

/**
 * @brief 初始化私有类，创建默认主题
 */
void QtMaterialStylePrivate::init()
{
    Q_Q(QtMaterialStyle);

    q->setTheme(new QtMaterialTheme); ///< 创建并设置默认主题
}

/**
 * @brief 设置 Material Design 主题
 * @param theme 主题对象指针
 * @note 内部实现
 */
void QtMaterialStyle::setTheme(QtMaterialTheme *theme)
{
    Q_D(QtMaterialStyle);

    d->theme = theme;
    theme->setParent(this); ///< 设置主题的父对象
}

/**
 * @brief 获取主题颜色
 * @param key 颜色键值
 * @return 对应颜色
 */
QColor QtMaterialStyle::themeColor(const QString &key) const
{
    Q_D(const QtMaterialStyle);

    Q_ASSERT(d->theme);

    return d->theme->getColor(key); ///< 返回主题中的颜色
}

/**
 * @brief 构造函数，初始化 Material Design 风格
 */
QtMaterialStyle::QtMaterialStyle()
    : QCommonStyle(),
      d_ptr(new QtMaterialStylePrivate(this))
{
    d_func()->init(); ///< 初始化私有类
}