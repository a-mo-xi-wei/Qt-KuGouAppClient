/**
 * @file ElaIconButton.h
 * @brief 定义 ElaIconButton 类，提供自定义图标按钮功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAICONBUTTON_H
#define ELAICONBUTTON_H

#include <QPushButton>

#include "Def.h"

class ElaIconButtonPrivate;

/**
 * @class ElaIconButton
 * @brief 自定义图标按钮类，支持图标、透明度和悬停效果
 */
class ELA_EXPORT ElaIconButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaIconButton)
    Q_PROPERTY_CREATE_Q_H(int,    BorderRadius)
    Q_PROPERTY_CREATE_Q_H(qreal,  Opacity)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightIconColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkIconColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverIconColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverIconColor)
    Q_PROPERTY_CREATE_Q_H(bool,   IsSelected)

public:
    /**
     * @brief 构造函数，初始化带图像的图标按钮
     * @param pix 按钮图像
     * @param parent 父控件指针，默认为 nullptr
     */
    ElaIconButton(QPixmap pix, QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带 Ela 图标的按钮
     * @param awesome Ela 图标类型
     * @param parent 父控件指针，默认为 nullptr
     */
    ElaIconButton(ElaIconType::IconName awesome, QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带 Ela 图标和像素大小的按钮
     * @param awesome Ela 图标类型
     * @param pixelSize 图标像素大小
     * @param parent 父控件指针，默认为 nullptr
     */
    ElaIconButton(ElaIconType::IconName awesome, int pixelSize, QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带 Ela 图标、像素大小和固定尺寸的按钮
     * @param awesome Ela 图标类型
     * @param pixelSize 图标像素大小
     * @param fixedWidth 按钮固定宽度
     * @param fixedHeight 按钮固定高度
     * @param parent 父控件指针，默认为 nullptr
     */
    ElaIconButton(ElaIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放图标按钮资源
     */
    ~ElaIconButton();

    /**
     * @brief 设置 Ela 图标
     * @param awesome Ela 图标类型
     */
    void setAwesome(ElaIconType::IconName awesome);

    /**
     * @brief 获取当前 Ela 图标
     * @return 当前 Ela 图标类型
     */
    ElaIconType::IconName getAwesome() const;

    /**
     * @brief 设置按钮图像
     * @param pix 按钮图像
     */
    void setPixmap(QPixmap pix);

protected:
    /**
     * @brief 事件处理
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool event(QEvent* event) override;

    /**
     * @brief 绘制事件，绘制按钮外观
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAICONBUTTON_H