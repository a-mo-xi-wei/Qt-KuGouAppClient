#ifndef GALLERYPHOTOWIDGET_H
#define GALLERYPHOTOWIDGET_H

#include "WaterZoomButton/WaterZoomButton.h"

struct GalleryPhotoData {
    QPixmap pixmap;
    QString title;
    QString subTitle;
};

class GalleryPhotoWidget : public WaterZoomButton {
    Q_OBJECT

public:
    explicit GalleryPhotoWidget(const GalleryPhotoData& data, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    static int fixed_width, fixed_height;
    static int content_width, content_height;
    static int pixmap_width, pixmap_height;
    static QColor title_color;
    static QColor subTitle_color;

private:
    QPixmap pixmap;
    QString title, subTitle;
};

#endif // GALLERYPHOTOWIDGET_H
