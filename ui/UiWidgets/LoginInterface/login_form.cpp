#include "login_form.h"
#include "qtmaterialfab.h"
#include "ElaToolTip.h"
#include "CheckBox1.h"
#include "ElaMessageBar.h"
#include "SApp.h"
#include "libhttp.h"

#include <QPainter>
#include <QLabel>
#include <QRegularExpressionValidator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

Login_form::Login_form(QWidget *parent)
    : QWidget{parent}
{
    this->resize(477, 620);

    initUi();

    this->animations();

    auto config = sApp->globalConfig();

    connect(remember_password_checkBox,
            &QCheckBox::stateChanged,
            [ = ](int checked) {
                //如果取消记住密码，则取消自动登陆(没记住密码，哪来的自动登陆)
                if (!checked)
                    auto_login_checkBox->setChecked(false);
                config->setValue("user/rememberPassword", remember_password_checkBox->isChecked());
            });
    connect(auto_login_checkBox,
            &QCheckBox::stateChanged,
            [ = ](int checked) {
                //如果选择了自动登陆，那么自动勾选上记住密码(不记住密码，怎么自动登陆)
                if (checked)
                    remember_password_checkBox->setChecked(true);
                config->setValue("user/autoLogin", auto_login_checkBox->isChecked());
            });

    connect(login_button,
            &Login_button::execute_animation_signal,
            this,
            &Login_form::execute_animation);
    connect(login_button, &Login_button::clicked, this, &Login_form::onLogin);

    connect(username, &QLineEdit::returnPressed, this, &Login_form::onLogin);
    connect(password, &QLineEdit::returnPressed, this, &Login_form::onLogin);

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
}

void Login_form::initUi()
{
    ///< 两个输入框
    username = new Input_box(":/Res/login/account.png", this);
    username->move(46, 161);
    username->setPlaceholderText("Username");
    username->setMaxLength(20);
    username->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression("[a-zA-Z0-9_\\-!@#$%^&*()+=.,?:;\"'{}<>/|]{0,20}"),
            this));
    username->openToolTip();
    username->setIconToolTip(QStringLiteral("用户名"));

    password = new Input_box(":/Res/login/password-lock.png", this);
    password->move(46, 253);
    password->setPlaceholderText("Password");
    password->setEchoMode(QLineEdit::Password);
    password->setMaxLength(16);
    password->setValidator(
        new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+$"), this));
    password->openToolTip();
    password->setIconToolTip(QStringLiteral("解锁"));

    ///< 提示tip
    {
        auto username_cue = new QLabel(this);
        username_cue->setPixmap(QPixmap(":/Res/window/cue-gray.svg"));
        username_cue->setFixedSize(14, 14);
        username_cue->move(
            username->x() + username->width() + 4,
            username->y() + (username->height() - username_cue->height()) / 2
            );
        auto username_cue_toolTip = new ElaToolTip(username_cue);
        username_cue_toolTip->setToolTip("6~20个字符，可包含字母、数字或符号");

        auto password_cue = new QLabel(this);
        password_cue->setPixmap(QPixmap(":/Res/window/cue-gray.svg"));
        password_cue->setFixedSize(14, 14);
        password_cue->move(
            password->x() + password->width() + 4,
            password->y() + (password->height() - password_cue->height()) / 2
            );
        auto password_cue_toolTip = new ElaToolTip(password_cue);
        password_cue_toolTip->setToolTip("必须包含6~16位数字或字母");
    }

    auto config = sApp->globalConfig();

    ///< 记住密码
    remember_password_checkBox = new AniCheckBox(this);
    remember_password_checkBox->setFixedWidth(190);
    remember_password_checkBox->setText("remember password");
    remember_password_checkBox->setStyleSheet(
        "color: #808897; font-size: 15px;");
    remember_password_checkBox->move(
        password->x(),
        // 水平对齐 password 左边
        password->y() + password->height() + 15 // 在 password 下方留出一定间距
        );
    //读取配置文件
    remember_password_checkBox->setChecked(config->value("user/rememberPassword").toBool());

    ///< 下次自动登录
    auto_login_checkBox = new AniCheckBox(this);
    auto_login_checkBox->setFixedWidth(160);
    auto_login_checkBox->setText("auto login");
    auto_login_checkBox->setStyleSheet(
        "color: #808897; font-size: 15px;");
    auto_login_checkBox->move(
        remember_password_checkBox->x() + remember_password_checkBox->width() + 15,
        remember_password_checkBox->y()
        );
    //读取配置文件
    auto_login_checkBox->setChecked(config->value("user/autoLogin").toBool());

    if (remember_password_checkBox->isChecked()) {
        username->setText(config->value("user/account").toString());
        password->setText(config->value("user/password").toString());
    }

    ///< 登录按钮
    login_button = new Login_button(this);
    login_button->setCenter_text("Login");
    login_button->move(46, 371);
    //设置快捷键为Enter，打开登录界面时，可以直接按回车登录
    login_button->setShortcut(Qt::Key::Key_Return);
    login_button->setDefault(true);

    ///< 底部四个登录选项按钮
    {
        other_login_buttons1 = new
            QtMaterialFloatingActionButton(QIcon(":/Res/login/qq.png"), this);
        other_login_buttons1->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
        other_login_buttons1->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
        other_login_buttons1->setCorner(Qt::BottomRightCorner);           ///< 设置按钮位置
        other_login_buttons1->setXOffset(365);                            ///< 设置 X 偏移
        other_login_buttons1->setYOffset(115);                            ///< 设置 Y 偏移
        other_login_buttons1->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
        auto qq_tooTip = new ElaToolTip(other_login_buttons1);
        qq_tooTip->setToolTip(QStringLiteral("QQ登录"));

        other_login_buttons2 = new QtMaterialFloatingActionButton(
            QIcon(":/Res/login/wechat.png"),
            this);
        other_login_buttons2->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
        other_login_buttons2->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
        other_login_buttons2->setCorner(Qt::BottomRightCorner);           ///< 设置按钮位置
        other_login_buttons2->setXOffset(260);                            ///< 设置 X 偏移
        other_login_buttons2->setYOffset(115);                            ///< 设置 Y 偏移
        other_login_buttons2->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
        auto wechat_tooTip = new ElaToolTip(other_login_buttons2);
        wechat_tooTip->setToolTip(QStringLiteral("微信登录"));

        other_login_buttons3 = new QtMaterialFloatingActionButton(
            QIcon(":/Res/login/logo_google.png"),
            this);
        other_login_buttons3->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
        other_login_buttons3->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
        other_login_buttons3->setCorner(Qt::BottomRightCorner);           ///< 设置按钮位置
        other_login_buttons3->setXOffset(155);                            ///< 设置 X 偏移
        other_login_buttons3->setYOffset(115);                            ///< 设置 Y 偏移
        other_login_buttons3->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
        auto google_tooTip = new ElaToolTip(other_login_buttons3);
        google_tooTip->setToolTip(QStringLiteral("Google登录"));

        other_login_buttons4 = new QtMaterialFloatingActionButton(
            QIcon(":/Res/login/github-fill.png"),
            this);
        other_login_buttons4->setCursor(Qt::PointingHandCursor);          ///< 设置发送按钮光标
        other_login_buttons4->setRippleStyle(Material::PositionedRipple); ///< 设置涟漪效果
        other_login_buttons4->setCorner(Qt::BottomRightCorner);           ///< 设置按钮位置
        other_login_buttons4->setXOffset(50);                             ///< 设置 X 偏移
        other_login_buttons4->setYOffset(115);                            ///< 设置 Y 偏移
        other_login_buttons4->setBackgroundColor(QColor(0xa5bbe4));       ///< 设置背景色
        auto github_tooTip = new ElaToolTip(other_login_buttons4);
        github_tooTip->setToolTip(QStringLiteral("Github登录"));
    }
}

