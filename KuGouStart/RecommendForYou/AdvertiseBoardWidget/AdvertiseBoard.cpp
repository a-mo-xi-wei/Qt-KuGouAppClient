#include "AdvertiseBoard.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QResizeEvent>

NavButton::NavButton(const QString& normalImage, const QString& hoverImage, QWidget* parent)
    : QLabel(parent), m_normal(normalImage), m_hover(hoverImage) {
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    setPixmap(m_normal);
    setAttribute(Qt::WA_TranslucentBackground);
    setAlignment(Qt::AlignCenter);
}

void NavButton::enterEvent(QEnterEvent* event) {
    setPixmap(m_hover);
    QLabel::enterEvent(event);
}

void NavButton::leaveEvent(QEvent* event) {
    setPixmap(m_normal);
    QLabel::leaveEvent(event);
}

void NavButton::mousePressEvent(QMouseEvent* event) {
    emit clicked();
    QLabel::mousePressEvent(event);
}

AdvertiseBoard::AdvertiseBoard(QWidget* parent)
    : QWidget(parent),
      m_leftBtn(new NavButton(":/Res/window/left.svg", ":/Res/window/left-pink.svg", this)),
      m_rightBtn(new NavButton(":/Res/window/right.svg", ":/Res/window/right-pink.svg", this)),
      m_timer(new QTimer(this))
{
    m_timer->setInterval(3000);
    connect(m_timer, &QTimer::timeout, this, [this] {
        m_currentIndex = (m_currentIndex + 1) % m_posters.size();
        update();
    });

    connect(m_leftBtn, &NavButton::clicked, this, [this] {
        m_currentIndex = (m_currentIndex - 1 + m_posters.size()) % m_posters.size();
        update();
        m_timer->start(); // Reset timer on interaction
    });

    connect(m_rightBtn, &NavButton::clicked, this, [this] {
        m_currentIndex = (m_currentIndex + 1) % m_posters.size();
        update();
        m_timer->start();
    });

    m_leftBtn->hide();
    m_rightBtn->hide();
    updateButtonPosition();
}

void AdvertiseBoard::addPoster(const QPixmap& posterPix) {
    m_posters.append(posterPix);
    if (m_posters.size() == 1 && !m_timer->isActive()) {
        m_timer->start();
    }
}

void AdvertiseBoard::setAspectRatio(double ratio) {
    m_aspectRatio = ratio > 0 ? ratio : 1.0;
    updateGeometry();
}

void AdvertiseBoard::setAutoPlayInterval(int ms) {
    m_timer->setInterval(ms > 0 ? ms : 3000);
}

void AdvertiseBoard::paintEvent(QPaintEvent* ev) {
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (!m_posters.isEmpty()) {
        // Draw current poster
        QPixmap scaled = m_posters[m_currentIndex].scaled(size(), Qt::KeepAspectRatioByExpanding);
        QPainterPath path;
        path.addRoundedRect(rect(), 10, 10);
        painter.setClipPath(path);
        painter.drawPixmap(rect(), scaled);
    }

    // Draw navigation dots
    if (m_posters.size() > 1) {
        QList<QPoint> centers;
        int totalWidth;
        calculateDotPositions(centers, totalWidth);

        painter.setPen(Qt::NoPen);
        for (int i = 0; i < centers.size(); ++i) {
            bool isActive = (i == m_currentIndex);
            int radius = isActive ? DOT_RADIUS + ACTIVE_DOT_EXTRA : DOT_RADIUS;
            painter.setBrush(isActive ? QColor(255, 100, 100) : QColor(255, 255, 255, 150));
            painter.drawEllipse(centers[i], radius, radius);
        }
    }
}

void AdvertiseBoard::resizeEvent(QResizeEvent* ev) {
    updateButtonPosition();
    setFixedHeight(ev->size().width() / m_aspectRatio);
    QWidget::resizeEvent(ev);
}

void AdvertiseBoard::enterEvent(QEnterEvent* ev) {
    m_leftBtn->show();
    m_rightBtn->show();
    QWidget::enterEvent(ev);
}

void AdvertiseBoard::leaveEvent(QEvent* ev) {
    m_leftBtn->hide();
    m_rightBtn->hide();
    if (!m_posters.isEmpty()) {
        m_timer->start();
    }
    QWidget::leaveEvent(ev);
}

void AdvertiseBoard::updateButtonPosition() {
    m_leftBtn->setFixedSize(60, height());
    m_rightBtn->setFixedSize(60, height());
    m_leftBtn->move(0, 0);
    m_rightBtn->move(width() - m_rightBtn->width(), 0);
}

void AdvertiseBoard::calculateDotPositions(QList<QPoint>& centers, int& totalWidth) {
    const int count = m_posters.size();
    const int maxRadius = DOT_RADIUS + ACTIVE_DOT_EXTRA;
    totalWidth = (count - 1) * (2 * maxRadius + DOT_SPACING) + 2 * maxRadius;

    int startX = (width() - totalWidth) / 2 + maxRadius;
    int yPos = height() - 20; // 20px from bottom

    for (int i = 0; i < count; ++i) {
        centers.append(QPoint(startX, yPos));
        startX += 2 * maxRadius + DOT_SPACING;
    }
}