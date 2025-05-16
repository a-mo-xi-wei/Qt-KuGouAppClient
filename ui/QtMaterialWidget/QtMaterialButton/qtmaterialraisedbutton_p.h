/**
* @file qtmaterialraisedbutton_p.h
 * @brief 定义 QtMaterialRaisedButtonPrivate 类，提供 QtMaterialRaisedButton 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALRAISEDBUTTON_P_H
#define QTMATERIALRAISEDBUTTON_P_H

#include "qtmaterialflatbutton_p.h"

// 前向声明
class QStateMachine;
class QState;
class QGraphicsDropShadowEffect;
class QtMaterialRaisedButton;

/**
 * @class QtMaterialRaisedButtonPrivate
 * @brief QtMaterialRaisedButton 的私有实现类，管理阴影效果和状态转换
 * @note 内部实现
 */
class QtMaterialRaisedButtonPrivate : public QtMaterialFlatButtonPrivate
{
    Q_DISABLE_COPY(QtMaterialRaisedButtonPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialRaisedButton)      ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialRaisedButton 的指针
     */
    QtMaterialRaisedButtonPrivate(QtMaterialRaisedButton *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialRaisedButtonPrivate();

    /**
     * @brief 初始化凸起按钮
     */
    void init();

    QStateMachine             *shadowStateMachine; ///< 阴影状态机
    QState                    *normalState;        ///< 正常状态
    QState                    *pressedState;       ///< 按下状态
    QGraphicsDropShadowEffect *effect;             ///< 阴影效果
};

#endif // QTMATERIALRAISEDBUTTON_P_H