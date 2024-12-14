#include"SMaskWidget.h"

#include <QMouseEvent>
#include<QPainterPath>

SMaskWidget::SMaskWidget(QWidget *parent)
    : QWidget(parent)
    ,m_radius(0)
    ,m_centerX(0)
    ,m_centerY(0)
    ,m_w(0)
    ,m_h(0)
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
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

void SMaskWidget::calOnce() {
    m_w = static_cast<const float>(rect().width());
    m_h = static_cast<const float>(rect().height());
    auto tmp = std::min(m_w,m_h);
    m_radius = static_cast<float>(tmp)/5;
    m_centerX = static_cast<const float>(rect().width())/2;
    m_centerY = static_cast<const float>(rect().height())/2;
    m_ap = static_cast<const QPointF>(QPointF(m_centerX - tmp / 26, m_centerY - tmp / 18));
    m_bp = static_cast<const QPointF>(QPointF(m_centerX - tmp / 26, m_centerY + tmp / 18));
    m_cp = static_cast<const QPointF>(QPointF(m_centerX + tmp / 15, m_centerY));
}

bool SMaskWidget::isMouseInCircle(const float &mouseX, const float &mouseY)const {
    // 计算鼠标位置和圆心的距离
    const float dx = mouseX - m_w/2;
    const float dy = mouseY - m_h/2;
    return (dx * dx + dy * dy <= m_radius * m_radius); // 在圆内时返回 true
}

void SMaskWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    //qDebug()<<"重绘";//只要悬停就会一直调用，不明所以
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置绘制颜色，带有透明度
    painter.setBrush(this->m_maskColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), this->m_borderRadius, this->m_borderRadius);
    if(this->m_isEnterCircle) {
        // 创建一个绘制路径
        QPainterPath path;
        // 定义三角形的三个顶点
        path.moveTo(m_ap.x(),m_ap.y());
        path.lineTo(m_bp.x(),m_bp.y());
        path.lineTo(m_cp.x(),m_cp.y());
        path.closeSubpath(); // 闭合路径
        path = path.simplified();
        if(this->m_hoverFillTriangleColor != QColor()) {
            painter.setBrush(this->m_hoverFillTriangleColor); // 设置三角形填充颜色
            painter.drawPath(path); // 绘制三角形路径
        }
        // 使用路径组合来创建镂空效果
        QPainterPath circlePath;
        circlePath.addEllipse(QPointF(m_centerX, m_centerY), m_radius, m_radius);
        // 使用路径组合来创建镂空效果
        QPainterPath combinedPath = circlePath.subtracted(path);
        combinedPath = combinedPath.simplified();
        // 绘制镂空效果
        painter.setBrush(this->m_hoverFillCircleColor); // 设置填充颜色为白色
        painter.drawPath(combinedPath);
    }
    else {
        //绘制圆形
        painter.setPen(Qt::white);
        if(this->m_defaultFillCircleColor == QColor()) {
            painter.setBrush(Qt::NoBrush);
        }
        else {
            painter.setBrush(this->m_defaultFillCircleColor);
        }
        painter.drawEllipse(QPointF(m_centerX,m_centerY), m_radius,m_radius);
        // 创建一个绘制路径
        QPainterPath path;
        painter.setBrush(m_defaultFillTriangleColor);
        painter.setPen(Qt::NoPen);
        // 定义三角形的三个顶点
        path.moveTo(m_ap.x(),m_ap.y());
        path.lineTo(m_bp.x(),m_bp.y());
        path.lineTo(m_cp.x(),m_cp.y());
        path.closeSubpath(); // 闭合路径
        path = path.simplified();
        painter.drawPath(path);
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
    if(this->m_isEnterCircle != isMouseInCircle(x,y)) {
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

}

void SMaskWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calOnce();
    //qDebug() << "大小改变******";
    update();
}
