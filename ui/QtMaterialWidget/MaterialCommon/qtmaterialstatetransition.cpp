/**
* @file qtmaterialstatetransition.cpp
 * @brief 实现 QtMaterialStateTransition 类，提供状态转换功能
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialstatetransition.h"

/**
 * @brief 构造函数，初始化状态转换类型
 * @param type 状态转换类型
 */
QtMaterialStateTransition::QtMaterialStateTransition(QtMaterialStateTransitionType type)
    : m_type(type)
{
}

/**
 * @brief 测试事件是否匹配状态转换
 * @param event 事件对象
 * @return 是否匹配
 */
bool QtMaterialStateTransition::eventTest(QEvent *event)
{
    if (event->type() != QEvent::Type(QEvent::User + 1)) {
        return false; ///< 非用户事件
    }
    QtMaterialStateTransitionEvent *transition = static_cast<QtMaterialStateTransitionEvent *>(event);
    return (m_type == transition->type); ///< 匹配转换类型
}

/**
 * @brief 执行状态转换
 * @param event 事件对象
 */
void QtMaterialStateTransition::onTransition(QEvent *)
{
    // 虚函数，子类可重写以实现具体转换逻辑
}