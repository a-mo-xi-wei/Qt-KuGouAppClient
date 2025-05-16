/**
* @file qtmaterialstatetransition.h
 * @brief 定义 QtMaterialStateTransition 类，提供状态转换功能
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSTATETRANSITION_H
#define QTMATERIALSTATETRANSITION_H

#include <QAbstractTransition>
#include "qtmaterialstatetransitionevent.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class QtMaterialStateTransition
 * @brief 状态转换类，继承自 QAbstractTransition，支持 Material Design 控件状态管理
 */
class QTMATERIALWIDGET_EXPORT QtMaterialStateTransition : public QAbstractTransition
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化状态转换类型
     * @param type 状态转换类型
     */
    QtMaterialStateTransition(QtMaterialStateTransitionType type);

protected:
    /**
     * @brief 测试事件是否匹配状态转换
     * @param event 事件对象
     * @return 是否匹配
     */
    virtual bool eventTest(QEvent *event);

    /**
     * @brief 执行状态转换
     * @param event 事件对象
     */
    virtual void onTransition(QEvent *);

private:
    QtMaterialStateTransitionType m_type; ///< 状态转换类型
};

#endif // QTMATERIALSTATETRANSITION_H