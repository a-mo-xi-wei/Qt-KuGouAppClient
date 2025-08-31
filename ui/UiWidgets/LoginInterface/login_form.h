#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H

#include "input_box.h"
#include "login_button.h"

class AniCheckBox;
class QtMaterialFloatingActionButton;

class Login_form : public QWidget
{
    Q_OBJECT

public:
    explicit Login_form(QWidget *parent = nullptr);

    void setDefaultButton(bool flag);

private:
    void initUi();

    void crop_corner(QPainter *painter);

    void draw_text(QPainter *painter);

    void animations();

public slots:
    void execute_animation(Login_button::AnimationState State);

    void onLogin();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void loginSuccess();

    void exit();

    void QQ_login();

    void WeChat_login();

    void Google_login();

    void Github_login();

private:
    QPropertyAnimation *animation;
    int zoom_rate = 20;
    bool m_isPasswordVisible = false;

    Input_box *username;
    Input_box *password;

    AniCheckBox *remember_password_checkBox;
    AniCheckBox *auto_login_checkBox;

    Login_button *login_button;

    QtMaterialFloatingActionButton *QQ_LoginBtn;
    QtMaterialFloatingActionButton *WeChat_LoginBtn;
    QtMaterialFloatingActionButton *Google_LoginBtn;
    QtMaterialFloatingActionButton *Github_LoginBtn;
};

#endif // LOGIN_FORM_H