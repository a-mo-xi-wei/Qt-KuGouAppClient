/**
 * @file ElaToggleSwitch.cpp
 * @brief 实现 ElaToggleSwitch 类，开关控件
 * @author [Liniyous]
 * @date 2024-12-15
 * @version 1.0
 */

#include "ElaToggleSwitch.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "ElaToggleSwitchPrivate.h"

/**
 * @brief 构造函数，初始化开关控件
 * @param parent 父控件指针，默认为 nullptr
 * @note 设置固定尺寸为 44x22，启用鼠标追踪
 */
ElaToggleSwitch::ElaToggleSwitch(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaToggleSwitchPrivate())
{
    Q_D(ElaToggleSwitch);
    d->q_ptr = this;                                              ///< 设置私有类指针
    setObjectName("ElaToggleSwitch");                             ///< 设置对象名称
    setMouseTracking(true);                                       ///< 启用鼠标追踪
    setFixedSize(44, 22);                                         ///< 设置固定尺寸
    d->_circleCenterX = -1;                                       ///< 初始化圆心 X 坐标
    d->_isToggled = false;                                        ///< 初始化开关状态
    d->_themeMode = eTheme->getThemeMode();                       ///< 获取当前主题模式
    setProperty("circleCenterX", 0.01);                           ///< 设置圆心 X 属性
    setProperty("circleRadius", 0.01);                            ///< 设置圆半径属性
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;                                ///< 更新主题模式
    });                                                           ///< 连接主题变化信号
}

/**
 * @brief 析构函数
 */
ElaToggleSwitch::~ElaToggleSwitch()
= default;

/**
 * @brief 设置开关状态
 * @param isToggled 是否开启
 * @note 启动位置动画
 */
void ElaToggleSwitch::setIsToggled(bool isToggled)
{
    Q_D(ElaToggleSwitch);
    if (d->_isToggled == isToggled)
    {
        return;                                                   ///< 状态未变化，直接返回
    }
    if (d->_isToggled)
    {
        d->_startPosAnimation(width() - height() / 2 - d->_margin * 2, height() / 2, isToggled); ///< 从开启到关闭动画
    }
    else
    {
        d->_startPosAnimation(height() / 2, width() - height() / 2 - d->_margin * 2, isToggled); ///< 从关闭到开启动画
    }
}

/**
 * @brief 获取开关状态
 * @return 当前开关状态
 */
bool ElaToggleSwitch::getIsToggled() const
{
    return d_ptr->_isToggled;                                     ///< 返回开关状态
}

/**
 * @brief 事件处理
 * @param event 事件
 * @return 是否处理事件
 * @note 处理鼠标进入、离开和移动事件
 */
bool ElaToggleSwitch::event(QEvent* event)
{
    Q_D(ElaToggleSwitch);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.3, height() * 0.35); ///< 鼠标进入时放大半径
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.35, height() * 0.3); ///< 鼠标离开时缩小半径
        }
        break;
    }
    case QEvent::MouseMove:
    {
        update();                                                 ///< 更新绘制
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);                                 ///< 调用父类处理
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 记录按下状态并启动半径动画
 */
void ElaToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    d->_adjustCircleCenterX();                                    ///< 调整圆心位置
    d->_isLeftButtonPress = true;                                 ///< 标记左键按下
    d->_lastMouseX = event->pos().x();                            ///< 记录鼠标 X 坐标
    d->_startRadiusAnimation(d->_circleRadius, height() * 0.25);  ///< 启动半径缩小动画
    QWidget::mousePressEvent(event);                              ///< 调用父类处理
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 根据位置启动位置动画并恢复半径
 */
void ElaToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    d->_isLeftButtonPress = false;                                ///< 清除左键按下状态
    QWidget::mouseReleaseEvent(event);                            ///< 调用父类处理
    if (d->_isMousePressMove)
    {
        d->_isMousePressMove = false;                             ///< 清除鼠标移动状态
        if (d->_circleCenterX > width() / 2)
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true); ///< 滑动到开启位置
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false); ///< 滑动到关闭位置
        }
    }
    else
    {
        if (d->_isToggled)
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false); ///< 点击切换到关闭
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true); ///< 点击切换到开启
        }
    }
    d->_startRadiusAnimation(height() * 0.25, height() * 0.35);   ///< 恢复半径动画
}

/**
 * @brief 鼠标移动事件
 * @param event 鼠标事件
 * @note 更新圆心位置
 */
void ElaToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaToggleSwitch);
    if (d->_isLeftButtonPress)
    {
        d->_isMousePressMove = true;                              ///< 标记鼠标移动
        int moveX = event->pos().x() - d->_lastMouseX;            ///< 计算 X 偏移
        d->_lastMouseX = event->pos().x();                        ///< 更新鼠标 X 坐标
        d->_circleCenterX += moveX;                               ///< 更新圆心 X 坐标
        d->_adjustCircleCenterX();                                ///< 限制圆心位置
    }
    QWidget::mouseMoveEvent(event);                               ///< 调用父类处理
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制背景圆角矩形和圆形滑块
 */
void ElaToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_D(ElaToggleSwitch);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); ///< 启用抗锯齿
    // @note 背景绘制
    painter.setPen(d->_isToggled ? QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1.5) : QPen(ElaThemeColor(d->_themeMode, BasicBorderDeep), 1.5)); ///< 设置边框颜色
    // @note 未使用，保留用于调试
    // painter.setBrush(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, PrimaryNormal) : (underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase)) : ElaThemeColor(d->_themeMode, BasicDisable));
    painter.setBrush(isEnabled() ? d->_isToggled ? QColor(38,161,225) : (underMouse() ? ElaThemeColor(d->_themeMode, BasicHover) : ElaThemeColor(d->_themeMode, BasicBase)) : ElaThemeColor(d->_themeMode, BasicDisable)); ///< 设置背景颜色
    QPainterPath path;
    path.moveTo(width() - height() - d->_margin, height() - d->_margin); ///< 移动到右上角
    path.arcTo(QRectF(QPointF(width() - height() - d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), -90, 180); ///< 绘制右半圆
    path.lineTo(height() / 2 + d->_margin, d->_margin);           ///< 连接到左上角
    path.arcTo(QRectF(QPointF(d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), 90, 180); ///< 绘制左半圆
    path.lineTo(width() - height() - d->_margin, height() - d->_margin); ///< 闭合路径
    path.closeSubpath();
    painter.drawPath(path);                                       ///< 绘制背景路径

    // @note 圆心绘制
    painter.setPen(Qt::NoPen);                                    ///< 无边框
    painter.setBrush(isEnabled() ? d->_isToggled ? ElaThemeColor(d->_themeMode, BasicTextInvert) : ElaThemeColor(d->_themeMode, ToggleSwitchNoToggledCenter) : ElaThemeColor(d->_themeMode, BasicTextDisable)); ///< 设置滑块颜色
    if (d->_circleRadius == 0)
    {
        d->_circleRadius = this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3; ///< 初始化半径
    }
    if (d->_isLeftButtonPress)
    {
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius); ///< 绘制拖动中的滑块
    }
    else
    {
        if (d->_circleCenterX == -1)
        {
            d->_circleCenterX = d->_isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2; ///< 初始化圆心位置
        }
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius); ///< 绘制静止滑块
    }
    painter.restore();                                            ///< 恢复画笔状态
}