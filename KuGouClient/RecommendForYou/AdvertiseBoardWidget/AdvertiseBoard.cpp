#include "AdvertiseBoard.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QPropertyAnimation>
#include <QResizeEvent>

NavButton::NavButton(const QString &normalImage, const QString &hoverImage, QWidget *parent)
    : QLabel(parent), m_normal(normalImage), m_hover(hoverImage) {
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    setPixmap(m_normal);
    setAttribute(Qt::WA_TranslucentBackground);
    setAlignment(Qt::AlignCenter);
    // 新增延迟检查定时器
    m_checkTimer = new QTimer(this);
    m_checkTimer->setInterval(300);
    connect(m_checkTimer, &QTimer::timeout, this, &NavButton::checkHoverState);
}

void NavButton::setHoverState(bool hover) {
    setPixmap(hover ? m_hover : m_normal);
}

void NavButton::checkHoverState() {
    QPoint globalMousePos = QCursor::pos();
    QRect globalRect = QRect(mapToGlobal(QPoint(0, 0)), size());
    bool isInside = globalRect.contains(globalMousePos);
    //qDebug() << "Global check: mouse at" << globalMousePos << "button at" << globalRect << "is inside:" << isInside;
    if (!isInside) {
        setHoverState(false);
        m_checkTimer->stop();
    }
}

bool NavButton::event(QEvent *e) {
    switch (e->type()) {
        case QEvent::HoverEnter:
            setHoverState(true);
            // 开始持续检查
            m_checkTimer->start();
            return true;

        case QEvent::HoverLeave:
        case QEvent::Leave:
            setHoverState(false);
            // 停止检查
            m_checkTimer->stop();
            return true;

        case QEvent::MouseButtonPress:
            emit clicked();
            return true;

        default:
            break;
    }
    return QLabel::event(e);
}

AdvertiseBoard::AdvertiseBoard(QWidget *parent)
    : QWidget(parent)
      , m_leftBtn(new NavButton(":/Res/window/left.svg", ":/Res/window/left-pink.svg", this))
      , m_rightBtn(new NavButton(":/Res/window/right.svg", ":/Res/window/right-pink.svg", this))
      , m_timer(new QTimer(this))
      , m_animation(new QPropertyAnimation(this))
{
    setMouseTracking(true);
    // 配置动画
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("slideOffset");
    m_animation->setDuration(500);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);

    // 连接动画结束信号
    connect(m_animation, &QPropertyAnimation::finished, this, [this] {
        m_isAnimating = false;
        m_timer->start();
        m_slideOffset = 0;
        update();
    });

    // 自动播放定时器
    connect(m_timer, &QTimer::timeout, this, [this] {
        if (!m_isAnimating && m_originalPosters.size() > 1) {
            switchToNext();
        }
    });

    // 导航按钮
    connect(m_leftBtn, &NavButton::clicked, this, [this] {
        if (!m_isAnimating && m_originalPosters.size() > 1) {
            switchToPrev();
        }
    });

    connect(m_rightBtn, &NavButton::clicked, this, [this] {
        if (!m_isAnimating && m_originalPosters.size() > 1) {
            switchToNext();
        }
    });

    m_leftBtn->hide();
    m_rightBtn->hide();
    updateButtonPosition();
    m_timer->setInterval(3000);
}

AdvertiseBoard::~AdvertiseBoard() {
    if (m_animation->state() == QPropertyAnimation::Running) {
        m_animation->stop();
    }
}

void AdvertiseBoard::addPoster(const QPixmap &posterPix) {
    m_originalPosters.append(posterPix);
    if (m_originalPosters.size() == 1 && !m_timer->isActive()) {
        m_timer->start();
    }
    updateScaledPosters();
}

void AdvertiseBoard::setAspectRatio(qreal ratio) {
    m_aspectRatio = ratio > 0 ? ratio : 2.0;
    updateGeometry();
    updateScaledPosters();
}

void AdvertiseBoard::setSlideOffset(int offset) {
    m_slideOffset = offset;
    update();
}

