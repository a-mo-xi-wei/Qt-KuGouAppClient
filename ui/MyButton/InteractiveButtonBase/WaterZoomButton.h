/**
* @file WaterZoomButton.h
 * @brief 定义 WaterZoomButton 类，提供具有缩放和水波纹效果的按钮
 * @author WeiWang
 * @date 2025-02-06
 * @version 1.0
 * @note 基于 InteractiveButtonBase 类扩展，支持动态缩放和 choking 效果
 */

#ifndef WATERZOOMBUTTON_H
#define WATERZOOMBUTTON_H

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

#include "InteractiveButtonBase.h"

/**
 * @class WaterZoomButton
 * @brief 具有缩放和水波纹效果的按钮类，继承自 InteractiveButtonBase
 */
class MYBUTTON_EXPORT WaterZoomButton : public InteractiveButtonBase
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化 WaterZoomButton
     * @param parent 父控件指针，默认为 nullptr
     * @param text 按钮显示的文本，默认为空
     */
    explicit WaterZoomButton(QWidget *parent = nullptr, const QString& text = "");

    /**
     * @brief 设置 choking 值（向内缩进的长度）
     * @param c choking 的固定值
     */
    void setChoking(const int& c);

    /**
     * @brief 根据比例设置 choking 值
     * @param p choking 比例，基于按钮最小尺寸
     */
    void setChokingProp(const double& p);

    /**
     * @brief 设置缩放时的圆角半径
     * @param radius 缩放时的圆角半径值
     */
    void setRadiusZoom(const int& radius);

    /**
     * @brief 设置按钮的圆角半径
     * @param x 绘制时的圆角半径
     * @param x2 缩放时的圆角半径
     * @note 仅使用 x 参数绘制，忽略 y 参数
     */
    void setRadius(const int& x, const int& x2);

protected:
    /**
     * @brief 获取背景绘制路径，支持动态缩放和 choking 效果
     * @return QPainterPath 背景绘制路径
     */
    QPainterPath getBgPainterPath() override;

    /**
     * @brief 处理控件大小调整事件，更新 choking 值
     * @param event 大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;

protected:
    int                     choking;        ///< 向内缩进的长度
    double                  choking_prop;   ///< choking 比例，基于按钮最小尺寸
    int                     radius_zoom;    ///< 缩放时的圆角半径
};

#endif // WATERZOOMBUTTON_H