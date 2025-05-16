/**
 * @file qtmaterialflatbutton_p.h
 * @brief 定义 QtMaterialFlatButtonPrivate 类，提供 QtMaterialFlatButton 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALFLATBUTTON_P_H
#define QTMATERIALFLATBUTTON_P_H

#include <QtGlobal>
#include <QColor>
#include "qtmaterialtheme.h"

// 前向声明
class QtMaterialFlatButton;
class QtMaterialRippleOverlay;
class QtMaterialFlatButtonStateMachine;

/**
 * @class QtMaterialFlatButtonPrivate
 * @brief QtMaterialFlatButton 的私有实现类，管理按钮样式和交互
 * @note 内部实现
 */
class QtMaterialFlatButtonPrivate
{
    Q_DISABLE_COPY(QtMaterialFlatButtonPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialFlatButton)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialFlatButton 的指针
     */
    QtMaterialFlatButtonPrivate(QtMaterialFlatButton *q);

    /**
     * @brief 析构函数，清理资源
     */
    virtual ~QtMaterialFlatButtonPrivate();

    /**
     * @brief 初始化扁平按钮
     */
    void init();

    QtMaterialFlatButton             *const q_ptr;               ///< 指向公共类
    QtMaterialRippleOverlay          *rippleOverlay;             ///< 水波纹覆盖层
    QtMaterialFlatButtonStateMachine *stateMachine;              ///< 状态机
    Material::Role                    role;                      ///< 按钮角色
    Material::RippleStyle             rippleStyle;               ///< 水波纹样式
    Material::ButtonIconPlacement     iconPlacement;             ///< 图标位置
    Material::OverlayStyle            overlayStyle;              ///< 覆盖层样式
    Qt::BGMode                        bgMode;                    ///< 背景模式
    Qt::Alignment                     textAlignment;             ///< 文本对齐
    QColor                            backgroundColor;           ///< 背景色
    QColor                            foregroundColor;           ///< 前景色
    QColor                            overlayColor;              ///< 覆盖层颜色
    QColor                            disabledColor;             ///< 禁用前景色
    QColor                            disabledBackgroundColor;   ///< 禁用背景色
    qreal                             fixedRippleRadius;         ///< 固定水波纹半径
    qreal                             cornerRadius;              ///< 圆角半径
    qreal                             baseOpacity;               ///< 基础透明度
    qreal                             fontSize;                  ///< 字体大小
    bool                              useThemeColors;            ///< 是否使用主题颜色
    bool                              useFixedRippleRadius;      ///< 是否使用固定水波纹半径
    bool                              haloVisible;               ///< 光晕是否可见
};

#endif // QTMATERIALFLATBUTTON_P_H