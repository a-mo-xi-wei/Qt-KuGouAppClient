#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include "interactivebuttonbase.h"
#include "sidehidelabel.h"
#include <QVBoxLayout>
class SlideShow : public QWidget {
    Q_OBJECT

public:
    explicit SlideShow(QWidget *parent = nullptr);

    void setPixmapSize(const QSize &size);

    void setPixmapScale(const bool &scale); // 已废弃
    void setAutoSlide(const int &interval = 5000) const; // 设置自动轮播，interval=0时停止
    int getCurrentIndex() const;

    void adjustLabels(SideHideLabel *hidingLabel = nullptr);

public slots:
    void addImage(const QPixmap &pixmap, QString text = "");

    void insertImage(const int &index, const QPixmap &pixmap, QString text = "");

    void removeImage(const int &index);

    void setCurrentIndex(int index);

    void slideToLeft();

    void slideToRight();

private:
    QPixmap getScaledRoundedPixmap(QPixmap pixmap) const;

    void moveTo(SideHideLabel *label, QRect geometry) const;

protected:
    void resizeEvent(QResizeEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void signalImageClicked(int index);

    void signalTextActivated(QString text);

private:
    QList<SideHideLabel*> labels;
    QList<QPixmap> pixmaps;
    QList<QString> texts;
    QList<InteractiveButtonBase*> indications;
    int currentIndex = -1;
    QTimer *autoSlideTimer;
    SideHideLabel *hidingLabel = nullptr;

    QColor normalColor = QColor(172, 128, 58);
    QColor selectColor = Qt::yellow;

    QSize oneSize; // 默认图片大小
    double backScale = 0.8; // 两侧卡片的缩放程度
    double sideOffside = 0.5; // 两侧卡片的位置偏移，越大靠越远
    double imgOffside = 0.15; // 图片内偏移比例
    bool scalePixmap = false; // 图片居中裁剪还是大小缩放（现在只能为false了）
    QHBoxLayout *indicationLayout;

    QRect centerRect, leftRect, rightRect, backRect; //backRect 指的是未显现出来的图片，隐藏在centerRect 的后面

};

#endif // SLIDESHOW_H
