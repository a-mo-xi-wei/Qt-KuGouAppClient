/**
* @file qtmaterialdialog_p.h
 * @brief 定义 QtMaterialDialogPrivate 类，提供 QtMaterialDialog 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALDIALOG_P_H
#define QTMATERIALDIALOG_P_H

#include <QtGlobal>

// 前向声明
class QStateMachine;
class QtMaterialDialog;
class QStackedLayout;
class QtMaterialDialogWindow;
class QtMaterialDialogProxy;

/**
 * @class QtMaterialDialogPrivate
 * @brief QtMaterialDialog 的私有实现类，管理状态机和布局
 * @note 内部实现
 */
class QtMaterialDialogPrivate
{
    Q_DISABLE_COPY(QtMaterialDialogPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialDialog)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialDialog 的指针
     */
    QtMaterialDialogPrivate(QtMaterialDialog *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialDialogPrivate();

    /**
     * @brief 初始化对话框
     */
    void init();

    QtMaterialDialog       *const q_ptr;         ///< 指向公共类
    QtMaterialDialogWindow *dialogWindow;        ///< 对话框窗口
    QStackedLayout         *proxyStack;          ///< 堆叠布局
    QStateMachine          *stateMachine;        ///< 状态机
    QtMaterialDialogProxy  *proxy;               ///< 代理窗口
};

#endif // QTMATERIALDIALOG_P_H