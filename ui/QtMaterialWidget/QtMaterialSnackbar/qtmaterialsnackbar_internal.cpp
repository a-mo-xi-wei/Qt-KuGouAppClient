/**
 * @file qtmaterialsnackbar_internal.cpp
 * @brief 实现 QtMaterialSnackbarStateMachine 类，提供提示条状态管理
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialsnackbar_internal.h"
#include "qtmaterialsnackbar.h"
#include "qtmaterialstatetransition.h"
#include <QPropertyAnimation>

/**
 * @brief 构造函数，初始化状态机
 * @param parent 指向 QtMaterialSnackbar 的指针
 * @note 内部实现
 */
QtMaterialSnackbarStateMachine::QtMaterialSnackbarStateMachine(QtMaterialSnackbar *parent)
    : QStateMachine(parent),
      m_snackbar(parent)
{
    m_timer.setSingleShot(true); ///< 设置单次定时器

    QState *hiddenState = new QState;  ///< 隐藏状态
    QState *visibleState = new QState; ///< 显示状态
    QState *finalState = new QState;   ///< 结束状态

    addState(hiddenState);
    addState(visibleState);
    addState(finalState);

    setInitialState(hiddenState); ///< 设置初始状态

    QtMaterialStateTransition *transition;

    // 显示转换
    transition = new QtMaterialStateTransition(SnackbarShowTransition);
    transition->setTargetState(visibleState);
    hiddenState->addTransition(transition);

    // 隐藏转换（从隐藏状态）
    transition = new QtMaterialStateTransition(SnackbarHideTransition);
    transition->setTargetState(visibleState);
    hiddenState->addTransition(transition);

    // 隐藏转换（从显示状态）
    transition = new QtMaterialStateTransition(SnackbarHideTransition);
    transition->setTargetState(finalState);
    visibleState->addTransition(transition);

    // 等待转换
    transition = new QtMaterialStateTransition(SnackbarWaitTransition);
    transition->setTargetState(hiddenState);
    finalState->addTransition(transition);

    // 下一条转换
    transition = new QtMaterialStateTransition(SnackbarNextTransition);
    transition->setTargetState(visibleState);
    finalState->addTransition(transition);

    // 连接信号
    connect(visibleState, SIGNAL(propertiesAssigned()),
            this, SLOT(snackbarShown())); ///< 显示完成
    connect(finalState, SIGNAL(propertiesAssigned()),
            m_snackbar, SLOT(dequeue())); ///< 结束时出队

    QPropertyAnimation *animation;

    // 偏移动画
    animation = new QPropertyAnimation(this, "offset", this);
    animation->setEasingCurve(QEasingCurve::OutCubic); ///< 缓动曲线
    animation->setDuration(300);                       ///< 动画时长
    addDefaultAnimation(animation);

    hiddenState->assignProperty(this, "offset", 1);  ///< 隐藏状态偏移
    visibleState->assignProperty(this, "offset", 0); ///< 显示状态无偏移
    finalState->assignProperty(this, "offset", 1);  ///< 结束状态偏移

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(progress())); ///< 定时器触发推进

    m_snackbar->installEventFilter(this); ///< 安装事件过滤器
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialSnackbarStateMachine::~QtMaterialSnackbarStateMachine()
{
}

/**
 * @brief 事件过滤
 * @param watched 监听对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialSnackbarStateMachine::eventFilter(QObject *watched, QEvent *event)
{
    if (QEvent::MouseButtonPress == event->type() && m_snackbar->clickToDismissMode()) {
        progress(); ///< 点击时推进状态
    }
    return QStateMachine::eventFilter(watched, event);
}

/**
 * @brief 设置垂直偏移
 * @param offset 偏移值
 */
void QtMaterialSnackbarStateMachine::setOffset(qreal offset)
{
    m_offset = offset;
    m_snackbar->update(); ///< 刷新界面
}

/**
 * @brief 推进状态
 */
void QtMaterialSnackbarStateMachine::progress()
{
    m_timer.stop(); ///< 停止定时器
    postEvent(new QtMaterialStateTransitionEvent(SnackbarHideTransition)); ///< 触发隐藏
    if (m_snackbar->clickToDismissMode()) {
        m_snackbar->setAttribute(Qt::WA_TransparentForMouseEvents, true); ///< 禁用鼠标事件
    }
}

/**
 * @brief 提示条显示完成
 */
void QtMaterialSnackbarStateMachine::snackbarShown()
{
    m_timer.start(m_snackbar->autoHideDuration()); ///< 启动定时器
    if (m_snackbar->clickToDismissMode()) {
        m_snackbar->setAttribute(Qt::WA_TransparentForMouseEvents, false); ///< 启用鼠标事件
    }
}