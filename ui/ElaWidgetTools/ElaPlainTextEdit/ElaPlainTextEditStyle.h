/**
* @file ElaPlainTextEditStyle.h
 * @brief 定义 ElaPlainTextEditStyle 类，提供纯文本编辑框的自定义样式
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAPLAINTEXTEDITSTYLE_H
#define ELAPLAINTEXTEDITSTYLE_H

#include <QProxyStyle>

#include "Def.h"

/**
 * @class ElaPlainTextEditStyle
 * @brief 纯文本编辑框自定义样式类，管理边框和焦点指示器绘制
 */
class ElaPlainTextEditStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)

public:
    /**
     * @brief 构造函数，初始化自定义样式
     * @param style 基础样式，默认为 nullptr
     */
    explicit ElaPlainTextEditStyle(QStyle* style = nullptr);

    /**
     * @brief 析构函数，释放自定义样式资源
     */
    ~ElaPlainTextEditStyle() override;

    /**
     * @brief 绘制控件元素
     * @param element 控件元素
     * @param option 样式选项
     * @param painter 绘图对象
     * @param widget 目标控件，默认为 nullptr
     */
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;
};

#endif // ELAPLAINTEXTEDITSTYLE_H