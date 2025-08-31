#ifndef REGISTRATION_FORM_H
#define REGISTRATION_FORM_H

#include "input_box.h"
#include "login_button.h"

class QtMaterialFloatingActionButton;

class Registration_form : public QWidget
{
    Q_OBJECT

public:
    explicit Registration_form(QWidget *parent = nullptr);

    void setDefaultButton(bool flag);

private:
    void animations();

    void crop_corner(QPainter *painter);

    void draw_text(QPainter *painter);

public slots:
    void execute_animation(Login_button::AnimationState State);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void exit();

    void QQ_login();

    void WeChat_login();

    void Google_login();

    void Github_login();

public:
    QPropertyAnimation *animation;
    int zoom_rate = 20;
    bool m_isPasswordVisible = false;
    bool m_forceInvalidHint = false;

    Input_box *username;
    Input_box *email;
    Input_box *password;

    Login_button *login_button;

    QtMaterialFloatingActionButton *QQ_LoginBtn;
    QtMaterialFloatingActionButton *WeChat_LoginBtn;
    QtMaterialFloatingActionButton *Google_LoginBtn;
    QtMaterialFloatingActionButton *Github_LoginBtn;
};

#endif // REGISTRATION_FORM_H