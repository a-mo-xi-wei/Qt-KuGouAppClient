/**
 * @file ElaToggleSwitchPrivate.cpp
 * @brief 实现 ElaToggleSwitchPrivate 类，开关控件的私有实现
 * @author [Liniyous]
 * @date 2024-12-15
 * @version 1.0
 */

#include "ElaToggleSwitchPrivate.h"
#include "ElaToggleSwitch.h"

#include <QPropertyAnimation>

/**
 * @brief 构造函数
 * @param parent 父对象指针，默认为 nullptr
 */
ElaToggleSwitchPrivate::ElaToggleSwitchPrivate(QObject *parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数
 */
ElaToggleSwitchPrivate::~ElaToggleSwitchPrivate()
= default;

/**
 * @brief 启动位置动画
 * @param startX 起始 X 坐标
 * @param endX 结束 X 坐标
 * @param isToggle 目标开关状态
 * @note 使用 InOutSine 缓动曲线，动画结束时自动删除
 */
void ElaToggleSwitchPrivate::_startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    Q_Q(ElaToggleSwitch);
    QPropertyAnimation *circleAnimation = new QPropertyAnimation(q, "circleCenterX"); ///< 创建位置动画
    connect(circleAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value) {
        this->_circleCenterX = value.toReal();                    ///< 更新圆心 X 坐标
        q->update();                                              ///< 触发重绘
    });                                                           ///< 连接动画值变化信号
    circleAnimation->setStartValue(startX);                       ///< 设置起始值
    circleAnimation->setEndValue(endX);                           ///< 设置结束值
    circleAnimation->setEasingCurve(QEasingCurve::InOutSine);     ///< 设置缓动曲线
    circleAnimation->start(QAbstractAnimation::DeleteWhenStopped); ///< 启动动画并自动删除
    _isToggled = isToggle;                                        ///< 更新开关状态
    Q_EMIT q->toggled(isToggle);                                  ///< 发射状态变化信号
}

/**
 * @brief 启动半径动画
 * @param startRadius 起始半径
 * @param endRadius 结束半径
 * @note 使用 InOutSine 缓动曲线，动画结束时自动删除
 */
void ElaToggleSwitchPrivate::_startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    Q_Q(ElaToggleSwitch);
    QPropertyAnimation *circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius"); ///< 创建半径动画
    connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value) {
        this->_circleRadius = value.toReal();                     ///< 更新圆半径
        q->update();                                              ///< 触发重绘
    });                                                           ///< 连接动画值变化信号
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine); ///< 设置缓动曲线
    circleRadiusAnimation->setStartValue(startRadius);            ///< 设置起始值
    circleRadiusAnimation->setEndValue(endRadius);                ///< 设置结束值
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped); ///< 启动动画并自动删除
}

/**
 * @brief 调整圆心 X 坐标
 * @note 限制圆心在开关边界内
 */
void ElaToggleSwitchPrivate::_adjustCircleCenterX()
{
    Q_Q(ElaToggleSwitch);
    if (_circleCenterX > q->width() - q->height() / 2 - _margin * 2)
    {
        _circleCenterX = q->width() - q->height() / 2 - _margin * 2; ///< 限制右边界
    }
    if (_circleCenterX < q->height() / 2)
    {
        _circleCenterX = q->height() / 2;                         ///< 限制左边界
    }
}