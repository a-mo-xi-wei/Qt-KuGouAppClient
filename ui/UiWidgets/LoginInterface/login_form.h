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
    explicit Login_form(QWidget* parent = nullptr);

private:
    void initUi();

    void crop_corner(QPainter* painter);

    void draw_text(QPainter* painter);

    void animations();

public slots:
    void execute_animation(Login_button::AnimationState State);

    void onLogin();

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void loginSuccess();

    void exit();

private:
    QPropertyAnimation* animation;
    int zoom_rate            = 20;
    bool m_isPasswordVisible = false;
    Input_box* username;
    Input_box* password;
    AniCheckBox* remember_password_checkBox;
    AniCheckBox* auto_login_checkBox;
    Login_button* login_button;
    QtMaterialFloatingActionButton* other_login_buttons1;
    QtMaterialFloatingActionButton* other_login_buttons2;
    QtMaterialFloatingActionButton* other_login_buttons3;
    QtMaterialFloatingActionButton* other_login_buttons4;
};

#endif // LOGIN_FORM_H
