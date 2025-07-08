#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QLabel>
#include <QList>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QVector>

static constexpr int DOT_RADIUS = 4;
static constexpr int DOT_SPACING = 8;
static constexpr int ACTIVE_DOT_EXTRA = 2;

class NavButton : public QLabel
{
    Q_OBJECT
public:
    explicit NavButton(const QString &normal, const QString &hover, QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap m_normal;
    QPixmap m_hover;
};

class AdvertiseBoard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int slideOffset READ slideOffset WRITE setSlideOffset)
public:
    explicit AdvertiseBoard(QWidget *parent = nullptr);
    ~AdvertiseBoard();

    void addPoster(const QPixmap &pixmap);
    void setAspectRatio(qreal aspectRatio);

    int slideOffset() const { return m_slideOffset; }
    void setSlideOffset(int offset);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void updateButtonPosition();
    void updateScaledPosters();

    void calculateDotPositions(QList<QPoint> &centers, int &totalWidth);

    void switchToNext();
    void switchToPrev();
    void startAnimation(int startValue, int endValue);

    QList<QPixmap> m_originalPosters;  // 原始海报图片
    QVector<QPixmap> m_scaledPosters;   // 缩放后的海报图片
    NavButton *m_leftBtn{};
    NavButton *m_rightBtn{};
    QTimer *m_timer{};
    QPropertyAnimation *m_animation{};

    int m_currentIndex = 0;
    int m_previousIndex = 0;
    int m_slideOffset = 0;
    qreal m_aspectRatio = 2.0;
    bool m_isAnimating = false;
    bool m_slidingToNext = true;
    bool m_isResizing = false;
};

#endif // ADVERTISEBOARD_H