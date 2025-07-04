#include "registration_form.h"
#include "qtmaterialfab.h"

#include <QPainter>
#include <QRegularExpressionValidator>

Registration_form::Registration_form(QWidget *parent)
    : QWidget{parent} {
    this->resize(477, 620);

    username = new Input_box(":/Res/login/account.png", this);
    username->move(46, 130);
    username->setPlaceholderText("Username");
    username->setMaxLength(20);
    username->setValidator(
        new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9_\\-!@#$%^&*()+=.,?:;\"'{}<>/|]{0,20}"), this));
    username->openToolTip();
    username->setIconToolTip(QStringLiteral("用户名"));

    email = new Input_box(":/Res/login/email.png", this);
    email->move(46, 220);
    email->setPlaceholderText("Email");
    email->setMaxLength(16);
    email->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this));
    email->openToolTip();
    email->setIconToolTip(QStringLiteral("邮箱"));

    password = new Input_box(":/Res/login/password.png", this);
    password->move(46, 310);
    password->setPlaceholderText("Password");
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    password->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));
    password->openToolTip();
    password->setIconToolTip(QStringLiteral("解锁"));

    login_button = new Login_button(this);
    login_button->setCenter_text("Registration");
    login_button->move(46, 400);

    other_login_buttons1 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/qq.png"), this);
    other_login_buttons1->setCursor(Qt::PointingHandCursor); ///< 设置发送按钮光标
    other_login_buttons1->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    other_login_buttons1->setCorner(Qt::BottomLeftCorner); ///< 设置按钮位置
    other_login_buttons1->setXOffset(62); ///< 设置 X 偏移
    other_login_buttons1->setYOffset(90); ///< 设置 Y 偏移
    other_login_buttons1->setBackgroundColor(QColor(0xa5bbe4)); ///< 设置背景色
    auto qq_tooTip = new ElaToolTip(other_login_buttons1);
    qq_tooTip->setToolTip(QStringLiteral("QQ登录"));

    other_login_buttons2 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/wechat.png"), this);
    other_login_buttons2->setCursor(Qt::PointingHandCursor); ///< 设置发送按钮光标
    other_login_buttons2->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    other_login_buttons2->setCorner(Qt::BottomLeftCorner); ///< 设置按钮位置
    other_login_buttons2->setXOffset(157); ///< 设置 X 偏移
    other_login_buttons2->setYOffset(90); ///< 设置 Y 偏移
    other_login_buttons2->setBackgroundColor(QColor(0xa5bbe4)); ///< 设置背景色
    auto wechat_tooTip = new ElaToolTip(other_login_buttons2);
    wechat_tooTip->setToolTip(QStringLiteral("微信登录"));

    other_login_buttons3 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/logo_google.png"), this);
    other_login_buttons3->setCursor(Qt::PointingHandCursor); ///< 设置发送按钮光标
    other_login_buttons3->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    other_login_buttons3->setCorner(Qt::BottomLeftCorner); ///< 设置按钮位置
    other_login_buttons3->setXOffset(262); ///< 设置 X 偏移
    other_login_buttons3->setYOffset(90); ///< 设置 Y 偏移
    other_login_buttons3->setBackgroundColor(QColor(0xa5bbe4)); ///< 设置背景色
    auto google_tooTip = new ElaToolTip(other_login_buttons3);
    google_tooTip->setToolTip(QStringLiteral("谷歌登录"));

    other_login_buttons4 = new QtMaterialFloatingActionButton(QIcon(":/Res/login/github-fill.png"), this);
    other_login_buttons4->setCursor(Qt::PointingHandCursor); ///< 设置发送按钮光标
    other_login_buttons4->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    other_login_buttons4->setCorner(Qt::BottomLeftCorner); ///< 设置按钮位置
    other_login_buttons4->setXOffset(367); ///< 设置 X 偏移
    other_login_buttons4->setYOffset(90); ///< 设置 Y 偏移
    other_login_buttons4->setBackgroundColor(QColor(0xa5bbe4)); ///< 设置背景色
    auto github_tooTip = new ElaToolTip(other_login_buttons4);
    github_tooTip->setToolTip(QStringLiteral("Github登录"));

    this->animations();
    connect(login_button, &Login_button::execute_animation_signal, this, &Registration_form::execute_animation);

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

void Registration_form::animations() {
    animation = new QPropertyAnimation(this->login_button, "geometry");
    animation->setDuration(250);
    animation->setStartValue(this->login_button->geometry());
    animation->setEndValue(QRect(this->login_button->pos().x() + zoom_rate,
                                 this->login_button->pos().y() + zoom_rate / 2, login_button->width() - zoom_rate * 2,
                                 login_button->height() - zoom_rate));
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Registration_form::execute_animation(Login_button::AnimationState State) {
    if (State == Login_button::Execute) {
        animation->setDirection(QAbstractAnimation::Forward);
        animation->start();
    } else if (State == Login_button::Restore) {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
    }
}

void Registration_form::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    crop_corner(&painter);
    draw_text(&painter);
}

void Registration_form::crop_corner(QPainter *painter) {
    painter->setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter->setBrush(Brush);
    painter->drawRect(0, 0, width(), height());
}

void Registration_form::draw_text(QPainter *painter) {
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(30);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter->setFont(font1);
    QColor semiTransparent(0, 0, 0, 255);
    painter->setPen(semiTransparent);

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, "Registration");
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 8));
    painter->drawText(rect1, "Registration");
}
