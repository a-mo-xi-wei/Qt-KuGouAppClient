/**
 * @file qtmaterialraisedbutton.cpp
 * @brief 实现 QtMaterialRaisedButton 类，提供 Material Design 凸起按钮
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialraisedbutton.h"
#include "qtmaterialraisedbutton_p.h"

#include <QStateMachine>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEventTransition>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialRaisedButton 的指针
 * @note 内部实现
 */
QtMaterialRaisedButtonPrivate::QtMaterialRaisedButtonPrivate(QtMaterialRaisedButton *q)
    : QtMaterialFlatButtonPrivate(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialRaisedButtonPrivate::~QtMaterialRaisedButtonPrivate()
{
}

/**
 * @brief 初始化凸起按钮
 */
void QtMaterialRaisedButtonPrivate::init()
{
    Q_Q(QtMaterialRaisedButton);

    shadowStateMachine = new QStateMachine(q); ///< 创建阴影状态机
    normalState        = new QState;           ///< 正常状态
    pressedState       = new QState;           ///< 按下状态
    effect             = new QGraphicsDropShadowEffect; ///< 阴影效果

    effect->setBlurRadius(7);                 ///< 初始模糊半径
    effect->setOffset(QPointF(0, 2));         ///< 初始偏移
    effect->setColor(QColor(0, 0, 0, 75));    ///< 阴影颜色

    q->setBackgroundMode(Qt::OpaqueMode);     ///< 设置不透明背景
    q->setMinimumHeight(42);                  ///< 设置最小高度
    q->setGraphicsEffect(effect);             ///< 应用阴影效果
    q->setBaseOpacity(0.3);                   ///< 设置基础透明度

    shadowStateMachine->addState(normalState);
    shadowStateMachine->addState(pressedState);

    normalState->assignProperty(effect, "offset", QPointF(0, 2)); ///< 正常状态偏移
    normalState->assignProperty(effect, "blurRadius", 7);         ///< 正常状态模糊半径

    pressedState->assignProperty(effect, "offset", QPointF(0, 5)); ///< 按下状态偏移
    pressedState->assignProperty(effect, "blurRadius", 29);       ///< 按下状态模糊半径

    QAbstractTransition *transition;

    // 鼠标按下转换
    transition = new QEventTransition(q, QEvent::MouseButtonPress);
    transition->setTargetState(pressedState);
    normalState->addTransition(transition);

    // 鼠标双击转换
    transition = new QEventTransition(q, QEvent::MouseButtonDblClick);
    transition->setTargetState(pressedState);
    normalState->addTransition(transition);

    // 鼠标释放转换
    transition = new QEventTransition(q, QEvent::MouseButtonRelease);
    transition->setTargetState(normalState);
    pressedState->addTransition(transition);

    QPropertyAnimation *animation;

    animation = new QPropertyAnimation(effect, "offset", q);
    animation->setDuration(100); ///< 偏移动画时长
    shadowStateMachine->addDefaultAnimation(animation);

    animation = new QPropertyAnimation(effect, "blurRadius", q);
    animation->setDuration(100); ///< 模糊半径动画时长
    shadowStateMachine->addDefaultAnimation(animation);

    shadowStateMachine->setInitialState(normalState);
    shadowStateMachine->start(); ///< 启动状态机
}

/**
 * @brief 构造函数，初始化凸起按钮
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialRaisedButton::QtMaterialRaisedButton(QWidget *parent)
    : QtMaterialFlatButton(*new QtMaterialRaisedButtonPrivate(this), parent)
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 构造函数，初始化凸起按钮
 * @param text 按钮文本
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialRaisedButton::QtMaterialRaisedButton(const QString &text, QWidget *parent)
    : QtMaterialFlatButton(*new QtMaterialRaisedButtonPrivate(this), parent)
{
    d_func()->init(); ///< 初始化私有类

    setText(text);    ///< 设置文本
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialRaisedButton::~QtMaterialRaisedButton()
{
}

/**
 * @brief 构造函数，初始化凸起按钮（私有）
 * @param d 私有类引用
 * @param parent 父控件指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialRaisedButton::QtMaterialRaisedButton(QtMaterialRaisedButtonPrivate &d, QWidget *parent)
    : QtMaterialFlatButton(d, parent)
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 处理事件
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialRaisedButton::event(QEvent *event)
{
    Q_D(QtMaterialRaisedButton);

    if (QEvent::EnabledChange == event->type()) {
        if (isEnabled()) {
            d->shadowStateMachine->start(); ///< 启用时启动状态机
            d->effect->setEnabled(true);    ///< 启用阴影效果
        } else {
            d->shadowStateMachine->stop();  ///< 禁用时停止状态机
            d->effect->setEnabled(false);   ///< 禁用阴影效果
        }
    }
    return QtMaterialFlatButton::event(event);
}