/**
* @file ElaExitDialogPrivate.cpp
 * @brief 实现 ElaExitDialogPrivate 类，管理退出对话框的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaExitDialogPrivate.h"
#include "ElaExitDialog.h"
#include "ElaMaskWidget.h"

/**
 * @brief 构造函数，初始化退出对话框私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaExitDialogPrivate::ElaExitDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放退出对话框私有资源
 */
ElaExitDialogPrivate::~ElaExitDialogPrivate() = default;
/**
 * @brief 执行关闭动画
 */
void ElaExitDialogPrivate::_doCloseAnimation()
{
    Q_Q(ElaExitDialog);
    _maskWidget->doMaskAnimation(0);
    q->QDialog::close();
}