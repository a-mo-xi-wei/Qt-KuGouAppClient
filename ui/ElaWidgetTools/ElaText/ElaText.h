/**
* @file ElaText.h
 * @brief 定义 ElaText 类，提供自定义文本标签功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATEXT_H
#define ELATEXT_H

#include <QLabel>

#include "Def.h"
#include "stdafx.h"

class ElaTextPrivate;

/**
 * @class ElaText
 * @brief 自定义文本标签类，支持主题切换、图标和文本样式
 */
class ELA_EXPORT ElaText : public QLabel
{
    Q_OBJECT
    Q_Q_CREATE(ElaText)
    Q_PROPERTY_CREATE_Q_H(bool,                    IsWrapAnywhere)
    Q_PROPERTY_CREATE_Q_H(int,                     TextPixelSize)
    Q_PROPERTY_CREATE_Q_H(int,                     TextPointSize)
    Q_PROPERTY_CREATE_Q_H(ElaTextType::TextStyle,  TextStyle)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName,   ElaIcon)

public:
    /**
     * @brief 构造函数，初始化文本标签
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaText(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本的文本标签
     * @param text 文本内容
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaText(QString text, QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本和像素大小的文本标签
     * @param text 文本内容
     * @param pixelSize 字体像素大小
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaText(QString text, int pixelSize, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放文本标签资源
     */
    ~ElaText() override;

protected:
    /**
     * @brief 绘制事件，绘制文本或图标
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELATEXT_H