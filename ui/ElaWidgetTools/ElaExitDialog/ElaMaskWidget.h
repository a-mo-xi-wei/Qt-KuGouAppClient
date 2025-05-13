/**
* @file ElaMaskWidget.h
 * @brief 定义 ElaMaskWidget 类，提供遮罩效果和动画
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMASKWIDGET_H
#define ELAMASKWIDGET_H

#include <QWidget>

#include "stdafx.h"

/**
 * @class ElaMaskWidget
 * @brief 遮罩控件类，提供透明遮罩效果和动画
 */
class ELA_EXPORT ElaMaskWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MaskAlpha)

public:
    /**
     * @brief 构造函数，初始化遮罩控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMaskWidget(QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放遮罩控件资源
     */
    ~ElaMaskWidget();

    /**
     * @brief 执行遮罩透明度动画
     * @param endValue 动画结束时的透明度值
     */
    void doMaskAnimation(int endValue);

protected:
    /**
     * @brief 重绘事件，绘制遮罩效果
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMASKWIDGET_H