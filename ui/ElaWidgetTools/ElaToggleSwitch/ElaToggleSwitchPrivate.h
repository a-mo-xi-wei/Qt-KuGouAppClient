/**
* @file ElaToggleSwitchPrivate.h
 * @brief 定义 ElaToggleSwitchPrivate 类，开关控件的私有实现
 * @author [Liniyous]
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef ELATOGGLESWITCHPRIVATE_H
#define ELATOGGLESWITCHPRIVATE_H

#include "Def.h"
#include "stdafx.h"

class ElaToggleSwitch;

/**
 * @class ElaToggleSwitchPrivate
 * @brief 开关控件的私有实现，管理状态和动画
 */
class ElaToggleSwitchPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaToggleSwitch); ///< 声明控件指针

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaToggleSwitchPrivate(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaToggleSwitchPrivate() override;

private:
    bool _isToggled{false};                                       ///< 开关状态
    int _margin{1};                                               ///< 边距
    qreal _circleCenterX{0};                                      ///< 滑块圆心 X 坐标
    qreal _circleRadius{0};                                       ///< 滑块半径
    bool _isLeftButtonPress{false};                               ///< 左键按下状态
    bool _isMousePressMove{false};                                ///< 鼠标拖动状态
    int _lastMouseX{0};                                           ///< 上次鼠标 X 坐标
    ElaThemeType::ThemeMode _themeMode;                           ///< 主题模式

    /**
     * @brief 启动位置动画
     * @param startX 起始 X 坐标
     * @param endX 结束 X 坐标
     * @param isToggle 目标开关状态
     */
    void _startPosAnimation(qreal startX, qreal endX, bool isToggle);

    /**
     * @brief 启动半径动画
     * @param startRadius 起始半径
     * @param endRadius 结束半径
     */
    void _startRadiusAnimation(qreal startRadius, qreal endRadius);

    /**
     * @brief 调整圆心 X 坐标
     * @note 限制在开关边界内
     */
    void _adjustCircleCenterX();
};

#endif // ELATOGGLESWITCHPRIVATE_H