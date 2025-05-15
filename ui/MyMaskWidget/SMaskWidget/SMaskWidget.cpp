/**
 * @file SMaskWidget.cpp
 * @brief 实现 SMaskWidget 类，提供带动画的遮罩控件功能
 * @author [Your Name]
 * @date 2025-05-15
 * @version 1.0
 */

#include "SMaskWidget.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>

/**
 * @brief 构造函数，初始化遮罩控件
 * @param parent 父控件指针，默认为 nullptr
 */
SMaskWidget::SMaskWidget(QWidget *parent)
    : QWidget(parent)
    ,m_w(0)
    ,m_h(0)
    ,m_radius(0)
    ,m_centerX(0)
    ,m_centerY(0)
    ,m_aniGroup(new QParallelAnimationGroup(this))
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);

    // 初始化位置动画
    m_posAnimation = new QPropertyAnimation(this, "animatedY");
    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // 初始化透明度动画
    m_alphaAnimation = new QPropertyAnimation(this, "alpha");
    m_alphaAnimation->setDuration(300);
    m_alphaAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    this->m_aniGroup->addAnimation(m_posAnimation);
    this->m_aniGroup->addAnimation(m_alphaAnimation);
}

/**
 * @brief 设置默认圆形填充颜色
 * @param color 颜色值
 */
void SMaskWidget::setDefaultFillCircleColor(const QColor &color)
{
    this->m_defaultFillCircleColor = color;
}

/**
 * @brief 设置默认三角形填充颜色
 * @param color 颜色值
 */
void SMaskWidget::setDefaultFillTriangleColor(const QColor &color)
{
    this->m_defaultFillTriangleColor = color;
}

/**
 * @brief 设置悬浮圆形填充颜色
 * @param color 颜色值
 */
void SMaskWidget::setHoverFillCircleColor(const QColor &color)
{
    this->m_hoverFillCircleColor = color;
}

/**
 * @brief 设置悬浮三角形填充颜色
 * @param color 颜色值
 */
void SMaskWidget::setHoverFillTriangleColor(const QColor &color)
{
    this->m_hoverFillTriangleColor = color;
}

/**
 * @brief 设置进入控件时是否改变光标
 * @param change 是否改变光标
 */
void SMaskWidget::setEnterWidgetChangeCursor(const bool &change)
{
    this->m_isEnterWidgetChangeCursor = change;
}

/**
 * @brief 设置边框圆角半径
 * @param radius 圆角半径
 */
void SMaskWidget::setBorderRadius(const int &radius)
{
    this->m_borderRadius = radius;
}

/**
 * @brief 设置遮罩颜色
 * @param color 颜色值
 */
void SMaskWidget::setMaskColor(const QColor &color)
{
    this->m_maskColor = color;
}

/**
 * @brief 设置基准尺寸
 * @param stander 基准尺寸
 */
void SMaskWidget::setStander(const int &stander)
{
    this->m_stander = stander;
    calOnce();
}

/**
 * @brief 设置是否启用动画
 * @param move 是否启用动画
 */
void SMaskWidget::setMove(const bool &move)
{
    this->m_isMove = move;
}

/**
 * @brief 获取是否启用动画
 * @return 是否启用动画
 */
bool SMaskWidget::getMove()
{
    return m_isMove;
}

/**
 * @brief 启动向上动画
 */
void SMaskWidget::animationUp()
{
    this->m_aniGroup->stop();
    this->m_posAnimation->setStartValue(3 * height() / 4);
    this->m_posAnimation->setEndValue(height() / 2);
    this->m_alphaAnimation->setStartValue(0);
    this->m_alphaAnimation->setEndValue(255);

    this->m_aniGroup->start();
}

/**
 * @brief 启动向下动画
 */
void SMaskWidget::animationDown()
{
    this->m_aniGroup->stop();
    this->m_posAnimation->setStartValue(height() / 2);
    this->m_posAnimation->setEndValue(3 * height() / 4);
    this->m_alphaAnimation->setStartValue(255);
    this->m_alphaAnimation->setEndValue(0);
    this->m_aniGroup->start();
}

/**
 * @brief 设置动画 Y 坐标
 * @param y Y 坐标
 */
void SMaskWidget::setAnimatedY(int y)
{
    m_animatedY = y;
    update();
}

/**
 * @brief 设置透明度
 * @param alpha 透明度值
 */
void SMaskWidget::setAlpha(int alpha)
{
    m_alpha = alpha;
    update();
}

/**
 * @brief 计算几何参数
 */
