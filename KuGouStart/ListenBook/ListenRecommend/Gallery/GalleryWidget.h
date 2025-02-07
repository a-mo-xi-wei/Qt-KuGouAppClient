#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt] 简单易用的轮播图
 *  项目地址为 ： https://github.com/iwxyi/Qt-GalleryWidget
 *  非常实用，感兴趣的可以star
 */

#include "GalleryPhotoWidget.h"

class GalleryWidget : public QWidget {
    Q_OBJECT

public:
    explicit GalleryWidget(QWidget *parent = nullptr);

    void addData(GalleryPhotoWidget* it);

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
};

#endif // GALLERYWIDGET_H
