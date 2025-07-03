#ifndef REGISTRATION_FORM_H
#define REGISTRATION_FORM_H

#include "input_box.h"
#include "login_button.h"

class QtMaterialFloatingActionButton;

class Registration_form : public QWidget {
    Q_OBJECT

public:
    explicit Registration_form(QWidget *parent = nullptr);

    void animations();

    void crop_corner(QPainter *painter);

    void draw_text(QPainter *painter);

public slots:
    void execute_animation(Login_button::AnimationState State);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    QPropertyAnimation *animation;
    int zoom_rate = 20;
    bool m_isPasswordVisible = false;
    Input_box *username;
    Input_box *email;
    Input_box *password;
    Login_button *login_button;
    QtMaterialFloatingActionButton *other_login_buttons1;
    QtMaterialFloatingActionButton *other_login_buttons2;
    QtMaterialFloatingActionButton *other_login_buttons3;
    QtMaterialFloatingActionButton *other_login_buttons4;
};

#endif // REGISTRATION_FORM_H
