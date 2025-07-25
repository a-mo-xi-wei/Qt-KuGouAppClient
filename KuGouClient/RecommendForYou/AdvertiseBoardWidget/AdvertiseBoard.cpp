#include "AdvertiseBoard.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QPropertyAnimation>
#include <QResizeEvent>

/**
 * @brief NavButton 构造函数
 * @param normalImage 正常状态下的按钮图片路径
 * @param hoverImage 悬停状态下的按钮图片路径
 * @param parent 父控件指针，默认为 nullptr
 */
NavButton::NavButton(const QString &normalImage, const QString &hoverImage, QWidget *parent)
    : QLabel(parent), m_normal(normalImage), m_hover(hoverImage) {
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    setPixmap(m_normal);
    setAttribute(Qt::WA_TranslucentBackground);
    setAlignment(Qt::AlignCenter);
    // 初始化延迟检查定时器
    m_checkTimer = new QTimer(this);
    m_checkTimer->setInterval(300);
    connect(m_checkTimer, &QTimer::timeout, this, &NavButton::checkHoverState);
}

/**
 * @brief 设置按钮的悬停状态
 * @param hover 是否为悬停状态
 */
void NavButton::setHoverState(bool hover) {
    setPixmap(hover ? m_hover : m_normal);
}

/**
 * @brief 检查鼠标是否仍在按钮区域内
 */
void NavButton::checkHoverState() {
    QPoint globalMousePos = QCursor::pos();
    QRect globalRect = QRect(mapToGlobal(QPoint(0, 0)), size());
    bool isInside = globalRect.contains(globalMousePos);
    if (!isInside) {
        setHoverState(false);
        m_checkTimer->stop();
    }
}

/**
 * @brief 处理事件，响应鼠标悬停、点击等
 * @param e 事件对象
 * @return 如果事件被处理则返回 true，否则返回 false
 */
