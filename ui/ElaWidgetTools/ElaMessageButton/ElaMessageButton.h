/**
* @file ElaMessageButton.h
 * @brief 定义 ElaMessageButton 类，提供消息触发按钮功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMESSAGEBUTTON_H
#define ELAMESSAGEBUTTON_H

#include <QPushButton>
#include <QWidget>

#include "Def.h"

class ElaMessageButtonPrivate;

/**
 * @class ElaMessageButton
 * @brief 消息触发按钮类，用于触发消息通知栏
 */
class ELA_EXPORT ElaMessageButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageButton)
    Q_PROPERTY_CREATE_Q_H(int,                              BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString,                         BarTitle)
    Q_PROPERTY_CREATE_Q_H(QString,                         BarText)
    Q_PROPERTY_CREATE_Q_H(int,                             DisplayMsec)
    Q_PROPERTY_CREATE_Q_H(QWidget*,                        MessageTargetWidget)
    Q_PROPERTY_CREATE_Q_H(ElaMessageBarType::MessageMode,  MessageMode)
    Q_PROPERTY_CREATE_Q_H(ElaMessageBarType::PositionPolicy, PositionPolicy)

public:
    /**
     * @brief 构造函数，初始化消息按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMessageButton(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本的消息按钮
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMessageButton(const QString& text, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放消息按钮资源
     */
    ~ElaMessageButton();

protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件对象
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 绘制事件，绘制按钮外观
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMESSAGEBUTTON_H