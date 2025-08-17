/**
* @file CheckBox1.h
 * @brief 定义 CheckBox1 类，提供带动画的复选框功能
 * @author [iwxyi]
 * @date 2025-05-15
 * @version 1.0
 * @note 此处参考开源项目：iwxyi 大神的 [Qt控件] 带动画的 CheckBox，项目地址为：https://github.com/iwxyi/Qt-AniCheckBox，非常实用，有兴趣的可以 star
 */

#ifndef CHECKBOX1_H
#define CHECKBOX1_H
/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt控件] 带动画的 CheckBox
 *  项目地址为 ： https://github.com/iwxyi/Qt-AniCheckBox
 *  非常实用，有兴趣的可以star
 */

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

#include "AniCheckBox.h"

/**
 * @class CheckBox1
 * @brief 自定义复选框类，继承自 AniCheckBox，支持动画效果
 */
class UIWIDGETS_EXPORT CheckBox1 : public AniCheckBox
{
public:
    /**
     * @brief 构造函数，初始化复选框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit CheckBox1(QWidget* parent = nullptr);

    /**
     * @brief 设置未选中状态的颜色
     * @param c 颜色值
     */
    void setUncheckedColor(QColor c);

protected:
    /**
     * @brief 鼠标进入事件，触发悬浮动画
     * @param e 进入事件
     */
    void enterEvent(QEnterEvent *e) override;

    /**
     * @brief 鼠标离开事件，触发离开动画
     * @param e 事件
     */
    void leaveEvent(QEvent *e) override;

    /**
     * @brief 复选框状态变化处理
     * @param state 复选框状态
     */
    void checkStateChanged(const int& state) override;

    /**
     * @brief 绘制复选框
     * @param painter 绘图对象
     * @param rect 绘制区域
     */
    void drawBox(QPainter &painter, QRectF rect) override;

protected:
    QColor uncheckedColor = QColor(0x88, 0x88, 0x88, 0x88); ///< 未选中状态颜色
};

#endif // CHECKBOX1_H