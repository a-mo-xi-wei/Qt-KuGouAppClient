#include "LyricWidget.h"
#include "ImageFilter.h"
#include "LyricViewer.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVBoxLayout>

ThreadCalcBackgroundImage::~ThreadCalcBackgroundImage()
{
    // 请求终止
    requestInterruption();
    quit();
    wait();
}

void ThreadCalcBackgroundImage::run()
{
    // 是否请求终止
    while (!isInterruptionRequested()) {
        bool bPicFound = false;
        QPixmap pixmapToDeal;

        {
            QMutexLocker locker(&m_mutex);
            if (!vecPic.empty()) {
                bPicFound = true;
                pixmapToDeal = vecPic.back();
                vecPic.clear();
            }
        }
        // locker超出范围并释放互斥锁
        if (bPicFound) {
            QPixmap newPixmap = ImageFilter::BlurImage(pixmapToDeal, 5, 100);

            bPicFound = false;
            {
                QMutexLocker locker(&m_mutex);
                if (vecPic.empty()) //在没有新图片需要计算时才发出图片,保证发出的总是最后一次计算
                    emit ready(newPixmap);
            }
            // locker超出范围并释放互斥锁
        } else
            msleep(2000);
    }
}

void ThreadCalcBackgroundImage::showPic(QPixmap pic)
{
    QMutexLocker locker(&m_mutex);
    vecPic.emplace_back(pic);
}

LyricWidget::LyricWidget(QWidget *parent)
    : QWidget(parent)
      , m_animationGroup(new QParallelAnimationGroup(this))

{
    this->setMouseTracking(true);
    setWindowOpacity(0.0);
    initLayout();
    initEntity();
    initConnection();
    finishInit();
    setOriginStyle();
    hide();
    setWhetherToUseBlackMask(true);
}

LyricWidget::~LyricWidget() = default;

void LyricWidget::setOriginStyle()
{
    lyricViewer->setOriginStyle();
    phonograph->setOriginStyle();
}

void LyricWidget::initLayout()
{
    widgetMainPreview = new QWidget(this);
    widgetMainPreview->setObjectName("widgetMainPreview");
    widgetMainPreview->setMouseTracking(true);

    QHBoxLayout *hLayout = new QHBoxLayout(widgetMainPreview);

    phonograph = new Phonograph(widgetMainPreview);
    phonograph->setMinimumSize(480, 650);
    phonograph->setMaximumSize(480, 650);
    phonograph->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    lyricViewer = new LyricViewer(widgetMainPreview);
    lyricViewer->setMinimumSize(550, 650);
    lyricViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    hLayout->addWidget(phonograph);
    hLayout->addWidget(lyricViewer);

    // 给 LyricWidget 添加布局，让 widgetMainPreview 拉伸
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(widgetMainPreview);
}

void LyricWidget::initEntity()
{
    calPicThread = new ThreadCalcBackgroundImage(this);

    //初始化图片
    useBlackMask = false;
    blurBackgroundImage = QPixmap(":/Res/lyric/default_preview_background.png");
    whiteMaskImage = QPixmap(":/Res/lyric/album_background_white_mask.png");
    blackMaskImage = QPixmap(":/Res/lyric/album_background_black_mask.png");
    connect(&revealTimer,
            &QTimer::timeout,
            this,
            [this] {
                revealProgress += 0.05; // 每帧推进
                if (revealProgress >= 1.0) {
                    revealProgress = 1.0;
                    blurBackgroundImage = nextBackgroundImage;
                    nextBackgroundImage = QPixmap();
                    revealTimer.stop();
                }
                update();
            });
}

void LyricWidget::initConnection()
{
    connect(calPicThread,
            &ThreadCalcBackgroundImage::ready,
            this,
            &LyricWidget::setNewBackgroundPixmap);
    connect(lyricViewer->getLyricPanel(),
            &LyricPanel::jumpToTime,
            this,
            [this](const int &time) {
                emit jumpToTime(time);
                lyricViewer->setBlockAutoScroll(false);
            });
}

void LyricWidget::finishInit() const
{
    calPicThread->start(QThread::Priority::HighPriority);
}

void LyricWidget::calcNewBackgroundImage(const QPixmap &pixmap) const
{
    calPicThread->showPic(pixmap);
}

//设置是否使用黑色mask图层
void LyricWidget::setWhetherToUseBlackMask(bool useBlack)
{
    if (useBlackMask != useBlack) {
        useBlackMask = useBlack;
        update();
    }
}

bool LyricWidget::isLyricValid() const
{
    return lyricViewer->isLyricValid();
}

void LyricWidget::setViewerHighlightLineLyricAtPos(const int &pos) const
{
    lyricViewer->setLyricPanelHighlightLineLyricAtPos(pos);
}

void LyricWidget::toggleAnimation(int duration)
{
    QRect targetRect = currentTargetRect();

    if (m_animating) {
        // 动画中切换时，直接反向当前动画
        QRect endRect;
        qreal endOpacity;
        if (m_visible) {
            endRect = targetRect;
            endRect.setTop(targetRect.bottom());
            endOpacity = 0.0;
        } else {
            endRect = targetRect;
            endOpacity = 1.0;
        }
        m_visible = !m_visible;
        animateTo(endRect, endOpacity, duration);
        return;
    }

    if (m_visible) {
        // 消失动画
        m_visible = false;
        QRect endRect = targetRect;
        endRect.setTop(targetRect.bottom());
        animateTo(endRect, 0.0, duration);
    } else {
        // 显示动画
        m_visible = true;
        QRect startRect = targetRect;
        startRect.setTop(targetRect.bottom());
        setGeometry(startRect);
        setWindowOpacity(0.0);
        animateTo(targetRect, 1.0, duration);
    }
}

