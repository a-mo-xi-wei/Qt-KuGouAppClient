#include "login_register_form.h"
#include "windoweffect.h"
#include "ElaToolTip.h"
#include "SApp.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QTimeLine>
#include <QTimer>

LoginRegisterForm::LoginRegisterForm(QWidget *parent)
    : QDialog{parent}
{
    this->setFixedSize(955, 620);
    //设置窗体透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    WindowEffect::addShadowEffect((HWND) winId());

    initUi();
    setRightShow();
    build_animation();
}

void LoginRegisterForm::setIsFirstShow(bool flag)
{
    isFirstShow = flag;
}

bool LoginRegisterForm::getIsFirstShow()
{
    return isFirstShow;
}

void LoginRegisterForm::setRightShow() const
{
    transparent_transition_interface2->raise();
    registration_form->lower();
    transparent_transition_interface->raise();
    login_form->lower();
}

void LoginRegisterForm::build_animation()
{
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

    connect(animation2,
            &QPropertyAnimation::valueChanged,
            this,
            [this] {
                if (scroll_bar->pos().x() > -this->width() / 2 && animation4->state() == !
                    QAbstractAnimation::Running &&
                    animation_execute_duration) {
                    animation2->pause();
                    animation3->setDirection(QAbstractAnimation::Forward);
                    animation3->start();
                    animation_execute_duration = false;
                } else if (scroll_bar->pos().x() < -this->width() / 10 && animation3->state() == !
                           QAbstractAnimation::Running &&
                           animation_restore_duration) {
                    animation2->pause();
                    animation4->setDirection(QAbstractAnimation::Backward);
                    animation4->start();
                    animation_restore_duration = false;
                }
            });

    connect(animation3,
            &QPropertyAnimation::finished,
            this,
            [this] {
                if (animation2->state() == QAbstractAnimation::Paused) {
                    animation2->resume();
                }
            });

    connect(animation4,
            &QPropertyAnimation::finished,
            this,
            [this] {
                if (animation2->state() == QAbstractAnimation::Paused) {
                    animation2->resume();
                }
            });

    connect(animation3,
            &QPropertyAnimation::finished,
            this,
            &LoginRegisterForm::onAnimation3Finished);
    connect(animation4,
            &QPropertyAnimation::finished,
            this,
            &LoginRegisterForm::onAnimation4Finished);
}

void LoginRegisterForm::onAnimation3Finished()
{
    if (currentSequence == 1) {
        animation4->setDirection(QAbstractAnimation::Forward);
        animation4->start();
        animation5->setDirection(QAbstractAnimation::Forward);
        animation5->start();
    }
    for (auto btn : findChildren<QPushButton *>()) {
        if (btn->objectName() == "minBtn" || btn->objectName() == "closeBtn")
            btn->raise();
    }
}

void LoginRegisterForm::onAnimation4Finished()
{
    if (currentSequence == 1)
        return;

    if (currentSequence == 2) {
        animation3->setDirection(QAbstractAnimation::Backward);
        animation3->start();

        animation6->setDirection(QAbstractAnimation::Backward);
        animation6->start();
    }
}

void LoginRegisterForm::execute_animation(Hollow_button::AnimationState status)
{
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

void LoginRegisterForm::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 25, 25);

    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPath(path);
}

void LoginRegisterForm::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->globalPosition().toPoint();
        m_startWindowPosition = this->pos();
        event->accept();
    } else {
        QDialog::mousePressEvent(event);
    }
}

void LoginRegisterForm::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint delta = event->globalPosition().toPoint() - m_dragStartPosition;
        this->move(m_startWindowPosition + delta);
        event->accept();
    } else {
        QDialog::mouseMoveEvent(event);
    }
}

bool LoginRegisterForm::eventFilter(QObject *watched, QEvent *event) ///< 仅仅用来识别最小化和关闭按钮
{
    auto btn = qobject_cast<QPushButton *>(watched);
    if (!btn)
        return QDialog::eventFilter(watched, event);

    const QString name = btn->objectName();

    if (event->type() == QEvent::Enter) {
        if (name == "minBtn") {
            btn->setIcon(QIcon(":/Res/titlebar/minimize-blue.svg"));
        } else if (name == "closeBtn") {
            btn->setIcon(QIcon(":/Res/titlebar/close-blue.svg"));
        }
    } else if (event->type() == QEvent::Leave) {
        if (name == "minBtn") {
            btn->setIcon(QIcon(":/Res/titlebar/minimize-black.svg"));
        } else if (name == "closeBtn") {
            btn->setIcon(QIcon(":/Res/titlebar/close-black.svg"));
        }
    }
    return QDialog::eventFilter(watched, event);
}

