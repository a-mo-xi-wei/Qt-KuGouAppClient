/**
* @file qtmaterialstyle.h
 * @brief 定义 QtMaterialStyle 类，提供 Material Design 风格支持
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSTYLE_H
#define QTMATERIALSTYLE_H

#include <QCommonStyle>
#include "qtmaterialstyle_p.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/** @brief 禁用主题颜色宏，关闭主题颜色使用 */
#define MATERIAL_DISABLE_THEME_COLORS \
if (d->useThemeColors == true) { d->useThemeColors = false; }

// 前向声明
class QtMaterialTheme;

/**
 * @class QtMaterialStyle
 * @brief Material Design 风格类，继承自 QCommonStyle，管理主题和颜色
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialStyle : public QCommonStyle
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     * @return QtMaterialStyle 实例引用
     */
    inline static QtMaterialStyle &instance();

    /**
     * @brief 析构函数，清理资源
     */
    virtual ~QtMaterialStyle() override;

    /**
     * @brief 设置 Material Design 主题
     * @param theme 主题对象指针
     */
    void setTheme(QtMaterialTheme *theme);

    /**
     * @brief 获取主题颜色
     * @param key 颜色键值
     * @return 对应颜色
     */
    QColor themeColor(const QString &key) const;

protected:
    const QScopedPointer<QtMaterialStylePrivate> d_ptr; ///< 私有实现指针

private:
    Q_DECLARE_PRIVATE(QtMaterialStyle) ///< 声明私有类

    /**
     * @brief 构造函数，初始化 Material Design 风格
     */
    QtMaterialStyle();

    Q_DISABLE_COPY(QtMaterialStyle) ///< 禁用拷贝
};

inline QtMaterialStyle::~QtMaterialStyle()
{
}

inline QtMaterialStyle &QtMaterialStyle::instance()
{
    static QtMaterialStyle instance;
    return instance; ///< 返回单例实例
}

#endif // QTMATERIALSTYLE_H