bool LyricWidget::isVisible() const { return m_visible; }

void LyricWidget::playPhonograph() const
{
    phonograph->play();
}

void LyricWidget::stopPhonograph() const
{
    phonograph->stop();
}

void LyricWidget::AlbumImageChanged(const QPixmap &newPixmap)
{
    phonograph->setAlbumCover(newPixmap);
    calcNewBackgroundImage(newPixmap);
}

void LyricWidget::setToDefaultAlbumImage()
{
    //不要每次都计算了，直接使用预定义的图片
    //AlbumImageChanged(QPixmap(":/resource/image/AlbumCover.jpg"));

    phonograph->setAlbumCover(QPixmap(":/Res/lyric/AlbumCover.jpg"));
    setNewBackgroundPixmap(QPixmap(":/Res/lyric/default_preview_background.png"));
}

void LyricWidget::setMusicTitle(const QString &title)
{
    lyricViewer->setMusicTitle(title);
}

void LyricWidget::setMusicSinger(const QString &singer)
{
    lyricViewer->setMusicSinger(singer);
}

void LyricWidget::setLyricPath(const QString &path)
{
    lyricViewer->setLyricPath(path);
}

void LyricWidget::setNewBackgroundPixmap(const QPixmap &newPixmap)
{
    if (blurBackgroundImage.isNull()) {
        // 第一次加载，直接显示
        blurBackgroundImage = newPixmap;
        update();
    } else {
        // 有旧图，准备动画
        nextBackgroundImage = newPixmap;
        revealProgress = 0.0;
        revealTimer.start(16); // ~60fps
    }
}

void LyricWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void LyricWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int margin = 4;  // 四周内边距
    int radius = 10; // 圆角半径

    // 1️⃣ 设置圆角裁剪，考虑 margin
    QRect innerRect = rect().adjusted(margin, margin, -margin, -margin);
    QPainterPath roundRectPath;
    roundRectPath.addRoundedRect(innerRect, radius, radius);
    painter.setClipPath(roundRectPath);

    // 2️⃣ 填充背景色
    painter.fillRect(innerRect, Qt::white);

    // 3️⃣ 绘制缩放背景
    auto drawScaledBackground = [&](const QPixmap &pix) {
        if (pix.isNull())
            return;
        QSize halfSize = pix.size();
        halfSize.setHeight(halfSize.height() * 2.5 / 4);
        QPixmap half = pix.scaled(halfSize);
        painter.drawPixmap(innerRect,
                           half.scaled(innerRect.size(), Qt::KeepAspectRatioByExpanding));
    };

    if (revealProgress < 1.0 && !nextBackgroundImage.isNull()) {
        painter.save();

        // 绘制旧背景
        drawScaledBackground(blurBackgroundImage);

        // 圆形 reveal 动画
        QPointF center(width() / 2.0, height() / 2.0);
        qreal maxRadius = std::hypot(width() / 2.0, height() / 2.0);
        qreal currentRadius = maxRadius * revealProgress;

        QPainterPath clipPath;
        clipPath.addEllipse(center, currentRadius, currentRadius);
        painter.setClipPath(clipPath, Qt::IntersectClip); // 与圆角裁剪叠加

        // 绘制新背景
        painter.setOpacity(revealProgress);
        drawScaledBackground(nextBackgroundImage);

        painter.restore();
        painter.setOpacity(1.0);
    } else {
        drawScaledBackground(blurBackgroundImage);
    }

    // 4️⃣ 绘制遮罩
    QPixmap maskLayer = useBlackMask ? blackMaskImage : whiteMaskImage;
    if (!maskLayer.isNull())
        painter.drawPixmap(innerRect, maskLayer.scaled(innerRect.size()));
}

QRect LyricWidget::currentTargetRect() const
{
    if (parentWidget())
        return parentWidget()->rect(); // 动态填满父控件
    else
        return QRect(pos(), size());
}

void LyricWidget::animateTo(const QRect &endRect, qreal endOpacity, int duration)
{
    if (m_animationGroup->state() == QAbstractAnimation::Running)
        m_animationGroup->stop();

    // 清空旧动画
    m_animationGroup->clear();

    auto *geomAnim = new QPropertyAnimation(this, "geometry");
    geomAnim->setDuration(duration);
    geomAnim->setStartValue(geometry());
    geomAnim->setEndValue(endRect);
    geomAnim->setEasingCurve(QEasingCurve::OutCubic);

    auto *opacityAnim = new QPropertyAnimation(this, "windowOpacity");
    opacityAnim->setDuration(duration);
    opacityAnim->setStartValue(windowOpacity());
    opacityAnim->setEndValue(endOpacity);

    m_animationGroup->addAnimation(geomAnim);
    m_animationGroup->addAnimation(opacityAnim);

    connect(m_animationGroup,
            &QParallelAnimationGroup::finished,
            [this]() {
                m_animating = false;
                // 仅在隐藏动画完成后才 hide
                if (!m_visible)
                    hide();
            });

    m_animating = true;
    show(); // 确保动画期间可见
    m_animationGroup->start();
}
