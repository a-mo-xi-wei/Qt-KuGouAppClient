/**
* @file ElaExitDialogPrivate.h
 * @brief 定义 ElaExitDialogPrivate 类，管理退出对话框的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELACONTENTDIALOGPRIVATE_H
#define ELACONTENTDIALOGPRIVATE_H

#include <QPointer>

#include "Def.h"
#include "stdafx.h"

class QVBoxLayout;
class ElaPushButton;
class ElaExitDialog;
class ElaMaskWidget;

/**
 * @class ElaExitDialogPrivate
 * @brief 退出对话框私有实现类，管理对话框的控件和动画
 */
class ElaExitDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExitDialog)

public:
    /**
     * @brief 构造函数，初始化退出对话框私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaExitDialogPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放退出对话框私有资源
     */
    ~ElaExitDialogPrivate() override;

private:
    /**
     * @brief 当前窗口 ID
     */
    qint64                      _currentWinID{0};

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode    _themeMode;

    /**
     * @brief 遮罩控件指针
     */
    QPointer<ElaMaskWidget>    _maskWidget{nullptr};

    /**
     * @brief 中心控件指针
     */
    QWidget*                   _centralWidget{nullptr};

    /**
     * @brief 按钮容器控件指针
     */
    QWidget*                   _buttonWidget{nullptr};

    /**
     * @brief 主布局指针
     */
    QVBoxLayout*               _mainLayout{nullptr};

    /**
     * @brief 左侧按钮文本
     */
    QString                    _leftButtonText{"cancel"};

    /**
     * @brief 中间按钮文本
     */
    QString                    _middleButtonText{"minimum"};

    /**
     * @brief 右侧按钮文本
     */
    QString                    _rightButtonText{"exit"};

    /**
     * @brief 左侧按钮指针
     */
    ElaPushButton*             _leftButton{nullptr};

    /**
     * @brief 中间按钮指针
     */
    ElaPushButton*             _middleButton{nullptr};

    /**
     * @brief 右侧按钮指针
     */
    ElaPushButton*             _rightButton{nullptr};

    /**
     * @brief 执行关闭动画
     */
    void _doCloseAnimation();
};

#endif // ELACONTENTDIALOGPRIVATE_H