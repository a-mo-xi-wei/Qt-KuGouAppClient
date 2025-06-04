#include "login_register_form.h"
#include "windoweffect.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

LoginRegisterForm::LoginRegisterForm(QWidget *parent)
    : QWidget{parent} {
    this->setFixedSize(955, 620);
    //设置窗体透明
    setWindowFlags(Qt::FramelessWindowHint);
    WindowEffect::addShadowEffect((HWND)winId());

    transparent_transition_interface2 = new Transparent_transition_interface(
        "Welcome Back!", "Already have an account?", "Login", this);
    transparent_transition_interface2->button->animation_status(false);
    transparent_transition_interface2->move(this->width(), 0);

    registration_form = new Registration_form(this);
    registration_form->move(width(), 0);

    login_form = new Login_form(this);
    login_form->move(this->width() / 2, 0);

    scroll_bar = new Scroll_bar(this);
    scroll_bar->move(-width() * 1.5, 0);

    transparent_transition_interface = new Transparent_transition_interface(
        "Hello, Welcome!", "Don't have an account?", "Register", this);
    transparent_transition_interface->move(0, 0);

    this->setRightShow();
    this->build_animation();

    connect(transparent_transition_interface->button, &Hollow_button::page_changed, this,
            &LoginRegisterForm::execute_animation);
    connect(transparent_transition_interface2->button, &Hollow_button::page_changed, this,
            &LoginRegisterForm::execute_animation);
}

void LoginRegisterForm::setRightShow() const {
    transparent_transition_interface2->raise();
    registration_form->lower();
    transparent_transition_interface->raise();
    login_form->lower();
}

void LoginRegisterForm::build_animation() {
    animation2 = new QPropertyAnimation(this->scroll_bar, "pos");
    animation2->setDuration(m_animation_duration);
    animation2->setStartValue(this->scroll_bar->pos());
    animation2->setEndValue(QPoint(this->width() / 2, 0));
    animation2->setEasingCurve(QEasingCurve::Linear);

    animation3 = new QPropertyAnimation(this->transparent_transition_interface, "pos");
    animation3->setDuration(m_animation_duration);
    animation3->setStartValue(this->transparent_transition_interface->pos());
    animation3->setEndValue(QPoint(-this->width() / 2, 0));
    animation3->setEasingCurve(QEasingCurve::Linear);

    animation4 = new QPropertyAnimation(this->transparent_transition_interface2, "pos");
    animation4->setDuration(m_animation_duration);
    animation4->setStartValue(this->transparent_transition_interface2->pos());
    animation4->setEndValue(QPoint(this->width() / 2, 0));
    animation4->setEasingCurve(QEasingCurve::Linear);

    animation5 = new QPropertyAnimation(this->registration_form, "pos");
    animation5->setDuration(m_animation_duration);
    animation5->setStartValue(this->registration_form->pos());
    animation5->setEndValue(QPoint(0, 0));
    animation5->setEasingCurve(QEasingCurve::Linear);

    animation6 = new QPropertyAnimation(this->login_form, "pos");
    animation6->setDuration(m_animation_duration);
    animation6->setStartValue(this->login_form->pos());
    animation6->setEndValue(QPoint(-this->width() / 2, 0));
    animation6->setEasingCurve(QEasingCurve::Linear);

    connect(animation2, &QPropertyAnimation::valueChanged, this, [this] {
        if (scroll_bar->pos().x() > -this->width() / 2 && animation4->state() == !QAbstractAnimation::Running &&
            animation_execute_duration) {
            animation2->pause();
            animation3->setDirection(QAbstractAnimation::Forward);
            animation3->start();
            animation_execute_duration = false;
        } else if (scroll_bar->pos().x() < -this->width() / 10 && animation3->state() == !QAbstractAnimation::Running &&
                   animation_restore_duration) {
            animation2->pause();
            animation4->setDirection(QAbstractAnimation::Backward);
            animation4->start();
            animation_restore_duration = false;
        }
    });

    connect(animation3, &QPropertyAnimation::finished, this, [this] {
    if (animation2->state() == QAbstractAnimation::Paused) {
        animation2->resume();
    }
});

    connect(animation4, &QPropertyAnimation::finished, this, [this] {
        if (animation2->state() == QAbstractAnimation::Paused) {
            animation2->resume();
        }
    });

    connect(animation3, &QPropertyAnimation::finished, this, &LoginRegisterForm::onAnimation3Finished);
    connect(animation4, &QPropertyAnimation::finished, this, &LoginRegisterForm::onAnimation4Finished);
}

void LoginRegisterForm::onAnimation3Finished() {
    if (currentSequence == 1) {
        animation4->setDirection(QAbstractAnimation::Forward);
        animation4->start();
        animation5->setDirection(QAbstractAnimation::Forward);
        animation5->start();
    }
}

void LoginRegisterForm::onAnimation4Finished() {
    if (currentSequence == 1) return;

    if (currentSequence == 2) {
        animation3->setDirection(QAbstractAnimation::Backward);
        animation3->start();

        animation6->setDirection(QAbstractAnimation::Backward);
        animation6->start();
    }
}

void LoginRegisterForm::execute_animation(Hollow_button::AnimationState status) {
    if (status == Hollow_button::AnimationState::ANIMATION_STATE_EXECUTING) {
        animation_execute_duration = true;
        currentSequence = 1;

        animation2->setDirection(QAbstractAnimation::Forward);
        animation2->start();

        animation6->setDirection(QAbstractAnimation::Forward);
        animation6->start();
    } else if (status == Hollow_button::AnimationState::ANIMATION_STATE_RESET) {
        animation_restore_duration = true;
        currentSequence = 2;

        animation2->setDirection(QAbstractAnimation::Backward);
        animation2->start();
    }
}

void LoginRegisterForm::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 25, 25);

    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPath(path);
}

void LoginRegisterForm::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->globalPosition().toPoint();
        m_startWindowPosition = this->pos();
        event->accept();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void LoginRegisterForm::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->globalPosition().toPoint() - m_dragStartPosition;
        this->move(m_startWindowPosition + delta);
        event->accept();
    } else {
        QWidget::mouseMoveEvent(event);
    }
}

int LoginRegisterForm::animation_duration() const {
    return m_animation_duration;
}

void LoginRegisterForm::setAnimation_duration(const int& newAnimation_duration) {
    m_animation_duration = newAnimation_duration;
}