void SMaskWidget::calOnce()
{
    m_w = static_cast<const float>(rect().width());
    m_h = static_cast<const float>(rect().height());
    float stander = 0;
    if (!this->m_stander) stander = std::min(m_w, m_h);
    else stander = this->m_stander;
    m_radius = stander / 5;
    m_centerX = static_cast<const float>(rect().width()) / 2;
    if (this->m_isMove) {
        m_animatedY = static_cast<const float>(rect().height())*3/4;
        m_ap = static_cast<const QPointF>(QPointF(m_centerX - stander / 26, m_animatedY - stander / 18));
        m_bp = static_cast<const QPointF>(QPointF(m_centerX - stander / 26, m_animatedY + stander / 18));
        m_cp = static_cast<const QPointF>(QPointF(m_centerX + stander / 15, m_animatedY));
    }
    else {
        m_centerY = static_cast<const float>(rect().height())/2;
        m_ap = static_cast<const QPointF>(QPointF(m_centerX - stander / 26, m_centerY - stander / 18));
        m_bp = static_cast<const QPointF>(QPointF(m_centerX - stander / 26, m_centerY + stander / 18));
        m_cp = static_cast<const QPointF>(QPointF(m_centerX + stander / 15, m_centerY));
    }
}

/**
 * @brief 判断鼠标是否在圆形内
 * @param mouseX 鼠标 X 坐标
 * @param mouseY 鼠标 Y 坐标
 * @return 是否在圆形内
 */
bool SMaskWidget::isMouseInCircle(const float &mouseX, const float &mouseY) const
{
    const float dx = mouseX - m_w / 2;
    const float dy = mouseY - m_h / 2;
    return (dx * dx + dy * dy <= m_radius * m_radius);
}

/**
 * @brief 绘制事件，绘制遮罩和动画效果
 * @param event 绘图事件
 */
void SMaskWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景遮罩
    painter.setBrush(m_maskColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), m_borderRadius, m_borderRadius);

    // 动态调整三角形顶点位置
    if (m_isMove) {
        painter.setOpacity(m_alpha / 255.0);
        const float stander = m_stander ? m_stander : std::min(m_w, m_h);
        const float offsetY = stander / 18;
        m_ap.setY(m_animatedY - offsetY);
        m_bp.setY(m_animatedY + offsetY);
        m_cp.setY(m_animatedY);
    }

    // 创建路径
    QPainterPath trianglePath;
    trianglePath.moveTo(m_ap);
    trianglePath.lineTo(m_bp);
    trianglePath.lineTo(m_cp);
    trianglePath.closeSubpath();
    trianglePath = trianglePath.simplified();

    QPainterPath circlePath;
    const qreal currentY = m_isMove ? m_animatedY : m_centerY;
    circlePath.addEllipse(QPointF(m_centerX, currentY), m_radius, m_radius);

    // 分状态绘制
    if (m_isEnterCircle) {
        // 绘制悬浮状态三角形
        if (m_hoverFillTriangleColor.isValid()) {
            painter.setBrush(m_hoverFillTriangleColor);
            painter.setPen(Qt::NoPen);
            painter.drawPath(trianglePath);
        }
        // 绘制镂空效果
        QPainterPath combinedPath = circlePath.subtracted(trianglePath);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setBrush(m_hoverFillCircleColor);
        painter.drawPath(combinedPath);
    } else {
        // 绘制默认状态圆形
        painter.setPen(Qt::white);
        painter.setBrush(m_defaultFillCircleColor.isValid() ? m_defaultFillCircleColor : Qt::NoBrush);
        painter.drawPath(circlePath);
        // 绘制默认状态三角形
        painter.setBrush(m_defaultFillTriangleColor);
        painter.setPen(Qt::NoPen);
        painter.drawPath(trianglePath);
    }
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 */
void SMaskWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 */
void SMaskWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 */
void SMaskWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
}

/**
 * @brief 鼠标移动事件
 * @param event 鼠标事件
 */
void SMaskWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    const auto x = static_cast<float>(event->pos().x());
    const auto y= static_cast<float>(event->pos().y());
    this->m_isEnterCircle = isMouseInCircle(x,y);
    if(this->m_isEnterWidgetChangeCursor) {//立马变指向
            this->setCursor(Qt::PointingHandCursor);
    }
    else {//进入圆圈再变指向
        if(this->m_isEnterCircle)this->setCursor(Qt::PointingHandCursor);
        else this->setCursor(Qt::ArrowCursor);
    }
    //qDebug() << "鼠标移动";
    update();
}

/**
 * @brief 大小调整事件
 * @param event 大小调整事件
 */
void SMaskWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calOnce();
    //qDebug() << "大小改变******";
    update();
}