bool NavButton::event(QEvent *e) {
    switch (e->type()) {
        case QEvent::HoverEnter:
            setHoverState(true);
            m_checkTimer->start();
            return true;

        case QEvent::HoverLeave:
        case QEvent::Leave:
            setHoverState(false);
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

/**
 * @brief AdvertiseBoard 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
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
        if (!m_isAnimating && m_postersPath.size() > 1) {
            switchToNext();
        }
    });

    // 导航按钮
    connect(m_leftBtn, &NavButton::clicked, this, [this] {
        if (!m_isAnimating && m_postersPath.size() > 1) {
            switchToPrev();
        }
    });

    connect(m_rightBtn, &NavButton::clicked, this, [this] {
        if (!m_isAnimating && m_postersPath.size() > 1) {
            switchToNext();
        }
    });

    m_leftBtn->hide();
    m_rightBtn->hide();
    updateButtonPosition();
    m_timer->setInterval(3000);

    // 初始化防抖定时器
    m_resizeTimer = new QTimer(this);
    m_resizeTimer->setSingleShot(true);
    m_resizeTimer->setInterval(200);

    connect(m_resizeTimer, &QTimer::timeout, this, [this]() {
        updateScaledPosters();
    });
}

/**
 * @brief AdvertiseBoard 析构函数
 */
AdvertiseBoard::~AdvertiseBoard() {
    if (m_animation->state() == QPropertyAnimation::Running) {
        m_animation->stop();
    }
}

/**
 * @brief 添加一张海报图片
 * @param pixPath 海报图片的路径
 */
void AdvertiseBoard::addPoster(const QString &pixPath) {
    m_postersPath.append(pixPath);
    if (m_postersPath.size() == 1 && !m_timer->isActive()) {
        m_timer->start();
    }
    updateScaledPosters();
}

/**
 * @brief 设置广告牌的宽高比
 * @param ratio 宽高比，需大于 0
 */
void AdvertiseBoard::setAspectRatio(qreal ratio) {
    m_aspectRatio = ratio > 0 ? ratio : 2.0;
    updateScaledPosters();
}

/**
 * @brief 设置滑动偏移量
 * @param offset 滑动偏移量
 */
void AdvertiseBoard::setSlideOffset(int offset) {
    m_slideOffset = offset;
    update();
}

/**
 * @brief 开始滑动动画
 * @param startValue 动画起始值
 * @param endValue 动画结束值
 */
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

/**
 * @brief 切换到下一张海报
 */
void AdvertiseBoard::switchToNext() {
    m_previousIndex = m_currentIndex;
    m_currentIndex = (m_currentIndex + 1) % m_postersPath.size();
    m_slidingToNext = true;
    startAnimation(width(), 0);
}

/**
 * @brief 切换到上一张海报
 */
void AdvertiseBoard::switchToPrev() {
    m_previousIndex = m_currentIndex;
    m_currentIndex = (m_currentIndex - 1 + m_postersPath.size()) % m_postersPath.size();
    m_slidingToNext = false;
    startAnimation(-width(), 0);
}

/**
 * @brief 切换到指定索引的海报
 * @param index 目标海报的索引
 */
void AdvertiseBoard::switchToIndex(const int &index) {
    if (index < 0 || index >= m_postersPath.size() ||
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

/**
 * @brief 绘制事件，绘制海报图片和导航圆点
 * @param ev 绘制事件对象
 */
void AdvertiseBoard::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 创建圆角矩形剪裁区域
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    if (!m_postersPath.isEmpty()) {
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
            painter.drawPixmap(rect(), m_scaledPosters[m_currentIndex]);
        }
    }

    if (m_postersPath.size() > 1) {
        QList<QPoint> centers;
        int totalWidth;
        calculateDotPositions(centers, totalWidth);
        // 清空并重新计算圆点区域
        m_dotRects.clear();

        painter.setPen(Qt::NoPen);
        for (int i = 0; i < centers.size(); ++i) {
            bool isActive = (i == m_currentIndex);
            int radius = isActive ? DOT_RADIUS + ACTIVE_DOT_EXTRA : DOT_RADIUS;
            painter.setBrush(isActive ? QColor(80, 143, 206) : QColor(255, 255, 255, 150));
            painter.drawEllipse(centers[i], radius, radius);

            // 存储圆点的矩形区域（用于鼠标检测）
            QRect dotRect(centers[i].x() - radius - 5, centers[i].y() - radius - 5,
                          radius * 2 + 5 * 2, radius * 2 + 5 * 2);
            m_dotRects.append(dotRect);
        }
    }
}

/**
 * @brief 窗口大小调整事件，更新按钮位置和缩放图片
 * @param ev 大小调整事件对象
 */
void AdvertiseBoard::resizeEvent(QResizeEvent *ev) {
    updateButtonPosition();
    setFixedHeight(ev->size().width() / m_aspectRatio);
    // 重置定时器
    m_resizeTimer->start();
    QWidget::resizeEvent(ev);
}

/**
 * @brief 鼠标进入事件，显示导航按钮
 * @param ev 鼠标进入事件对象
 */
void AdvertiseBoard::enterEvent(QEnterEvent *ev) {
    m_leftBtn->show();
    m_rightBtn->show();
    QWidget::enterEvent(ev);
}

/**
 * @brief 鼠标离开事件，隐藏导航按钮
 * @param ev 鼠标离开事件对象
 */
void AdvertiseBoard::leaveEvent(QEvent *ev) {
    m_leftBtn->hide();
    m_rightBtn->hide();
    QWidget::leaveEvent(ev);
}

/**
 * @brief 鼠标移动事件，处理导航圆点的交互
 * @param event 鼠标移动事件对象
 */
void AdvertiseBoard::mouseMoveEvent(QMouseEvent *event) {
    if (m_postersPath.size() <= 1) return;

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

/**
 * @brief 鼠标释放事件，处理导航圆点的点击
 * @param event 鼠标释放事件对象
 */
void AdvertiseBoard::mouseReleaseEvent(QMouseEvent *event) {
    if (m_postersPath.size() <= 1) return;

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

/**
 * @brief 更新导航按钮的位置
 */
void AdvertiseBoard::updateButtonPosition() {
    const int btnWidth = qMin(60, width() / 6);
    m_leftBtn->setFixedSize(btnWidth, height());
    m_rightBtn->setFixedSize(btnWidth, height());
    m_leftBtn->move(0, 0);
    m_rightBtn->move(width() - m_rightBtn->width(), 0);
}

/**
 * @brief 更新缩放后的海报图片
 */
void AdvertiseBoard::updateScaledPosters() {
    m_scaledPosters.clear();
    m_scaledPosters.reserve(m_postersPath.size());
    for (const QString &path: m_postersPath) {
        QImage image(path);
        if (image.isNull()) {
            qWarning() << __FILE__ << " " << __LINE__ << " image is null";
            return;
        }
        // QImage 更快且线程安全
        QImage scaled = image.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        m_scaledPosters.append(QPixmap::fromImage(scaled));
    }
}

/**
 * @brief 计算导航圆点位置
 * @param centers 输出参数，存储圆点中心点
 * @param totalWidth 输出参数，存储圆点总宽度
 */
void AdvertiseBoard::calculateDotPositions(QList<QPoint> &centers, int &totalWidth) {
    const int count = m_postersPath.size();
    const int maxRadius = DOT_RADIUS + ACTIVE_DOT_EXTRA;
    totalWidth = (count - 1) * (2 * maxRadius + DOT_SPACING) + 2 * maxRadius;

    int startX = (width() - totalWidth) / 2 + maxRadius;
    int yPos = height() - 20;

    for (int i = 0; i < count; ++i) {
        centers.append(QPoint(startX, yPos));
        startX += 2 * maxRadius + DOT_SPACING;
    }
}