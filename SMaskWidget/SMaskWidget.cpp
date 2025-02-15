#include "SMaskWidget.h"

#include <QMouseEvent>
#include <QPainterPath>
#include <QPainter>
#include <QPropertyAnimation>

SMaskWidget::SMaskWidget(QWidget *parent)
    : QWidget(parent)
    ,m_w(0)
    ,m_h(0)
    ,m_radius(0)
    ,m_centerX(0)
    ,m_centerY(0)
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    // 位置动画
    m_posAnimation = new QPropertyAnimation(this, "animatedY");
    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // 透明度动画
    m_alphaAnimation = new QPropertyAnimation(this, "alpha");
    m_alphaAnimation->setDuration(300);
    m_alphaAnimation->setEasingCurve(QEasingCurve::OutQuad);

}

void SMaskWidget::setDefaultFillCircleColor(const QColor &color) {
    this->m_defaultFillCircleColor = color;
}

void SMaskWidget::setDefaultFillTriangleColor(const QColor &color) {
    this->m_defaultFillTriangleColor = color;
}

void SMaskWidget::setHoverFillCircleColor(const QColor &color) {
    this->m_hoverFillCircleColor = color;
}

void SMaskWidget::setHoverFillTriangleColor(const QColor &color) {
    this->m_hoverFillTriangleColor = color;
}

void SMaskWidget::setEnterWidgetChangeCursor(const bool &change) {
    this->m_isEnterWidgetChangeCursor = change;
}

void SMaskWidget::setBorderRadius(const int &radius) {
    this->m_borderRadius = radius;
}

void SMaskWidget::setMaskColor(const QColor &color) {
    this->m_maskColor = color;
}

void SMaskWidget::setStander(const int &stander) {
    this->m_stander = stander;
    calOnce();
}

void SMaskWidget::setMove(const bool &move) {
    this->m_isMove = move;
}

bool SMaskWidget::getMove() {
    return m_isMove;
}

void SMaskWidget::animationUp() {
    //qDebug()<<"开始动画向上";
    // 位置动画：从3/4高度到2/4高度
    this->m_posAnimation->stop();
    this->m_posAnimation->setStartValue(3*height()/4);
    this->m_posAnimation->setEndValue(height()/2);
    this->m_posAnimation->start();

    // 透明度动画：从0到255
    this->m_alphaAnimation->stop();
    this->m_alphaAnimation->setStartValue(0);
    this->m_alphaAnimation->setEndValue(255);
    this->m_alphaAnimation->start();
}

void SMaskWidget::animationDown() {
    //qDebug()<<"开始动画向下";
    // 反向动画
    this->m_posAnimation->stop();
    this->m_posAnimation->setStartValue(height()/2);
    this->m_posAnimation->setEndValue(3*height()/4);
    this->m_posAnimation->start();

    this->m_alphaAnimation->stop();
    this->m_alphaAnimation->setStartValue(255);
    this->m_alphaAnimation->setEndValue(0);
    this->m_alphaAnimation->start();
}

void SMaskWidget::setAnimatedY(int y) {
    m_animatedY = y;
    //qDebug()<<"当前动画高度: "<<m_animatedY;
    update();
    emit animatedYChanged(y);
}

void SMaskWidget::setAlpha(int alpha) {
    m_alpha = alpha;
    //qDebug()<<"当前透明度: "<<alpha;
    update();
    emit alphaChanged(alpha);
}

void SMaskWidget::calOnce() {
    m_w = static_cast<const float>(rect().width());
    m_h = static_cast<const float>(rect().height());
    float stander = 0;
    if (!this->m_stander) stander = std::min(m_w,m_h);
    else stander = this->m_stander;
    //qDebug()<<"当前标准: "<<stander;
    m_radius = stander/5;
    m_centerX = static_cast<const float>(rect().width())/2;
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

bool SMaskWidget::isMouseInCircle(const float &mouseX, const float &mouseY)const {
    // 计算鼠标位置和圆心的距离
    const float dx = mouseX - m_w/2;
    const float dy = mouseY - m_h/2;
    return (dx * dx + dy * dy <= m_radius * m_radius); // 在圆内时返回 true
}

void SMaskWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 先绘制背景遮罩（始终绘制）
    painter.setBrush(m_maskColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), m_borderRadius, m_borderRadius);

    // 2. 动态调整三角形顶点位置（仅当启用动画时）
    if (m_isMove) {
        const float stander = m_stander ? m_stander : std::min(m_w, m_h);
        const float offsetY = stander / 18;
        m_ap.setY(m_animatedY - offsetY);
        m_bp.setY(m_animatedY + offsetY);
        m_cp.setY(m_animatedY);
    }

    // 3. 创建公共路径元素
    QPainterPath trianglePath;
    trianglePath.moveTo(m_ap);
    trianglePath.lineTo(m_bp);
    trianglePath.lineTo(m_cp);
    trianglePath.closeSubpath();
    trianglePath = trianglePath.simplified();

    QPainterPath circlePath;
    const qreal currentY = m_isMove ? m_animatedY : m_centerY;
    circlePath.addEllipse(QPointF(m_centerX, currentY), m_radius, m_radius);

    // 4. 分状态绘制
    if (m_isEnterCircle) {
        // 4.1 先绘制三角形（如果需要填充）
        if (m_hoverFillTriangleColor.isValid()) {
            painter.setBrush(m_hoverFillTriangleColor);
            painter.setPen(Qt::NoPen);
            painter.drawPath(trianglePath);
        }

        // 4.2 创建镂空效果路径
        QPainterPath combinedPath = circlePath.subtracted(trianglePath);

        // 修复关键点：确保使用正确的合成模式
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setBrush(m_hoverFillCircleColor);
        painter.drawPath(combinedPath);
    } else {
        // 5.1 绘制圆形
        painter.setPen(Qt::white);
        painter.setBrush(m_defaultFillCircleColor.isValid() ?
                        m_defaultFillCircleColor : Qt::NoBrush);
        painter.drawPath(circlePath);

        // 5.2 绘制三角形
        painter.setBrush(m_defaultFillTriangleColor);
        painter.setPen(Qt::NoPen);
        painter.drawPath(trianglePath);
        //qDebug()<<"圆形路径："<<circlePath<<" 三角形路径："<<trianglePath;
    }
}

void SMaskWidget::mousePressEvent(QMouseEvent *event) {
    //qDebug() << "鼠标按下";
    event->ignore();
}

void SMaskWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void SMaskWidget::mouseReleaseEvent(QMouseEvent *event) {
    //qDebug() << "鼠标释放";
    event->ignore();
}

void SMaskWidget::mouseMoveEvent(QMouseEvent *event) {
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

void SMaskWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calOnce();
    //qDebug() << "大小改变******";
    update();
}
