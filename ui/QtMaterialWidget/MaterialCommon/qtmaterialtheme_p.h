/**
* @file qtmaterialtheme_p.h
 * @brief 定义 QtMaterialThemePrivate 类，提供 QtMaterialTheme 的私有实现
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALTHEME_P_H
#define QTMATERIALTHEME_P_H

#include <QHash>

// 前向声明
class QtMaterialTheme;

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class QtMaterialThemePrivate
 * @brief QtMaterialTheme 的私有实现类，管理颜色映射
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialThemePrivate
{
    Q_DISABLE_COPY(QtMaterialThemePrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialTheme)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialTheme 的指针
     */
    QtMaterialThemePrivate(QtMaterialTheme *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialThemePrivate();

    /**
     * @brief 构造 RGBA 颜色
     * @param r 红色分量
     * @param g 绿色分量
     * @param b 蓝色分量
     * @param a 透明度（0.0 到 1.0）
     * @return 构造的颜色对象
     */
    QColor rgba(int r, int g, int b, qreal a) const;

    QtMaterialTheme *const q_ptr;       ///< 指向公共类的指针
    QHash<QString, QColor> colors;      ///< 颜色键值映射
};

#endif // QTMATERIALTHEME_P_H