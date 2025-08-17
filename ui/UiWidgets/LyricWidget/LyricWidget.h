#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

#include <QThread>
#include <QMutex>
#include <QTimer>

#include "Phonograph.h"

class LyricViewer;
class QParallelAnimationGroup;

//计算背景的线程
class ThreadCalcBackgroundImage : public QThread
{
    Q_OBJECT

public:
    explicit ThreadCalcBackgroundImage(QObject *parent)
        : QThread(parent) {}

    ~ThreadCalcBackgroundImage() override;

    void run() override;

    void showPic(QPixmap pic);
signals:
    void ready(QPixmap pixmap);

private:
    QMutex m_mutex;
    QVector<QPixmap> vecPic;
};

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

//预览页面
class UIWIDGETS_EXPORT LyricWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LyricWidget(QWidget *parent = 0);
    ~LyricWidget() override;

    void setOriginStyle();

    //对lyricViewer的操作再加一层facade
    void setMusicTitle(const QString &title);
    void setMusicSinger(const QString &singer);
    void setLyricPath(const QString &path);
    void setLyricRawText(const QString& content);
    void setWhetherToUseBlackMask(bool); //设置是否使用黑色mask图层

    bool isLyricValid() const;
    void setViewerHighlightLineLyricAtPos(const int &pos) const;

    void toggleAnimation(int duration = 300);

    bool isVisible() const;

private:
    void initLayout();
    void initEntity();
    void initConnection();
    void finishInit() const;

    void calcNewBackgroundImage(const QPixmap &pixmap) const; //开线程计算并设置新的背景图片

public slots:
    void playPhonograph() const;
    void stopPhonograph() const;
    void AlbumImageChanged(const QPixmap &newPixmap);

    void setToDefaultAlbumImage();

private slots:
    void setNewBackgroundPixmap(const QPixmap &newPixmap);

signals:
    void jumpToTime(const int &time);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    QWidget *widgetMainPreview; //这个父控件

    Phonograph *phonograph;   //留声机类
    LyricViewer *lyricViewer; //歌词查看器

    QPixmap blurBackgroundImage; //模糊后的背景图片
    QPixmap whiteMaskImage;      //白色的过滤图片
    QPixmap blackMaskImage;      //黑色的过滤图片

    bool useBlackMask; //标记是否使用黑色的mask图层

    ThreadCalcBackgroundImage *calPicThread; //计算图片的线程

    // 动画相关
    QPixmap nextBackgroundImage; // 等待替换的新图
    qreal revealProgress = 1.0;  // 0.0~1.0，动画进度
    QTimer revealTimer;          // 驱动动画

    bool m_animating = false;
    bool m_visible = false;

    QParallelAnimationGroup *m_animationGroup = nullptr;

    QRect currentTargetRect() const; // 动态计算目标矩形
    void animateTo(const QRect &endRect, qreal endOpacity, int duration);
};

#endif // LYRICWIDGET_H