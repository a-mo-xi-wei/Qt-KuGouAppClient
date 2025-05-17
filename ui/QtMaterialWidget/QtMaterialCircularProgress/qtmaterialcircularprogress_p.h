/**
* @file qtmaterialcircularprogress_p.h
 * @brief 定义 QtMaterialCircularProgressPrivate 类，提供 QtMaterialCircularProgress 的私有实现
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALCIRCULARPROGRESS_P_H
#define QTMATERIALCIRCULARPROGRESS_P_H

#include <QtGlobal>
#include "qtmaterialtheme.h"

// 前向声明
class QtMaterialCircularProgress;
class QtMaterialCircularProgressDelegate;

/**
 * @class QtMaterialCircularProgressPrivate
 * @brief QtMaterialCircularProgress 的私有实现类，管理进度条属性和动画
 * @note 内部实现
 */
class QtMaterialCircularProgressPrivate
{
    Q_DISABLE_COPY(QtMaterialCircularProgressPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialCircularProgress)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialCircularProgress 的指针
     */
    QtMaterialCircularProgressPrivate(QtMaterialCircularProgress *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialCircularProgressPrivate();

    /**
     * @brief 初始化圆形进度条
     */
    void init();

    QtMaterialCircularProgress         *const q_ptr;        ///< 指向公共类
    QtMaterialCircularProgressDelegate *delegate;           ///< 动画代理
    Material::ProgressType              progressType;       ///< 进度条类型
    QColor                              color;              ///< 进度条颜色
    qreal                               penWidth;           ///< 线宽
    int                                 size;               ///< 尺寸
    bool                                useThemeColors;     ///< 是否使用主题颜色
};

#endif // QTMATERIALCIRCULARPROGRESS_P_H