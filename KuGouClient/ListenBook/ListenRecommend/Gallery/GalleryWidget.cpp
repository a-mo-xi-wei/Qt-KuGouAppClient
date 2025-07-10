/**
 * @file GalleryWidget.cpp
 * @brief 实现 GalleryWidget 类，提供照片画廊控件功能
 * @author WeiWang
 * @date 2025-05-16
 * @version 1.0
 * @note 参考开源项目：iwxyi 的 Qt-GalleryWidget，项目地址：https://github.com/iwxyi/Qt-GalleryWidget
 */

#include "GalleryWidget.h"

#include <QPropertyAnimation>
#include <QResizeEvent>

/**
 * @brief 水平间距
 */
int GalleryWidget::item_spacing_h = 1;

/**
 * @brief 垂直间距
 */
int GalleryWidget::item_spacing_v = 1;

/**
 * @brief 构造函数，初始化照片画廊控件
 * @param parent 父控件指针，默认为 nullptr
 */
GalleryWidget::GalleryWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setCursor(Qt::PointingHandCursor);             ///< 设置手形光标
    //this->setAttribute(Qt::WA_OpaquePaintEvent);         ///< 启用不透明绘制
}

/**
 * @brief 添加照片卡片数据
 * @param it 照片卡片控件指针
 * @note 初始化列表并开启显示动画
 */
void GalleryWidget::addData(GalleryPhotoWidget *it)
{
    widgets.append(it);                                  ///< 添加卡片到列表
    resizeGallery();                                     ///< 调整画廊布局
}

/**
 * @brief 调整画廊布局
 * @param emit_pos 动画起始位置，默认为 (0, 0)
 * @note 重新调整所有控件位置并应用动画
 */
void GalleryWidget::resizeGallery(QPoint emit_pos)
{
    if (widgets.isEmpty())
    {
        return;                                          ///< 列表为空时返回
    }
    const int gpw_width = GalleryPhotoWidget::fixed_width; ///< 获取卡片宽度
    const int gpw_height = GalleryPhotoWidget::fixed_height; ///< 获取卡片高度
    int col_count = qMax((this->width() + item_spacing_h) / (gpw_width + item_spacing_h), 1); ///< 计算列数
    if (col_count > widgets.size())
    {
        col_count = static_cast<int>(widgets.size());    ///< 限制列数
    }
    const int row_count = qMax((static_cast<int>(widgets.size()) + col_count - 1) / col_count, 1); ///< 计算行数
    const int total_height = row_count * (gpw_height + item_spacing_v) + item_spacing_v * 2; ///< 计算总高度
    this->setMinimumHeight(total_height + 5);            ///< 设置最小高度
    this->resize(this->width(), total_height);           ///< 调整画廊尺寸
    const int total_left = (this->width() - col_count * (gpw_width + item_spacing_h)) / 2; ///< 计算左侧边距
    const int total_top = item_spacing_v;                ///< 设置顶部边距
    int cur_row = 0, cur_col = 0;                        ///< 初始化行列计数
    for (const auto &widget : widgets)
    {
        const QPoint pos(total_left + cur_col * (gpw_width + item_spacing_h), total_top + cur_row * (gpw_height + item_spacing_v)); ///< 计算卡片位置
        const auto ani = new QPropertyAnimation(widget, "pos"); ///< 创建位置动画
        ani->setStartValue(widget->pos());               ///< 设置起始位置
        ani->setEndValue(pos);                           ///< 设置目标位置
        ani->setDuration(300);                           ///< 设置动画时长
        ani->setEasingCurve(QEasingCurve::OutQuad);      ///< 设置缓动曲线
        connect(ani, &QPropertyAnimation::finished, ani, &QObject::deleteLater); ///< 动画完成后清理
        ani->start();                                    ///< 启动动画
        cur_col++;                                       ///< 增加列计数
        if (cur_col >= col_count)
        {
            cur_col = 0;                                 ///< 重置列计数
            cur_row++;                                   ///< 增加行计数
        }
    }
}

/**
 * @brief 获取照片卡片列表
 * @return 照片卡片列表引用
 */
QVector<GalleryPhotoWidget *> &GalleryWidget::getWidgets()
{
    return this->widgets;                                ///< 返回卡片列表
}

/**
 * @brief 调整大小事件，重新布局画廊
 * @param event 调整大小事件
 */
void GalleryWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类调整大小事件
    resizeGallery();                                     ///< 调整画廊布局
}