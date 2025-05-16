/**
* @file qtmaterialstatetransitionevent.h
 * @brief 定义 QtMaterialStateTransitionEvent 类和状态转换类型枚举
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSTATETRANSITIONEVENT_H
#define QTMATERIALSTATETRANSITIONEVENT_H

#include <QEvent>

/**
 * @brief 状态转换类型枚举，定义 Material Design 控件的状态转换
 */
enum QtMaterialStateTransitionType {
    SnackbarShowTransition     = 1,    ///< Snackbar 显示
    SnackbarHideTransition     = 2,    ///< Snackbar 隐藏
    SnackbarWaitTransition     = 3,    ///< Snackbar 等待
    SnackbarNextTransition     = 4,    ///< Snackbar 下一个
    FlatButtonPressedTransition = 5,   ///< 扁平按钮按下
    FlatButtonCheckedTransition = 6,   ///< 扁平按钮选中
    FlatButtonUncheckedTransition = 7, ///< 扁平按钮取消选中
    CollapsibleMenuExpand      = 8,    ///< 可折叠菜单展开
    CollapsibleMenuCollapse    = 9,    ///< 可折叠菜单折叠
    SliderChangedToMinimum     = 10,   ///< 滑块设置为最小值
    SliderChangedFromMinimum   = 11,   ///< 滑块从最小值改变
    SliderNoFocusMouseEnter    = 12,   ///< 滑块无焦点鼠标进入
    SliderNoFocusMouseLeave    = 13,   ///< 滑块无焦点鼠标离开
    DialogShowTransition       = 14,   ///< 对话框显示
    DialogHideTransition       = 15,   ///< 对话框隐藏
    MaxTransitionType          = 65535 ///< 最大转换类型
};

/**
 * @class QtMaterialStateTransitionEvent
 * @brief 状态转换事件类，继承自 QEvent，携带转换类型
 */
struct QtMaterialStateTransitionEvent : public QEvent
{
    /**
     * @brief 构造函数，初始化状态转换事件
     * @param type 状态转换类型
     */
    QtMaterialStateTransitionEvent(QtMaterialStateTransitionType type)
        : QEvent(QEvent::Type(QEvent::User + 1)),
          type(type)
    {
    }

    QtMaterialStateTransitionType type; ///< 状态转换类型
};

#endif // QTMATERIALSTATETRANSITIONEVENT_H