void LoginRegisterForm::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event); // 确保正常显示

    this->setWindowOpacity(0.0); // 从透明开始

    // 添加模糊效果
    auto blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(10); // 初始模糊
    this->setGraphicsEffect(blur);

    // 模糊动画（减弱模糊）
    auto blurAnim = new QPropertyAnimation(blur, "blurRadius", this);
    blurAnim->setDuration(300);
    blurAnim->setStartValue(10);
    blurAnim->setEndValue(0);

    // 透明度动画（从透明到不透明）
    auto opacityAnim = new QPropertyAnimation(this, "windowOpacity", this);
    opacityAnim->setDuration(300);
    opacityAnim->setStartValue(0.0);
    opacityAnim->setEndValue(1.0);

    // 并行动画组
    auto group = new QParallelAnimationGroup(this);
    group->addAnimation(blurAnim);
    group->addAnimation(opacityAnim);

    connect(group,
            &QParallelAnimationGroup::finished,
            this,
            [ = ] {
                setGraphicsEffect(nullptr); // 移除模糊
                if (isFirstShow) {
                    auto config = sApp->globalConfig();

                    //如果上次退出之前勾选了自动登陆，则自动点击登陆按钮
                    if (config->value("user/autoLogin").toBool()) {
                        qDebug() << "自动登录";
                        QTimer::singleShot(0,
                                           login_form,
                                           [this] {
                                               login_form->onLogin();
                                           });
                    }
                }
            });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginRegisterForm::accept()
{
    auto blur = new QGraphicsBlurEffect(this);
    this->setGraphicsEffect(blur);

    auto blurAnim = new QPropertyAnimation(blur, "blurRadius");
    blurAnim->setDuration(300);
    blurAnim->setStartValue(0);
    blurAnim->setEndValue(10);

    auto opacityAnim = new QPropertyAnimation(this, "windowOpacity");
    opacityAnim->setDuration(300);
    opacityAnim->setStartValue(1.0);
    opacityAnim->setEndValue(0.0);

    auto group = new QParallelAnimationGroup(this);
    group->addAnimation(blurAnim);
    group->addAnimation(opacityAnim);

    connect(group,
            &QParallelAnimationGroup::finished,
            this,
            [ = ] {
                setGraphicsEffect(nullptr);
                QDialog::accept();
            });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginRegisterForm::initUi()
{
    auto minBtn = new QPushButton(this);
    minBtn->setObjectName("minBtn");
    minBtn->setIcon(QIcon(QStringLiteral(":/Res/titlebar/minimize-black.svg")));
    minBtn->setCursor(Qt::PointingHandCursor);
    minBtn->installEventFilter(this);
    auto closeBtn = new QPushButton(this);
    closeBtn->setObjectName("closeBtn");
    closeBtn->setIcon(QIcon(QStringLiteral(":/Res/titlebar/close-black.svg")));
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->installEventFilter(this);
    closeBtn->setStyleSheet(R"(
        QPushButton {
            color: black;
            background: transparent;
            border: none;
            icon-size: 15px;
        }
        QPushButton:hover {
            color: #00A1FF;
        }
    )");

    minBtn->setStyleSheet(R"(
        QPushButton {
            color: black;
            background: transparent;
            border: none;
            icon-size: 15px;
        }
        QPushButton:hover {
            color: #00A1FF;
        }
    )");

    // 按钮尺寸
    const int btnWidth = 40;
    const int btnHeight = 30;

    minBtn->resize(btnWidth, btnHeight);
    closeBtn->resize(btnWidth, btnHeight);

    // 移动到右上角（比如间隔 0 或 5 像素）
    closeBtn->move(width() - btnWidth, 0);   // 最右
    minBtn->move(width() - 2 * btnWidth, 0); // 紧挨着关闭按钮左边

    //min_toolButton
    auto min_toolButton_toolTip = new ElaToolTip(minBtn);
    min_toolButton_toolTip->setToolTip(QStringLiteral("最小化"));

    //close_toolButton
    auto close_toolButton_toolTip = new ElaToolTip(closeBtn);
    close_toolButton_toolTip->setToolTip(QStringLiteral("关闭"));

    connect(minBtn, &QPushButton::clicked, this, &QDialog::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &LoginRegisterForm::exit);

    transparent_transition_interface2 = new Transparent_transition_interface(
        "Welcome Back!",
        "Already have an account?",
        "Login",
        this);
    transparent_transition_interface2->button->animation_status(false);
    transparent_transition_interface2->move(this->width(), 0);

    registration_form = new Registration_form(this);
    registration_form->move(width(), 0);

    login_form = new Login_form(this);
    login_form->move(this->width() / 2, 0);

    scroll_bar = new Scroll_bar(this);
    scroll_bar->move(-width() * 1.5, 0);

    transparent_transition_interface = new Transparent_transition_interface(
        "Hello, Welcome!",
        "Don't have an account?",
        "Register",
        this);
    transparent_transition_interface->move(0, 0);

    connect(transparent_transition_interface->button,
            &Hollow_button::page_changed,
            this,
            &LoginRegisterForm::execute_animation);
    connect(transparent_transition_interface2->button,
            &Hollow_button::page_changed,
            this,
            &LoginRegisterForm::execute_animation);

    connect(login_form, &Login_form::loginSuccess, this, &QDialog::accept);

    connect(login_form, &Login_form::exit, this, &LoginRegisterForm::exit);
    connect(registration_form, &Registration_form::exit, this, &LoginRegisterForm::exit);
}

int LoginRegisterForm::animation_duration() const
{
    return m_animation_duration;
}

void LoginRegisterForm::setAnimation_duration(const int &newAnimation_duration)
{
    m_animation_duration = newAnimation_duration;
}