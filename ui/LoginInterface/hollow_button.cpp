#include "hollow_button.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

Hollow_button::Hollow_button(QWidget *parent) : QPushButton{parent} {
    this->setCursor(Qt::PointingHandCursor);

    animation3 = new QPropertyAnimation(this, "radius");
    animation3->setDuration(400);
    animation3->setStartValue(m_radius);
    animation3->setEndValue(this->width());
    animation3->setEasingCurve(QEasingCurve::Linear);
    animation3->setDirection(QAbstractAnimation::Forward);

    animation1 = new QPropertyAnimation(this, "opacity");
    animation1->setDuration(400);
    animation1->setStartValue(m_opacity);
    animation1->setEndValue(0);
    animation1->setEasingCurve(QEasingCurve::Linear);
    animation1->setDirection(QAbstractAnimation::Forward);

    connect(animation3, &QPropertyAnimation::finished, this, &Hollow_button::reset_animation);
    connect(animation1, &QPropertyAnimation::finished, this, &Hollow_button::reset_animation);
}

void Hollow_button::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    draw_border(&painter);
    draw_text(&painter);
    draw_disappearing_circle(&painter);
}

void Hollow_button::draw_border(QPainter *painter) {
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), height() / 4, height() / 4);
    painter->setClipPath(path);

    QPen pen;
    pen.setWidth(5);
    pen.setColor(QColor(255, 255, 255, 255));
    painter->setPen(pen);
    painter->drawRoundedRect(0, 0, width(), height(), height() / 4, height() / 4);
}

void Hollow_button::draw_text(QPainter *painter) {
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(13);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter->setFont(font1);

    QColor semiTransparent(255, 255, 255, 255);
    painter->setPen(semiTransparent);

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, m_center_text);
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 2));
    painter->drawText(rect1, m_center_text);
}

void Hollow_button::draw_disappearing_circle(QPainter *painter) {
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), height() / 4, height() / 4);
    painter->setClipPath(path);

    painter->setPen(Qt::NoPen);
    QBrush brush(QColor(255, 255, 255, m_opacity));
    painter->setBrush(brush);
    painter->drawEllipse(mouse_coordinates, m_radius, m_radius);
}


void Hollow_button::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mouse_coordinates = event->pos();
        update();
        execute_animation();
    }
    QWidget::mousePressEvent(event);
}

void Hollow_button::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (this->status == true) emit page_changed(Hollow_button::ANIMATION_STATE_EXECUTING);
        else emit page_changed(Hollow_button::ANIMATION_STATE_RESET);
    }
    QWidget::mouseReleaseEvent(event);
}

void Hollow_button::animation_status(bool status) {
    this->status = status;
}

void Hollow_button::execute_animation() {
    animation3->start();
    animation1->start();
}

void Hollow_button::reset_animation() {
    m_radius = 0;
    m_opacity = 255;
}

QString Hollow_button::center_text() const {
    return m_center_text;
}

void Hollow_button::setCenter_text(const QString &newCenter_text) {
    m_center_text = newCenter_text;
    update();
}

int Hollow_button::radius() const {
    return m_radius;
}

void Hollow_button::setRadius(int newRadius) {
    if (m_radius == newRadius)
        return;
    m_radius = newRadius;
    update();
}

int Hollow_button::opacity() const {
    return m_opacity;
}

void Hollow_button::setOpacity(int newOpacity) {
    if (m_opacity == newOpacity)
        return;
    m_opacity = newOpacity;
    update();
}
