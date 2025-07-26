/**
* @file ElaPushButton.h
 * @brief 定义 ElaPushButton 类，提供自定义按钮功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAPUSHBUTTON_H
#define ELAPUSHBUTTON_H

#include <QPushButton>

#include "stdafx.h"

class ElaPushButtonPrivate;

/**
 * @class ElaPushButton
 * @brief 自定义按钮类，支持主题切换和颜色配置
 */
class ELA_EXPORT ElaPushButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaPushButton)
    Q_PROPERTY_CREATE_Q_H(int,    BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)

public:
    /**
     * @brief 构造函数，初始化按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaPushButton(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本的按钮
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaPushButton(QString text, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放按钮资源
     */
    ~ElaPushButton() override;

    /**
     * @brief 设置亮色主题文本颜色
     * @param color 文本颜色
     */
    void setLightTextColor(QColor color);

    /**
     * @brief 获取亮色主题文本颜色
     * @return 文本颜色
     */
    QColor getLightTextColor() const;

    /**
     * @brief 设置暗色主题文本颜色
     * @param color 文本颜色
     */
    void setDarkTextColor(QColor color);

    /**
     * @brief 获取暗色主题文本颜色
     * @return 文本颜色
     */
    QColor getDarkTextColor() const;

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

#endif // ELAPUSHBUTTON_H