#include "login_button.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

Login_button::Login_button(QWidget* parent) : QPushButton{ parent }
{
    this->resize(392, 57);
    this->setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
    this->start_animation();
}

void Login_button::start_animation()
{
    animation = new QPropertyAnimation(this, "color_opacity");
    animation->setDuration(200);
    animation->setStartValue(this->color_opacity());
    animation->setEndValue(188);
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Login_button::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), 10, 10);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);

    const QBrush Brush(QColor(123, 150, 228, m_color_opacity));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());

    draw_text(&painter);
}

void Login_button::draw_text(QPainter *painter)
{
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

bool Login_button::event(QEvent* e)
{
    if (e->type() == QEvent::Enter)
    {
        animation->setDirection(QPropertyAnimation::Forward);
        animation->start();
        // @note qDebug() << "鼠标进入";
    }
    else if (e->type() == QEvent::Leave)
    {
        animation->setDirection(QPropertyAnimation::Backward);
        animation->start();
        // @note qDebug() << "鼠标离开";
        update();
    }

    return QPushButton::event(e);
}

void Login_button::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) emit execute_animation_signal(AnimationState::Execute);
    QAbstractButton::mousePressEvent(event);
}

void Login_button::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) emit execute_animation_signal(AnimationState::Restore);
    QAbstractButton::mouseReleaseEvent(event);
}

int Login_button::color_opacity() const
{
    return m_color_opacity;
}

void Login_button::setColor_opacity(int newColor_opacity)
{
    m_color_opacity = newColor_opacity;
    update();
}

QString Login_button::center_text() const
{
    return m_center_text;
}

void Login_button::setCenter_text(const QString &newCenter_text)
{
    m_center_text = newCenter_text;
}
