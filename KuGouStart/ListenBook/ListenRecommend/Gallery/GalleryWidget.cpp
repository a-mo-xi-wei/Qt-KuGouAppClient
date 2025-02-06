#include "GalleryWidget.h"

#include <QPropertyAnimation>

int GalleryWidget::item_spacing_h = 10;
int GalleryWidget::item_spacing_v = 10;

GalleryWidget::GalleryWidget(QWidget *parent) : QWidget(parent)
{
    
}

/**
 * 加载数据，初始化列表
 * 并且开启显示动画
 */
void GalleryWidget::loadData(const QList<GalleryPhotoData>& list)
{
    foreach (GalleryPhotoWidget* widget, widgets) {
        widget->deleteLater();
    }
    widgets.clear();

    foreach (GalleryPhotoData data, list)
    {
        widgets.append(new GalleryPhotoWidget(data, this));
    }

    resizeGallery();
}

/**
 * 重新调整所有控件的位置
 */
void GalleryWidget::resizeGallery(QPoint emit_pos)
{
    if (widgets.isEmpty())
        return ;
    const int gpw_width = GalleryPhotoWidget::fixed_width;
    const int gpw_height = GalleryPhotoWidget::fixed_height;
    int col_count = qMax((this->width()-item_spacing_h) / (gpw_width + item_spacing_h), 1); // 一列数量
    if (col_count > widgets.size())
        col_count = static_cast<int>(widgets.size());
    const int row_count = qMax((static_cast<int>(widgets.size()) + col_count - 1) / col_count, 1); // 行数
    const int total_height = row_count * (gpw_height + item_spacing_v) + item_spacing_v*2;
    this->setMinimumHeight(total_height);
    this->resize(this->width(), total_height);
    const int total_left = (this->width() - col_count * (gpw_width + item_spacing_h)) / 2;
    const int total_top = item_spacing_v;

    int cur_row = 0, cur_col = 0;
    for (const auto& widget : widgets)
    {
        QPoint pos(total_left + cur_col * (gpw_width + item_spacing_h), total_top + cur_row * (gpw_height + item_spacing_v));
        const auto ani = new QPropertyAnimation(widget, "pos");
        ani->setStartValue(widget->pos());
        ani->setEndValue(pos);
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuad);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        ani->start();

        cur_col++;
        if (cur_col >= col_count)
        {
            cur_col = 0;
            cur_row++;
        }
    }
}

void GalleryWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->setFixedWidth(width());
    resizeGallery();
}
