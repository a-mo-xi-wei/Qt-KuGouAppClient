﻿#include "login_form.h"
#include "qtmaterialfab.h"

#include <QPainter>
#include <QRegularExpressionValidator>

#include "ElaToolTip.h"

Login_form::Login_form(QWidget *parent)
    : QWidget{parent} {
    this->resize(477, 620);

    username = new Input_box(":/Res/login/account.png", this);
    username->move(46, 161);
    username->setPlaceholderText("Username");
    username->setMaxLength(20);
    username->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\-!@#$%^&*()+=.,?:;\"'{}<>/|]{0,20}"), this));
    username->openToolTip();
    username->setIconToolTip(QStringLiteral("用户名"));

    password = new Input_box(":/Res/login/password-lock.png", this);
    password->move(46, 253);
    password->setPlaceholderText("Password");
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    password->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));
    password->openToolTip();
    password->setIconToolTip(QStringLiteral("解锁"));

    login_button = new Login_button(this);
    login_button->setCenter_text("Login");
    login_button->move(46, 371);

    other_login_buttons1 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/qq.png"), this);
    other_login_buttons1->setCursor(Qt::PointingHandCursor);         ///< 设置发送按钮光标
    other_login_buttons1->setRippleStyle(Material::PositionedRipple);   ///< 设置涟漪效果
    other_login_buttons1->setCorner(Qt::BottomRightCorner);             ///< 设置按钮位置
    other_login_buttons1->setXOffset(365);                              ///< 设置 X 偏移
    other_login_buttons1->setYOffset(115);                              ///< 设置 Y 偏移
    other_login_buttons1->setBackgroundColor(QColor(0xa5bbe4));    ///< 设置背景色
    auto qq_tooTip = new ElaToolTip(other_login_buttons1);
    qq_tooTip->setToolTip(QStringLiteral("QQ登录"));

    other_login_buttons2 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/wechat.png"), this);
    other_login_buttons2->setCursor(Qt::PointingHandCursor);         ///< 设置发送按钮光标
    other_login_buttons2->setRippleStyle(Material::PositionedRipple);   ///< 设置涟漪效果
    other_login_buttons2->setCorner(Qt::BottomRightCorner);             ///< 设置按钮位置
    other_login_buttons2->setXOffset(260);                              ///< 设置 X 偏移
    other_login_buttons2->setYOffset(115);                              ///< 设置 Y 偏移
    other_login_buttons2->setBackgroundColor(QColor(0xa5bbe4));    ///< 设置背景色
    auto wechat_tooTip = new ElaToolTip(other_login_buttons2);
    wechat_tooTip->setToolTip(QStringLiteral("微信登录"));

    other_login_buttons3 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/logo_google.png"), this);
    other_login_buttons3->setCursor(Qt::PointingHandCursor);         ///< 设置发送按钮光标
    other_login_buttons3->setRippleStyle(Material::PositionedRipple);   ///< 设置涟漪效果
    other_login_buttons3->setCorner(Qt::BottomRightCorner);             ///< 设置按钮位置
    other_login_buttons3->setXOffset(155);                              ///< 设置 X 偏移
    other_login_buttons3->setYOffset(115);                              ///< 设置 Y 偏移
    other_login_buttons3->setBackgroundColor(QColor(0xa5bbe4));    ///< 设置背景色
    auto google_tooTip = new ElaToolTip(other_login_buttons3);
    google_tooTip->setToolTip(QStringLiteral("Google登录"));

    other_login_buttons4 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/github-fill.png"), this);
    other_login_buttons4->setCursor(Qt::PointingHandCursor);         ///< 设置发送按钮光标
    other_login_buttons4->setRippleStyle(Material::PositionedRipple);   ///< 设置涟漪效果
    other_login_buttons4->setCorner(Qt::BottomRightCorner);             ///< 设置按钮位置
    other_login_buttons4->setXOffset(50);                               ///< 设置 X 偏移
    other_login_buttons4->setYOffset(115);                              ///< 设置 Y 偏移
    other_login_buttons4->setBackgroundColor(QColor(0xa5bbe4));    ///< 设置背景色
    auto github_tooTip = new ElaToolTip(other_login_buttons4);
    github_tooTip->setToolTip(QStringLiteral("Github登录"));

    this->animations();

    connect(login_button, &Login_button::execute_animation_signal, this, &Login_form::execute_animation);

    // 连接图标点击信号
    connect(password, &Input_box::iconClicked, [this] {
        m_isPasswordVisible = !m_isPasswordVisible;

        password->setIconToolTip(m_isPasswordVisible ? QStringLiteral("锁定") : QStringLiteral("解锁"));
        // 切换密码可见性
        password->setEchoMode(m_isPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);

        // 切换图标（需要准备两个图标）
        password->setIcon(m_isPasswordVisible ? ":/Res/login/password-unlock.png" : ":/Res/login/password-lock.png");
    });
}

void Login_form::animations() {
    animation = new QPropertyAnimation(this->login_button, "geometry");
    animation->setDuration(250);
    animation->setStartValue(this->login_button->geometry());
    animation->setEndValue(QRect(this->login_button->pos().x() + zoom_rate,
                                 this->login_button->pos().y() + zoom_rate / 2, login_button->width() - zoom_rate * 2,
                                 login_button->height() - zoom_rate));
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Login_form::execute_animation(Login_button::AnimationState State) {
    if (State == Login_button::Execute) {
        animation->setDirection(QAbstractAnimation::Forward);
        animation->start();
    } else if (State == Login_button::Restore) {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
        ///< 松开鼠标，发送登录信号
        //m_libHttp.UrlRequestPost(QStringLiteral("http://127.0.0.1:8080/api/addSong"), jsonString); ///< 发送登录请求

    }
}

void Login_form::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    crop_corner(&painter);
    draw_text(&painter);
}

void Login_form::crop_corner(QPainter *painter) {
    painter->setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter->setBrush(Brush);
    painter->drawRect(0, 0, width(), height());
}

void Login_form::draw_text(QPainter *painter) {
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(30);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter->setFont(font1);

    QColor semiTransparent(0, 0, 0, 255);
    painter->setPen(semiTransparent);

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, "Login");
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 6));
    painter->drawText(rect1, "Login");
}
