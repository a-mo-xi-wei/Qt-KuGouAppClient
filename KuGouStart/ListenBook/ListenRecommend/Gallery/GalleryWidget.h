#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt] 简单易用的轮播图
 *  项目地址为 ： https://github.com/iwxyi/Qt-GalleryWidget
 *  非常实用，感兴趣的可以star
 */

#include "MyScrollArea.h"
#include "galleryphotowidget.h"

class GalleryWidget : public MyScrollArea {
    Q_OBJECT

public:
    explicit GalleryWidget(QWidget *parent = nullptr);

    void loadData(const QList<GalleryPhotoData>& list);

    void resizeGallery(QPoint emit_pos = QPoint(0, 0));

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void signalPictureClicked(GalleryPhotoWidget *bean);

public:
    static int item_spacing_h;
    static int item_spacing_v;

private:
    QList<GalleryPhotoWidget *> widgets;
    QWidget *center_widget;
};

#endif // GALLERYWIDGET_H
