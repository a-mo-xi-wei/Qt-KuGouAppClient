/**
 * @file qtmaterialfab_p.h
 * @brief 定义 QtMaterialFloatingActionButtonPrivate 类，提供 QtMaterialFloatingActionButton 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALFAB_P_H
#define QTMATERIALFAB_P_H

#include "qtmaterialraisedbutton_p.h"

// 前向声明
class QtMaterialFloatingActionButton;

/**
 * @class QtMaterialFloatingActionButtonPrivate
 * @brief QtMaterialFloatingActionButton 的私有实现类，管理按钮尺寸和位置
 * @note 内部实现
 */
class QtMaterialFloatingActionButtonPrivate : public QtMaterialRaisedButtonPrivate
{
    Q_DISABLE_COPY(QtMaterialFloatingActionButtonPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialFloatingActionButton)     ///< 声明公共类

public:
    /** @brief 按钮尺寸常量 */
    enum {
        DefaultDiameter = 56, ///< 标准直径
        MiniDiameter = 40     ///< 迷你直径
    };

    /** @brief 图标尺寸常量 */
    enum {
        DefaultIconSize = 24, ///< 标准图标尺寸
        MiniIconSize = 18     ///< 迷你图标尺寸
    };

    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialFloatingActionButton 的指针
     */
    QtMaterialFloatingActionButtonPrivate(QtMaterialFloatingActionButton *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialFloatingActionButtonPrivate();

    /**
     * @brief 初始化浮动动作按钮
     */
    void init();

    /**
     * @brief 计算按钮几何位置
     * @return 按钮的矩形区域
     */
    QRect fabGeometry() const;

    /**
     * @brief 配置阴影效果属性
     */
    void setupProperties();

    /**
     * @brief 获取按钮直径
     * @return 直径（标准或迷你）
     */
    inline int diameter() const;

    /**
     * @brief 获取图标尺寸
     * @return 图标尺寸（标准或迷你）
     */
    inline int iconSize() const;

    Qt::Corner corner; ///< 按钮所在角落
    bool       mini;   ///< 是否为迷你模式
    int        offsX;  ///< X 轴偏移
    int        offsY;  ///< Y 轴偏移
};

inline int QtMaterialFloatingActionButtonPrivate::diameter() const
{
    return mini ? MiniDiameter : DefaultDiameter;
}

inline int QtMaterialFloatingActionButtonPrivate::iconSize() const
{
    return mini ? MiniIconSize : DefaultIconSize;
}

#endif // QTMATERIALFAB_P_H