/**
 * @file WaterZoomButton.cpp
 * @brief 实现 WaterZoomButton 类，提供具有缩放和水波纹效果的按钮功能
 * @author WeiWang
 * @date 2025-02-06
 * @version 1.0
 * @note 基于 InteractiveButtonBase 类扩展，支持动态缩放和 choking 效果
 */

#include "WaterZoomButton.h"
#include <QPainterPath>

/**
 * @brief 构造函数，初始化 WaterZoomButton
 * @param parent 父控件指针，默认为 nullptr
 * @param text 按钮显示的文本，默认为空
 */
WaterZoomButton::WaterZoomButton(QWidget *parent, const QString& text)
    : InteractiveButtonBase(text, parent)
    , choking(10)       ///< 向里缩的长度，初始值为 10
    , radius_zoom(-1)   ///< 缩放时的圆角半径，初始值为 -1
    , choking_prop(0)   ///< choking 比例，初始值为 0
{
}

/**
 * @brief 设置 choking 值（向内缩进的长度）
 * @param c choking 的固定值
 */
void WaterZoomButton::setChoking(const int& c)
{
    choking = c;
}

/**
 * @brief 根据比例设置 choking 值
 * @param p choking 比例，基于按钮最小尺寸
 */
void WaterZoomButton::setChokingProp(const double& p)
{
    choking      = min(width(), height()) * p; ///< 计算 choking 值
    choking_prop = p;                          ///< 保存 choking 比例
}

/**
 * @brief 设置缩放时的圆角半径
 * @param radius 缩放时的圆角半径值
 */
void WaterZoomButton::setRadiusZoom(const int& radius)
{
    radius_zoom = radius;
}

/**
 * @brief 设置按钮的圆角半径
 * @param x 绘制时的圆角半径
 * @param x2 缩放时的圆角半径
 * @note 仅使用 x 参数绘制，忽略 y 参数
 */
void WaterZoomButton::setRadius(const int& x, const int& x2)
{
    // 注意：最终绘制中只计算 x 的半径，无视 y 的半径
    InteractiveButtonBase::setRadius(x);
    radius_zoom = x2;
}

/**
 * @brief 获取背景绘制路径，支持动态缩放和 choking 效果
 * @return QPainterPath 背景绘制路径
 */
QPainterPath WaterZoomButton::getBgPainterPath()
{
    QPainterPath path;
    int c; ///< choking 值
    int r; ///< 圆角半径

    // 根据悬停进度计算 choking 和圆角半径
    if (!hover_progress)
    {
        c = choking;
        r = radius_x;
    }
    else
    {
        c = choking * (1 - getNolinearProg(hover_progress, hovering ? FastSlower : SlowFaster));
        r = radius_zoom < 0 ? radius_x :
            radius_x + (radius_zoom - radius_x) * hover_progress / 100;
    }

    // 绘制带圆角或矩形的背景路径
    if (r)
    {
        path.addRoundedRect(QRect(c, c, size().width() - c * 2, size().height() - c * 2), r, r);
    }
    else
    {
        path.addRect(QRect(c, c, size().width() - c * 2, size().height() - c * 2));
    }

    return path;
}

/**
 * @brief 处理控件大小调整事件，更新 choking 值
 * @param event 大小调整事件
 */
void WaterZoomButton::resizeEvent(QResizeEvent *event)
{
    InteractiveButtonBase::resizeEvent(event);

    // 根据比例更新 choking 值
    if (qAbs(choking_prop) > 0.0001)
    {
        choking = min(width(), height()) * choking_prop;
    }
}