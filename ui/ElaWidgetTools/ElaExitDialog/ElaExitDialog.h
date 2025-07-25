/**
 * @file ElaExitDialog.h
 * @brief 定义 ElaExitDialog 类，提供退出对话框功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELACONTENTDIALOG_H
#define ELACONTENTDIALOG_H

#include <QDialog>

#include "stdafx.h"

class ElaExitDialogPrivate;

/**
 * @class ElaExitDialog
 * @brief 退出对话框类，提供退出、最小化和取消选项
 */
class ELA_EXPORT ElaExitDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaExitDialog)

public:
    /**
     * @brief 构造函数，初始化退出对话框
     * @param parent 父控件指针
     */
    explicit ElaExitDialog(QWidget* parent);

    /**
     * @brief 析构函数，释放退出对话框资源
     */
    ~ElaExitDialog() override;

    /**
     * @brief 设置中心控件
     * @param centralWidget 中心控件指针
     */
    void setCentralWidget(QWidget* centralWidget);

    /**
     * @brief 设置左侧按钮文本
     * @param text 按钮文本
     */
    void setLeftButtonText(QString text);

    /**
     * @brief 设置中间按钮文本
     * @param text 按钮文本
     */
    void setMiddleButtonText(QString text);

    /**
     * @brief 设置右侧按钮文本
     * @param text 按钮文本
     */
    void setRightButtonText(QString text);

    /**
     * @brief 关闭对话框
     */
    void close();

public Q_SLOTS:
    /**
     * @brief 左侧按钮点击槽函数
     */
    virtual void onLeftButtonClicked();

    /**
     * @brief 中间按钮点击槽函数
     */
    virtual void onMiddleButtonClicked();

    /**
     * @brief 右侧按钮点击槽函数
     */
    virtual void onRightButtonClicked();

Q_SIGNALS:
    /**
     * @brief 左侧按钮点击信号
     */
    void leftButtonClicked();

    /**
     * @brief 中间按钮点击信号
     */
    void middleButtonClicked();

    /**
     * @brief 右侧按钮点击信号
     */
    void rightButtonClicked();

protected:
    /**
     * @brief 显示事件处理
     * @param event 显示事件
     */
    void showEvent(QShowEvent* event) override;

    /**
     * @brief 重绘事件，绘制对话框背景和按钮区域
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent* event) override;

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    /**
     * @brief 本地事件处理（Windows 平台）
     * @param eventType 事件类型
     * @param message 事件消息
     * @param result 事件结果
     * @return 是否处理事件
     */
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    /**
     * @brief 本地事件处理（Windows 平台，Qt5）
     * @param eventType 事件类型
     * @param message 事件消息
     * @param result 事件结果
     * @return 是否处理事件
     */
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};

#endif // ELACONTENTDIALOG_H