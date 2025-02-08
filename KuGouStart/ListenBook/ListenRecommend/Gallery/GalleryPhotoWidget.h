#ifndef GALLERYPHOTOWIDGET_H
#define GALLERYPHOTOWIDGET_H

#include "MyBlockWidget.h"
#include "WaterZoomButton/WaterZoomButton.h"

class GalleryPhotoWidget : public WaterZoomButton {
    Q_OBJECT

public:
    explicit GalleryPhotoWidget(QWidget *parent = nullptr);

    void setCoverPix(const QString &pixmapPath)const;

    void setTitleText(const QString &title) ;

    void setDescribeText(const QString &desc);

    void setPopularText(const QString& text) const;

private:
     void initUi();

    void updateTitleText()const;

    void updateDescText()const;

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

public:
    static int fixed_width, fixed_height;
    static int content_width, content_height;
    static int pixmap_width, pixmap_height;

private:
    MyBlockWidget* m_coverWidget{};
    QLabel* m_titleLab{};
    QLabel* m_descLab{};
    //text
    QString m_titleText;
    QString m_describeText;
};

#endif // GALLERYPHOTOWIDGET_H
