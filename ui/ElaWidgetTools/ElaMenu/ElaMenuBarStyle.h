/**
* @file ElaMenuBarStyle.h
 * @brief 定义 ElaMenuBarStyle 类，提供自定义菜单栏样式
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMENUBARSTYLE_H
#define ELAMENUBARSTYLE_H

#include <QProxyStyle>

#include "Def.h"

/**
 * @class ElaMenuBarStyle
 * @brief 自定义菜单栏样式类，管理菜单栏的绘制和布局
 */
class ElaMenuBarStyle : public QProxyStyle
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化菜单栏样式
     * @param style 基础样式指针，默认为 nullptr
     */
    explicit ElaMenuBarStyle(QStyle* style = nullptr);

    /**
     * @brief 析构函数，释放菜单栏样式资源
     */
    ~ElaMenuBarStyle();

    /**
     * @brief 绘制基本图形元素
     * @param element 图形元素
     * @param option 样式选项
     * @param painter 绘制器
     * @param widget 控件指针
     */
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

    /**
     * @brief 绘制控件元素
     * @param element 控件元素
     * @param option 样式选项
     * @param painter 绘制器
     * @param widget 控件指针
     */
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

    /**
     * @brief 计算内容尺寸
     * @param type 内容类型
     * @param option 样式选项
     * @param size 原始尺寸
     * @param widget 控件指针
     * @return 计算后的尺寸
     */
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

    /**
     * @brief 获取像素度量值
     * @param metric 度量类型
     * @param option 样式选项
     * @param widget 控件指针
     * @return 像素度量值
     */
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;

    /**
     * @brief 菜单栏项边距
     */
    int                      _menuBarItemMargin{0};
};

#endif // ELAMENUBARSTYLE_H