void AdvertiseBoard::startAnimation(int startValue, int endValue) {
    if (m_isAnimating) return;

    m_isAnimating = true;
    m_animation->setStartValue(startValue);
    m_animation->setEndValue(endValue);
    m_animation->start();

    // 暂停自动播放
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void AdvertiseBoard::switchToNext() {
    m_previousIndex = m_currentIndex;
    m_currentIndex = (m_currentIndex + 1) % m_originalPosters.size();
    m_slidingToNext = true;
    startAnimation(width(), 0);
}

void AdvertiseBoard::switchToPrev() {
    m_previousIndex = m_currentIndex;
    m_currentIndex = (m_currentIndex - 1 + m_originalPosters.size()) % m_originalPosters.size();
    m_slidingToNext = false;
    startAnimation(-width(), 0);
}

void AdvertiseBoard::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 创建圆角矩形剪裁区域
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    if (!m_originalPosters.isEmpty()) {
        if (m_isAnimating) {
            // 动画状态：绘制两张图片（当前图片和切换中的图片）
            const QPixmap &current = m_scaledPosters[m_currentIndex];
            const QPixmap &previous = m_scaledPosters[m_previousIndex];

            // 简化绘制逻辑 - 直接使用缩放后的图片
            if (m_slidingToNext) {
                // 向右滑动
                painter.drawPixmap(m_slideOffset - width(), 0, width(), height(), previous);
                painter.drawPixmap(m_slideOffset, 0, width(), height(), current);
            } else {
                // 向左滑动
                painter.drawPixmap(m_slideOffset + width(), 0, width(), height(), previous);
                painter.drawPixmap(m_slideOffset, 0, width(), height(), current);
            }
        } else {
            // 非动画状态：只绘制当前图片
            const QPixmap &scaled = m_scaledPosters[m_currentIndex];
            painter.drawPixmap(rect(), scaled);
        }
    }

    if (m_originalPosters.size() > 1) {
        QList<QPoint> centers;
        int totalWidth;
        calculateDotPositions(centers, totalWidth); ///< 计算导航圆点位置
        // 清空并重新计算圆点区域
        m_dotRects.clear();

        painter.setPen(Qt::NoPen); ///< 设置无边框
        for (int i = 0; i < centers.size(); ++i) {
            bool isActive = (i == m_currentIndex);
            int radius = isActive ? DOT_RADIUS + ACTIVE_DOT_EXTRA : DOT_RADIUS; ///< 设置圆点半径
            painter.setBrush(isActive ? QColor(80, 143, 206) : QColor(255, 255, 255, 150)); ///< 设置圆点颜色
            painter.drawEllipse(centers[i], radius, radius); ///< 绘制圆点

            // 存储圆点的矩形区域（用于鼠标检测）
            QRect dotRect(centers[i].x() - radius - 5, centers[i].y() - radius - 5,
                          radius * 2 + 5 * 2, radius * 2 + 5 * 2);
            m_dotRects.append(dotRect);
        }
    }
}

void AdvertiseBoard::resizeEvent(QResizeEvent *ev) {
    if (m_isResizing) return; // 避免重复缩放
    m_isResizing = true;
    updateButtonPosition();
    setFixedHeight(ev->size().width() / m_aspectRatio);
    updateScaledPosters();
    m_isResizing = false;
    QWidget::resizeEvent(ev);
}

void AdvertiseBoard::enterEvent(QEnterEvent *ev) {
    m_leftBtn->show();
    m_rightBtn->show();
    QWidget::enterEvent(ev);
}

void AdvertiseBoard::leaveEvent(QEvent *ev) {
    m_leftBtn->hide();
    m_rightBtn->hide();
    QWidget::leaveEvent(ev);
}

void AdvertiseBoard::mouseMoveEvent(QMouseEvent *event) {
    if (m_originalPosters.size() <= 1) return;

    QPoint mousePos = event->pos();
    // 检查鼠标是否在圆点区域内
    for (int i = 0; i < m_dotRects.size(); ++i) {
        if (m_dotRects[i].contains(mousePos) && i != m_currentIndex) {
            switchToIndex(i);
            break;
        }
    }
    QWidget::mouseMoveEvent(event);
}

void AdvertiseBoard::mouseReleaseEvent(QMouseEvent *event) {
    if (m_originalPosters.size() <= 1) return;

    QPoint mousePos = event->pos();
    // 检查鼠标是否在圆点区域内
    for (int i = 0; i < m_dotRects.size(); ++i) {
        if (m_dotRects[i].contains(mousePos) && i != m_currentIndex) {
            switchToIndex(i);
            break;
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void AdvertiseBoard::switchToIndex(const int &index) {
    if (index < 0 || index >= m_originalPosters.size() ||
        index == m_currentIndex || m_isAnimating) {
        return;
        }

    m_previousIndex = m_currentIndex;
    m_currentIndex = index;

    // 确定滑动方向
    if (m_currentIndex > m_previousIndex) {
        m_slidingToNext = true;
        startAnimation(width(), 0);
    } else {
        m_slidingToNext = false;
        startAnimation(-width(), 0);
    }
}

void AdvertiseBoard::updateButtonPosition() {
    const int btnWidth = qMin(60, width() / 6);
    m_leftBtn->setFixedSize(btnWidth, height());
    m_rightBtn->setFixedSize(btnWidth, height());
    m_leftBtn->move(0, 0);
    m_rightBtn->move(width() - m_rightBtn->width(), 0);
}

void AdvertiseBoard::updateScaledPosters() {
    m_scaledPosters.clear();
    m_scaledPosters.reserve(m_originalPosters.size());

    for (const QPixmap &poster: m_originalPosters) {
        if (!poster.isNull()) {
            m_scaledPosters.append(poster.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }
}

/**
 * @brief 计算导航圆点位置
 * @param centers 圆点中心点列表
 * @param totalWidth 总宽度
 * @note 计算圆点位置并返回总宽度
 */
void AdvertiseBoard::calculateDotPositions(QList<QPoint> &centers, int &totalWidth) {
    const int count = m_originalPosters.size();
    const int maxRadius = DOT_RADIUS + ACTIVE_DOT_EXTRA;
    totalWidth = (count - 1) * (2 * maxRadius + DOT_SPACING) + 2 * maxRadius; ///< 计算总宽度

    int startX = (width() - totalWidth) / 2 + maxRadius; ///< 计算起始 X 坐标
    int yPos = height() - 20; ///< 设置 Y 坐标

    for (int i = 0; i < count; ++i) {
        centers.append(QPoint(startX, yPos)); ///< 添加圆点中心点
        startX += 2 * maxRadius + DOT_SPACING; ///< 更新 X 坐标
    }
}
