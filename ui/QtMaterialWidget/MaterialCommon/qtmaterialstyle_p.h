/**
* @file qtmaterialstyle_p.h
 * @brief 定义 QtMaterialStylePrivate 类，提供 QtMaterialStyle 的私有实现
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSTYLE_P_H
#define QTMATERIALSTYLE_P_H

// 前向声明
class QtMaterialStyle;
class QtMaterialTheme;

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class QtMaterialStylePrivate
 * @brief QtMaterialStyle 的私有实现类，管理主题和初始化
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialStylePrivate
{
    Q_DISABLE_COPY(QtMaterialStylePrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialStyle)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialStyle 的指针
     */
    QtMaterialStylePrivate(QtMaterialStyle *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialStylePrivate();

    /**
     * @brief 初始化私有类，创建默认主题
     */
    void init();

    QtMaterialStyle *const q_ptr; ///< 指向公共类的指针
    QtMaterialTheme *theme;       ///< 主题对象
};

#endif // QTMATERIALSTYLE_P_H