void Login_form::animations()
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

void Login_form::execute_animation(Login_button::AnimationState State)
{
    if (State == Login_button::Execute) {
        animation->setDirection(QAbstractAnimation::Forward);
        animation->start();
    } else if (State == Login_button::Restore) {
        animation->setDirection(QAbstractAnimation::Backward);
        animation->start();
    }
}

void Login_form::onLogin()
{
    if (username->text().isEmpty()) {
        username->setFocus();
        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                             "Error",
                             QString("用户名不能为空"),
                             1000,
                             this->window());
        return;
    }
    if (password->text().isEmpty()) {
        password->setFocus();
        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                             "Error",
                             QString("密码不能为空"),
                             1000,
                             this->window());
        return;
    }
    CLibhttp libHttp;
    auto postJson = QJsonObject ///< 创建 JSON 数据
    {
        {"account", username->text()},
        {"password", password->text()}
    };
    QJsonDocument doc(postJson);
    QString jsonString = doc.toJson(QJsonDocument::Compact); ///< 转换为 JSON 字符串
    auto reply = libHttp.UrlRequestPost(
        QStringLiteral("http://127.0.0.1:8080/api/login"),
        jsonString);
    ///< 发送登录请求
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
        ///< 登录成功，禁止鼠标交互
        this->setEnabled(false);
        this->parentWidget()->setEnabled(false);
        ElaMessageBar::success(ElaMessageBarType::Top,
                               "Success",
                               QString("登录成功"),
                               1000,
                               this->window());
        auto token = obj.value("token").toString();
        sApp->setUserData("user/token", token);
        auto config = sApp->globalConfig();
        config->setValue("user/account", username->text());
        config->setValue("user/password", password->text());
        config->setValue("user/rememberPassword", remember_password_checkBox->isChecked());
        config->setValue("user/autoLogin", auto_login_checkBox->isChecked());
        QTimer::singleShot(1000,
                           this,
                           [this] {
                               emit loginSuccess();
                           });
    } else {
        QString message = obj.value("message").toString();
        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                             "Error",
                             QString("%1").arg(message),
                             1000,
                             this->window());

        // qDebug() << reply;
    }
}

void Login_form::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    crop_corner(&painter);
    draw_text(&painter);
}

void Login_form::crop_corner(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    QBrush Brush(QColor(255, 255, 255, 255));
    painter->setBrush(Brush);
    painter->drawRect(0, 0, width(), height());
}

void Login_form::draw_text(QPainter *painter)
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

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, "Login");
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() / 6));
    painter->drawText(rect1, "Login");
}
