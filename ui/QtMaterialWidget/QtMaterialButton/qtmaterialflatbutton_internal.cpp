/**
 * @file qtmaterialflatbutton_internal.cpp
 * @brief 实现 QtMaterialFlatButtonStateMachine 类，提供扁平按钮状态管理
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialflatbutton_internal.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialstatetransition.h"

#include <QEventTransition>
#include <QPropertyAnimation>
#include <QFocusEvent>
#include <QSequentialAnimationGroup>

/**
 * @brief 构造函数，初始化状态机
 * @param parent 指向 QtMaterialFlatButton 的指针
 * @note 内部实现
 */
QtMaterialFlatButtonStateMachine::QtMaterialFlatButtonStateMachine(QtMaterialFlatButton *parent)
    : QStateMachine(parent),
      m_button(parent),
      m_topLevelState(new QState(QState::ParallelStates)),
      m_configState(new QState(m_topLevelState)),
      m_checkableState(new QState(m_topLevelState)),
      m_checkedState(new QState(m_checkableState)),
      m_uncheckedState(new QState(m_checkableState)),
      m_neutralState(new QState(m_configState)),
      m_neutralFocusedState(new QState(m_configState)),
      m_hoveredState(new QState(m_configState)),
      m_hoveredFocusedState(new QState(m_configState)),
      m_pressedState(new QState(m_configState)),
      m_haloAnimation(new QSequentialAnimationGroup(this)),
      m_overlayOpacity(0),
      m_checkedOverlayProgress(parent->isChecked() ? 1 : 0),
      m_haloOpacity(0),
      m_haloSize(0.8),
      m_haloScaleFactor(1),
      m_wasChecked(false)
{
    Q_ASSERT(parent);

    parent->installEventFilter(this); ///< 安装事件过滤器

    m_configState->setInitialState(m_neutralState); ///< 设置初始配置状态
    addState(m_topLevelState);
    setInitialState(m_topLevelState);

    m_checkableState->setInitialState(parent->isChecked() ? m_checkedState
                                                          : m_uncheckedState); ///< 设置初始选中状态

    QtMaterialStateTransition *transition;
    QPropertyAnimation *animation;

    // 选中转换
    transition = new QtMaterialStateTransition(FlatButtonCheckedTransition);
    transition->setTargetState(m_checkedState);
    m_uncheckedState->addTransition(transition);

    animation = new QPropertyAnimation(this, "checkedOverlayProgress", this);
    animation->setDuration(200); ///< 选中动画时长
    transition->addAnimation(animation);

    // 未选中转换
    transition = new QtMaterialStateTransition(FlatButtonUncheckedTransition);
    transition->setTargetState(m_uncheckedState);
    m_checkedState->addTransition(transition);

    animation = new QPropertyAnimation(this, "checkedOverlayProgress", this);
    animation->setDuration(200); ///< 未选中动画时长
    transition->addAnimation(animation);

    // 状态转换：中立 -> 中立焦点
    addTransition(m_button, QEvent::FocusIn, m_neutralState, m_neutralFocusedState);
    // 状态转换：中立焦点 -> 中立
    addTransition(m_button, QEvent::FocusOut, m_neutralFocusedState, m_neutralState);
    // 状态转换：中立 -> 悬停
    addTransition(m_button, QEvent::Enter, m_neutralState, m_hoveredState);
    // 状态转换：悬停 -> 中立
    addTransition(m_button, QEvent::Leave, m_hoveredState, m_neutralState);
    // 状态转换：中立焦点 -> 悬停焦点
    addTransition(m_button, QEvent::Enter, m_neutralFocusedState, m_hoveredFocusedState);
    // 状态转换：悬停焦点 -> 中立焦点
    addTransition(m_button, QEvent::Leave, m_hoveredFocusedState, m_neutralFocusedState);
    // 状态转换：悬停 -> 悬停焦点
    addTransition(m_button, QEvent::FocusIn, m_hoveredState, m_hoveredFocusedState);
    // 状态转换：悬停焦点 -> 悬停
    addTransition(m_button, QEvent::FocusOut, m_hoveredFocusedState, m_hoveredState);

    // 按下转换
    transition = new QtMaterialStateTransition(FlatButtonPressedTransition);
    transition->setTargetState(m_pressedState);
    m_hoveredState->addTransition(transition);

    // 状态转换：按下 -> 中立焦点
    addTransition(m_button, QEvent::Leave, m_pressedState, m_neutralFocusedState);
    // 状态转换：按下 -> 悬停
    addTransition(m_button, QEvent::FocusOut, m_pressedState, m_hoveredState);

    // 设置状态属性
    m_neutralState->assignProperty(this, "haloSize", 0);         ///< 中立状态无光晕
    m_neutralFocusedState->assignProperty(this, "haloSize", 0.7); ///< 中立焦点状态光晕尺寸
    m_hoveredState->assignProperty(this, "haloSize", 0);         ///< 悬停状态无光晕
    m_pressedState->assignProperty(this, "haloSize", 4);         ///< 按下状态光晕尺寸
    m_hoveredFocusedState->assignProperty(this, "haloSize", 0.7); ///< 悬停焦点状态光晕尺寸

    // 光晕缩放动画
    QPropertyAnimation *grow = new QPropertyAnimation(this);
    QPropertyAnimation *shrink = new QPropertyAnimation(this);

    grow->setTargetObject(this);
    grow->setPropertyName("haloScaleFactor");
    grow->setStartValue(0.56); ///< 起始缩放因子
    grow->setEndValue(0.63);   ///< 终止缩放因子
    grow->setEasingCurve(QEasingCurve::InOutSine); ///< 缓动曲线
    grow->setDuration(840);    ///< 动画时长

    shrink->setTargetObject(this);
    shrink->setPropertyName("haloScaleFactor");
    shrink->setStartValue(0.63); ///< 起始缩放因子
    shrink->setEndValue(0.56);   ///< 终止缩放因子
    shrink->setEasingCurve(QEasingCurve::InOutSine); ///< 缓动曲线
    shrink->setDuration(840);    ///< 动画时长

    m_haloAnimation->addAnimation(grow);
    m_haloAnimation->addAnimation(shrink);
    m_haloAnimation->setLoopCount(-1); ///< 无限循环
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialFlatButtonStateMachine::~QtMaterialFlatButtonStateMachine()
{
}

/**
 * @brief 设置覆盖层透明度
 * @param opacity 覆盖层透明度
 */
void QtMaterialFlatButtonStateMachine::setOverlayOpacity(qreal opacity)
{
    m_overlayOpacity = opacity;
    m_button->update(); ///< 刷新按钮界面
}

/**
 * @brief 设置选中覆盖层进度
 * @param progress 选中进度（0 到 1）
 */
void QtMaterialFlatButtonStateMachine::setCheckedOverlayProgress(qreal progress)
{
    m_checkedOverlayProgress = progress;
    m_button->update(); ///< 刷新按钮界面
}


/**
 * @brief 设置光晕透明度
 * @param opacity 光晕透明度
 */
void QtMaterialFlatButtonStateMachine::setHaloOpacity(qreal opacity)
{
    m_haloOpacity = opacity;
    m_button->update(); ///< 刷新按钮界面
}

/**
 * @brief 设置光晕尺寸
 * @param size 光晕尺寸
 */
void QtMaterialFlatButtonStateMachine::setHaloSize(qreal size)
{
    m_haloSize = size;
    m_button->update(); ///< 刷新按钮界面
}

/**
 * @brief 设置光晕缩放因子
 * @param factor 缩放因子
 */
void QtMaterialFlatButtonStateMachine::setHaloScaleFactor(qreal factor)
{
    m_haloScaleFactor = factor;
    m_button->update(); ///< 刷新按钮界面
}

/**
 * @brief 启动动画和状态机
 */
void QtMaterialFlatButtonStateMachine::startAnimations()
{
    m_haloAnimation->start(); ///< 启动光晕动画
    start();                  ///< 启动状态机
}

/**
 * @brief 配置状态属性
 */
void QtMaterialFlatButtonStateMachine::setupProperties()
{
    QColor overlayColor;

    if (Qt::TransparentMode == m_button->backgroundMode()) {
        overlayColor = m_button->backgroundColor(); ///< 透明模式使用背景色
    } else {
        overlayColor = m_button->foregroundColor(); ///< 不透明模式使用前景色
    }

    const qreal baseOpacity = m_button->baseOpacity(); ///< 基础透明度

    // 设置状态属性
    m_neutralState->assignProperty(this, "overlayOpacity", 0); ///< 中立状态无覆盖层
    m_neutralState->assignProperty(this, "haloOpacity", 0);    ///< 中立状态无光晕
    m_neutralFocusedState->assignProperty(this, "overlayOpacity", 0); ///< 中立焦点状态无覆盖层
    m_neutralFocusedState->assignProperty(this, "haloOpacity", baseOpacity); ///< 中立焦点状态光晕
    m_hoveredState->assignProperty(this, "overlayOpacity", baseOpacity); ///< 悬停状态覆盖层
    m_hoveredState->assignProperty(this, "haloOpacity", 0);    ///< 悬停状态无光晕
    m_hoveredFocusedState->assignProperty(this, "overlayOpacity", baseOpacity); ///< 悬停焦点状态覆盖层
    m_hoveredFocusedState->assignProperty(this, "haloOpacity", baseOpacity); ///< 悬停焦点状态光晕
    m_pressedState->assignProperty(this, "overlayOpacity", baseOpacity); ///< 按下状态覆盖层
    m_pressedState->assignProperty(this, "haloOpacity", 0);    ///< 按下状态无光晕
    m_checkedState->assignProperty(this, "checkedOverlayProgress", 1); ///< 选中状态覆盖层
    m_uncheckedState->assignProperty(this, "checkedOverlayProgress", 0); ///< 未选中状态无覆盖层

    m_button->update(); ///< 刷新按钮界面
}

/**
 * @brief 更新选中状态
 */
void QtMaterialFlatButtonStateMachine::updateCheckedStatus()
{
    const bool checked = m_button->isChecked();
    if (m_wasChecked != checked) {
        m_wasChecked = checked;
        if (checked) {
            postEvent(new QtMaterialStateTransitionEvent(FlatButtonCheckedTransition)); ///< 触发选中转换
        } else {
            postEvent(new QtMaterialStateTransitionEvent(FlatButtonUncheckedTransition)); ///< 触发未选中转换
        }
    }
}

/**
 * @brief 事件过滤器
 * @param watched 观察对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialFlatButtonStateMachine::eventFilter(QObject *watched, QEvent *event)
{
    if (QEvent::FocusIn == event->type()) {
        QFocusEvent *focusEvent = static_cast<QFocusEvent *>(event);
        if (focusEvent && Qt::MouseFocusReason == focusEvent->reason()) {
            postEvent(new QtMaterialStateTransitionEvent(FlatButtonPressedTransition)); ///< 鼠标焦点触发按下
            return true;
        }
    }
    return QStateMachine::eventFilter(watched, event);
}

/**
 * @brief 添加事件转换
 * @param object 触发对象
 * @param eventType 事件类型
 * @param fromState 起始状态
 * @param toState 目标状态
 */
void QtMaterialFlatButtonStateMachine::addTransition(QObject *object, QEvent::Type eventType, QState *fromState, QState *toState)
{
    addTransition(new QEventTransition(object, eventType), fromState, toState);
}

/**
 * @brief 添加抽象转换
 * @param transition 转换对象
 * @param fromState 起始状态
 * @param toState 目标状态
 */
void QtMaterialFlatButtonStateMachine::addTransition(QAbstractTransition *transition, QState *fromState, QState *toState)
{
    transition->setTargetState(toState);

    QPropertyAnimation *animation;

    animation = new QPropertyAnimation(this, "overlayOpacity", this);
    animation->setDuration(150); ///< 覆盖层动画时长
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(this, "haloOpacity", this);
    animation->setDuration(170); ///< 光晕透明度动画时长
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(this, "haloSize", this);
    animation->setDuration(350); ///< 光晕尺寸动画时长
    animation->setEasingCurve(QEasingCurve::OutCubic); ///< 缓动曲线
    transition->addAnimation(animation);

    fromState->addTransition(transition);
}