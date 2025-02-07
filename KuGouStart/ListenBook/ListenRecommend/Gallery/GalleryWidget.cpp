#include "GalleryWidget.h"

#include <QPropertyAnimation>

int GalleryWidget::item_spacing_h = 1;
int GalleryWidget::item_spacing_v = 1;

GalleryWidget::GalleryWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setCursor(Qt::PointingHandCursor);
    this->setContentsMargins(0,0,0,0);
}

/**
 * 加载数据，初始化列表
 * 并且开启显示动画
 */
void GalleryWidget::addData(GalleryPhotoWidget* it)
{
    widgets.append(it);

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
    int col_count = qMax((this->width() + item_spacing_h) / (gpw_width + item_spacing_h), 1); // 一列数量
    if (col_count > widgets.size())
        col_count = static_cast<int>(widgets.size());
    const int row_count = qMax((static_cast<int>(widgets.size()) + col_count - 1) / col_count, 1); // 行数
    const int total_height = row_count * (gpw_height + item_spacing_v) + item_spacing_v*2;
    this->setMinimumHeight(total_height);
    this->resize(this->width(), total_height);
    const int total_left = (this->width() - col_count * (gpw_width + item_spacing_h)) / 2;
    const int total_top = item_spacing_v;
    //qDebug()<<"当前宽度："<<this->width()<<" 当前高度："<<this->height();
    //qDebug()<<"行数有 ："<<row_count<<" 列数有："<<col_count;
    int cur_row = 0, cur_col = 0;
    for (const auto& widget : widgets)
    {
        const QPoint pos(total_left + cur_col * (gpw_width + item_spacing_h), total_top + cur_row * (gpw_height + item_spacing_v));
        const auto ani = new QPropertyAnimation(widget, "pos");
        ani->setStartValue(widget->pos());
        ani->setEndValue(pos);
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuad);
        //connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, &QPropertyAnimation::finished, ani, &QObject::deleteLater);
        ani->start();

        cur_col++;
        if (cur_col >= col_count)
        {
            cur_col = 0;
            cur_row++;
        }
    }
}

QVector<GalleryPhotoWidget *> & GalleryWidget::getWidgets() {
    return this->widgets;
}

void GalleryWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeGallery();
}
