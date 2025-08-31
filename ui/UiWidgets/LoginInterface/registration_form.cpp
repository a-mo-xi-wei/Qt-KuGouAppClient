#include "registration_form.h"
#include "qtmaterialfab.h"
#include "ValidationHint.h"
#include "libhttp.h"
#include "ElaMessageBar.h"

#include <QPainter>
#include <QRegularExpressionValidator>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

Registration_form::Registration_form(QWidget *parent)
    : QWidget{parent}
{
    this->resize(477, 620);

    username = new Input_box(":/Res/login/account.png", this);
    username->move(46, 130);
    username->setPlaceholderText("Username");
    username->openToolTip();
    username->setIconToolTip(QStringLiteral("用户名"));

    email = new Input_box(":/Res/login/email.png", this);
    email->move(46, 220);
    email->setPlaceholderText("Email");
    email->openToolTip();
    email->setIconToolTip(QStringLiteral("邮箱"));

    password = new Input_box(":/Res/login/password.png", this);
    password->move(46, 310);
    password->setPlaceholderText("Password");
    password->setEchoMode(QLineEdit::Password);
    password->openToolTip();
    password->setIconToolTip(QStringLiteral("解锁"));

    auto usernameHint = new ValidationHint(this);
    usernameHint->setFixedWidth(username->width());
    usernameHint->setTargetPos(QPoint(username->x(), username->y() + username->height() + 4));
    // 下方 4px 间距
    usernameHint->setHintText("6~20个字符，可包含字母、数字或符号");
    usernameHint->setStatus(ValidationHint::Neutral);
    usernameHint->hide();

    auto emailHint = new ValidationHint(this);
    emailHint->setFixedWidth(email->width());
    emailHint->setTargetPos(QPoint(email->x(), email->y() + email->height() + 4));
    emailHint->setHintText("请输入有效的邮箱地址");
    emailHint->setStatus(ValidationHint::Neutral);
    emailHint->hide();

    auto passwordHint = new ValidationHint(this);
    passwordHint->setFixedWidth(password->width());
    passwordHint->setTargetPos(QPoint(password->x(), password->y() + password->height() + 4));
    passwordHint->setHintText("密码必须包含6~16位数字或字母");
    passwordHint->setStatus(ValidationHint::Neutral);
    passwordHint->hide();

    login_button = new Login_button(this);
    login_button->setCenter_text("Registration");
    login_button->move(46, 400);
    login_button->setShortcut(Qt::Key::Key_Return);

    QQ_LoginBtn = new QtMaterialFloatingActionButton(QIcon(":/Res/login/qq.png"), this);
    QQ_LoginBtn->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
    QQ_LoginBtn->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    QQ_LoginBtn->setCorner(Qt::BottomLeftCorner);            ///< 设置按钮位置
    QQ_LoginBtn->setXOffset(62);                             ///< 设置 X 偏移
    QQ_LoginBtn->setYOffset(90);                             ///< 设置 Y 偏移
    QQ_LoginBtn->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
    auto qq_tooTip = new ElaToolTip(QQ_LoginBtn);
    qq_tooTip->setToolTip(QStringLiteral("QQ登录"));

    WeChat_LoginBtn = new
        QtMaterialFloatingActionButton(QIcon(":/Res/login/wechat.png"), this);
    WeChat_LoginBtn->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
    WeChat_LoginBtn->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    WeChat_LoginBtn->setCorner(Qt::BottomLeftCorner);            ///< 设置按钮位置
    WeChat_LoginBtn->setXOffset(157);                            ///< 设置 X 偏移
    WeChat_LoginBtn->setYOffset(90);                             ///< 设置 Y 偏移
    WeChat_LoginBtn->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
    auto wechat_tooTip = new ElaToolTip(WeChat_LoginBtn);
    wechat_tooTip->setToolTip(QStringLiteral("微信登录"));

    Google_LoginBtn = new QtMaterialFloatingActionButton(
        QIcon(":/Res/login/logo_google.png"),
        this);
    Google_LoginBtn->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
    Google_LoginBtn->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    Google_LoginBtn->setCorner(Qt::BottomLeftCorner);            ///< 设置按钮位置
    Google_LoginBtn->setXOffset(262);                            ///< 设置 X 偏移
    Google_LoginBtn->setYOffset(90);                             ///< 设置 Y 偏移
    Google_LoginBtn->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
    auto google_tooTip = new ElaToolTip(Google_LoginBtn);
    google_tooTip->setToolTip(QStringLiteral("谷歌登录"));

    Github_LoginBtn = new QtMaterialFloatingActionButton(
        QIcon(":/Res/login/github-fill.png"),
        this);
    Github_LoginBtn->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
    Github_LoginBtn->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
    Github_LoginBtn->setCorner(Qt::BottomLeftCorner);            ///< 设置按钮位置
    Github_LoginBtn->setXOffset(367);                            ///< 设置 X 偏移
    Github_LoginBtn->setYOffset(90);                             ///< 设置 Y 偏移
    Github_LoginBtn->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
    auto github_tooTip = new ElaToolTip(Github_LoginBtn);
    github_tooTip->setToolTip(QStringLiteral("Github登录"));

    ///< 连接相关信号
    connect(QQ_LoginBtn,
            &QtMaterialFloatingActionButton::clicked,
            this,
            &Registration_form::QQ_login);
    connect(WeChat_LoginBtn,
            &QtMaterialFloatingActionButton::clicked,
            this,
            &Registration_form::WeChat_login);
    connect(Google_LoginBtn,
            &QtMaterialFlatButton::clicked,
            this,
            &Registration_form::Google_login);
    connect(Github_LoginBtn,
            &QtMaterialFlatButton::clicked,
            this,
            &Registration_form::Github_login);

    this->animations();
    connect(login_button,
            &Login_button::execute_animation_signal,
            this,
            &Registration_form::execute_animation);
    connect(login_button,
            &Login_button::clicked,
            this,
            [ = ] {
                if (usernameHint->getStatus() == ValidationHint::Valid &&
                    emailHint->getStatus() == ValidationHint::Valid &&
                    passwordHint->getStatus() == ValidationHint::Valid) ///<满足条件，发送注册请求
                {
                    CLibhttp libHttp;
                    auto postJson = QJsonObject ///< 创建 JSON 数据
                    {
                        {"account", username->text()},
                        {"password", password->text()},
                        {"email", email->text()},
                    };
                    QJsonDocument doc(postJson);
                    QString jsonString = doc.toJson(QJsonDocument::Compact); ///< 转换为 JSON 字符串
                    auto reply = libHttp.UrlRequestPost(
                        QStringLiteral("http://127.0.0.1:8080/api/register"),
                        jsonString);
                    ///< 发送添加请求
                    ///< 解析返回的 JSON 数据
                    QJsonParseError parseError;
                    doc = QJsonDocument::fromJson(reply.toUtf8(), &parseError);
                    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
                        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                                             "Error",
                                             QString("JSON 解析错误"),
                                             1000,
                                             this->window());
                        qWarning() << "JSON parse error:" << parseError.errorString();
                        return;
                    }

                    QJsonObject obj = doc.object();
                    QString status = obj.value("status").toString();

                    if (status == "success") {
                        ElaMessageBar::success(ElaMessageBarType::BottomRight,
                                               "Success",
                                               QString("注册成功,请返回登录"),
                                               1000,
                                               this->window());
                    } else {
                        QString message = obj.value("message").toString();
                        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                                             "Error",
                                             QString("%1").arg(message),
                                             1000,
                                             this->window());

                        // qDebug() << reply;
                    }
                } else ///<判空
                {
                    if (username->text().isEmpty()) {
                        username->setFocus(); // 不需要 clearFocus

                        QTimer::singleShot(100,
                                           [this, usernameHint] {
                                               usernameHint->setStatus(
                                                   ValidationHint::Invalid,
                                                   "用户名不能为空");
                                               usernameHint->smoothShow();
                                           });
                    } else if (email->text().isEmpty()) {
                        email->setFocus();
                        QTimer::singleShot(100,
                                           [this, emailHint] {
                                               emailHint->setStatus(
                                                   ValidationHint::Invalid,
                                                   "邮箱不能为空");
                                               emailHint->smoothShow();
                                           });
                    } else if (password->text().isEmpty()) {
                        password->setFocus();
                        QTimer::singleShot(100,
                                           [this, passwordHint] {
                                               passwordHint->setStatus(
                                                   ValidationHint::Invalid,
                                                   "密码不能为空");
                                               passwordHint->smoothShow();
                                           });
                    }
                }
            });

    // 连接图标点击信号
    connect(password,
            &Input_box::iconClicked,
            [this] {
                m_isPasswordVisible = !m_isPasswordVisible;

                password->setIconToolTip(m_isPasswordVisible
                                             ? QStringLiteral("锁定")
                                             : QStringLiteral("解锁"));

                // 切换密码可见性
                password->
                    setEchoMode(m_isPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);

                // 切换图标（需要准备两个图标）
                password->setIcon(m_isPasswordVisible
                                      ? ":/Res/login/password-unlock.png"
                                      : ":/Res/login/password-lock.png");
            });

    connect(username,
            &ElaLineEdit::focusIn,
            [this, usernameHint] {
                usernameHint->smoothShow();
            });
    connect(username,
            &ElaLineEdit::focusOut,
            [this, usernameHint] {
                if (usernameHint->getStatus() != ValidationHint::Invalid)
                    usernameHint->smoothHide();
            });

    connect(email,
            &ElaLineEdit::focusIn,
            [this, emailHint] {
                emailHint->smoothShow();
            });
    connect(email,
            &ElaLineEdit::focusOut,
            [this, emailHint] {
                if (emailHint->getStatus() != ValidationHint::Invalid)
                    emailHint->smoothHide();
            });

    connect(password,
            &ElaLineEdit::focusIn,
            [this, passwordHint] {
                passwordHint->smoothShow();
            });
    connect(password,
            &ElaLineEdit::focusOut,
            [this, passwordHint] {
                if (passwordHint->getStatus() != ValidationHint::Invalid)
                    passwordHint->smoothHide();
            });

    connect(username,
            &QLineEdit::textChanged,
            [this, usernameHint ](const QString &text) {
                if (text.length() == 0) {
                    usernameHint->setStatus(ValidationHint::Neutral);
                    return;
                }
                if (text.length() < 6 || text.length() > 20) {
                    usernameHint->setStatus(ValidationHint::Invalid);
                } else {
                    static QRegularExpression re("[a-zA-Z0-9_\\-!@#$%^&*()+=.,?:;\"'{}<>/|]{0,20}");
                    usernameHint->setStatus(re.match(text).hasMatch()
                                                ? ValidationHint::Valid
                                                : ValidationHint::Invalid);
                }
            });
    connect(email,
            &QLineEdit::textChanged,
            [this, emailHint](const QString &text) {
                if (text.length() == 0) {
                    emailHint->setStatus(ValidationHint::Neutral);
                    return;
                }
                if (text.length() < 6 || text.length() > 30) {
                    emailHint->setStatus(ValidationHint::Invalid);
                } else {
                    static QRegularExpression re("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
                    emailHint->setStatus(re.match(text).hasMatch()
                                             ? ValidationHint::Valid
                                             : ValidationHint::Invalid);
                }
            });
    connect(password,
            &QLineEdit::textChanged,
            [this, passwordHint](const QString &text) {
                if (text.length() == 0) {
                    passwordHint->setStatus(ValidationHint::Neutral);
                    return;
                }
                if (text.length() < 6 || text.length() > 16) {
                    passwordHint->setStatus(ValidationHint::Invalid);
                } else {
                    static QRegularExpression re("[a-zA-Z0-9]+$");
                    passwordHint->setStatus(re.match(text).hasMatch()
                                                ? ValidationHint::Valid
                                                : ValidationHint::Invalid);
                }
            });
}

void Registration_form::setDefaultButton(bool flag)
{
    qDebug() << "设置注册界面 注册按钮默认 ：" << flag;
    login_button->setDefault(flag);
}

void Registration_form::animations()
{
    animation = new QPropertyAnimation(this->login_button, "geometry");
    animation->setDuration(250);
    animation->setStartValue(this->login_button->geometry());
    animation->setEndValue(QRect(this->login_button->pos().x() + zoom_rate,
                                 this->login_button->pos().y() + zoom_rate / 2,
                                 login_button->width() - zoom_rate * 2,
                                 login_button->height() - zoom_rate));
    animation->setEasingCurve(QEasingCurve::Linear);
}

void Registration_form::execute_animation(Login_button::AnimationState State)
{
    if (State == Login_button::Execute) {
        animation->setDirection(QAbstractAnimation::Forward);
        animation->start();
    } else if (State == Login_button::Restore) {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
    }
}

void Registration_form::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    crop_corner(&painter);
    draw_text(&painter);
}

void Registration_form::crop_corner(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter->setBrush(Brush);
    painter->drawRect(0, 0, width(), height());
}

void Registration_form::draw_text(QPainter *painter)
{
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