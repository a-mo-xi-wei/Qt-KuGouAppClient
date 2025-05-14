/**
 * @file ElaScrollBarStyle.h
 * @brief 定义 ElaScrollBarStyle 类，提供自定义滚动条样式
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELASCROLLBARSTYLE_H
#define ELASCROLLBARSTYLE_H

#include <QProxyStyle>

#include "Def.h"

class ElaScrollBar;

/**
 * @class ElaScrollBarStyle
 * @brief 自定义滚动条样式类，管理滚动条的绘制和动画
 */
class ElaScrollBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool,        IsExpand)
    Q_PROPERTY_CREATE(qreal,      Opacity)
    Q_PROPERTY_CREATE(qreal,      SliderExtent)
    Q_PRIVATE_CREATE(ElaScrollBar*, ScrollBar)

public:
    /**
     * @brief 构造函数，初始化自定义样式
     * @param style 基础样式，默认为 nullptr
     */
    explicit ElaScrollBarStyle(QStyle* style = nullptr);

    /**
     * @brief 析构函数，释放自定义样式资源
     */
    ~ElaScrollBarStyle();

    /**
     * @brief 绘制复杂控件
     * @param control 控件类型
     * @param option 样式选项
     * @param painter 绘图对象
     * @param widget 目标控件，默认为 nullptr
     */
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;

    /**
     * @brief 获取像素度量值
     * @param metric 度量类型
     * @param option 样式选项，默认为 nullptr
     * @param widget 目标控件，默认为 nullptr
     * @return 度量值
     */
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    /**
     * @brief 获取样式提示
     * @param hint 提示类型
     * @param option 样式选项，默认为 nullptr
     * @param widget 目标控件，默认为 nullptr
     * @param returnData 返回数据，默认为 nullptr
     * @return 提示值
     */
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

    /**
     * @brief 启动展开动画
     * @param isExpand 是否展开
     */
    void startExpandAnimation(bool isExpand);

private:
    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;

    /**
     * @brief 滑块边距
     */
    qreal                    _sliderMargin{2.5};

    /**
     * @brief 滚动条宽度
     */
    int                      _scrollBarExtent{10};
};

#endif // ELASCROLLBARSTYLE_H