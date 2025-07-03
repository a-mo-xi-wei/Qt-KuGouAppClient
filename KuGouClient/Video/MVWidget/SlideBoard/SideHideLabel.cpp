/**
 * @file SideHideLabel.cpp
 * @brief 实现 SideHideLabel 类，提供支持偏移和圆角裁剪的标签控件
 * @author iwxyi
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：https://github.com/iwxyi/Qt-SlideShow
 */

#include "SideHideLabel.h"
#include <QPainter>
#include <QPainterPath>

/**
 * @brief 构造函数，初始化标签控件
 * @param parent 父控件指针，默认为 nullptr
 */
SideHideLabel::SideHideLabel(QWidget *parent)
    : QLabel(parent)
{
}

/**
 * @brief 复制标签控件
 * @param other 源标签控件指针
 * @return 新创建的标签控件指针
 */
SideHideLabel *SideHideLabel::copy(const SideHideLabel *other)
{
    const auto label = new SideHideLabel(other->parentWidget());
    label->setGeometry(other->geometry()); ///< 复制几何属性
    label->sideOffset = other->sideOffset; ///< 复制偏移量
    label->startX = other->startX;        ///< 复制起始 X 位置
    label->radius = other->radius;        ///< 复制圆角半径
    label->pixmap = other->pixmap;        ///< 复制图片
    return label;
}

/**
 * @brief 设置图片和最大偏移量
 * @param pixmap 图片
 * @param sideMaxOffset 最大偏移量
 */
void SideHideLabel::setPixmap(const QPixmap &pixmap, const double &sideMaxOffset)
{
    this->pixmap = pixmap;               ///< 设置图片
    this->startX = -sideMaxOffset;       ///< 设置起始 X 位置为负最大偏移量
}

/**
 * @brief 获取最大偏移量
 * @return 最大偏移量
 */
double SideHideLabel::getMaxOffset() const
{
    return -startX;
}

/**
 * @brief 绘制事件
 * @param e 绘制事件对象
 * @note 重写基类方法
 */
void SideHideLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);         ///< 启用抗锯齿
    painter.setRenderHint(QPainter::SmoothPixmapTransform); ///< 启用平滑图片变换

    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius);          ///< 添加圆角矩形路径
    painter.setClipPath(path);                            ///< 设置裁剪路径

    const QPixmap newPixmap = pixmap.scaledToHeight(this->height(), Qt::FastTransformation); ///< 缩放图片

    painter.drawPixmap(static_cast<int>(startX + sideOffset), 0, ///< 绘制图片，应用偏移
                       newPixmap.width(), newPixmap.height(),
                       newPixmap);
}

/**
 * @brief 设置侧边偏移量
 * @param of 偏移量
 */
void SideHideLabel::setSideOffset(const double &of)
{
    this->sideOffset = of;
    update(); ///< 刷新界面
}

/**
 * @brief 获取侧边偏移量
 * @return 偏移量
 */
double SideHideLabel::getSideOffset() const
{
    return sideOffset;
}