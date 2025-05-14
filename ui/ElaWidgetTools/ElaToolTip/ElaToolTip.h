/**
* @file ElaToolTip.h
 * @brief 定义 ElaToolTip 类，提供自定义工具提示功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATOOLTIP_H
#define ELATOOLTIP_H

#include <QWidget>

#include "stdafx.h"

class ElaToolTipPrivate;

/**
 * @class ElaToolTip
 * @brief 自定义工具提示类，支持文本、自定义控件和动画
 */
class ELA_EXPORT ElaToolTip : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaToolTip)
    Q_PROPERTY_CREATE_Q_H(int,      BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int,      DisplayMsec)
    Q_PROPERTY_CREATE_Q_H(int,      ShowDelayMsec)
    Q_PROPERTY_CREATE_Q_H(int,      HideDelayMsec)
    Q_PROPERTY_CREATE_Q_H(QString,  ToolTip)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CustomWidget)

public:
    /**
     * @brief 构造函数，初始化工具提示
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaToolTip(QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放工具提示资源
     */
    ~ElaToolTip() override;

    /**
     * @brief 设置工具提示始终显示
     */
    void setToolTipShowAlways();

    /**
     * @brief 更新工具提示位置
     */
    void updatePos();

protected:
    /**
     * @brief 绘制事件，绘制工具提示外观
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELATOOLTIP_H