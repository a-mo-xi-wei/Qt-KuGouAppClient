#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QLabel>
#include <QList>

class QTimer;

class NavButton : public QLabel {
    Q_OBJECT

public:
    NavButton(const QString &normalImage, const QString &hoverImage, QWidget *parent = nullptr);

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

class AdvertiseBoard : public QWidget {
    Q_OBJECT

public:
    explicit AdvertiseBoard(QWidget *parent = nullptr);

    ~AdvertiseBoard() override = default;

    void addPoster(const QPixmap &posterPix);

    void setAspectRatio(double ratio);

    void setAutoPlayInterval(int ms);

protected:
    void paintEvent(QPaintEvent *ev) override;

    void resizeEvent(QResizeEvent *ev) override;

    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

private:
    void updateButtonPosition();

    void calculateDotPositions(QList<QPoint> &centers, int &totalWidth);

    QList<QPixmap> m_posters;
    NavButton *m_leftBtn;
    NavButton *m_rightBtn;
    QTimer *m_timer;
    int m_currentIndex = 0;
    double m_aspectRatio = 2.0;

    // 圆点样式常量
    static constexpr int DOT_RADIUS = 4;
    static constexpr int DOT_SPACING = 8;
    static constexpr int ACTIVE_DOT_EXTRA = 2;
};

#endif // ADVERTISEBOARD_H
