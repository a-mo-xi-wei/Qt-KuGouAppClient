#ifndef LOGIN_REGISTER_FORM_H
#define LOGIN_REGISTER_FORM_H

#include "scroll_bar.h"
#include "transparent_transition_interface.h"
#include "registration_form.h"
#include "login_form.h"

#if defined(LOGIN_REGISTER_LIBRARY)
#define LOGIN_REGISTER_EXPORT Q_DECL_EXPORT
#else
#define LOGIN_REGISTER_EXPORT Q_DECL_IMPORT
#endif

class LOGIN_REGISTER_EXPORT LoginRegisterForm : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animation_duration READ animation_duration WRITE setAnimation_duration FINAL)

public:
    explicit LoginRegisterForm(QWidget *parent = nullptr);

    ~LoginRegisterForm() override = default;

    Scroll_bar *scroll_bar;
    Transparent_transition_interface *transparent_transition_interface;
    Transparent_transition_interface *transparent_transition_interface2;
    Registration_form *registration_form;
    Login_form *login_form;

    void build_animation();

    QPropertyAnimation *animation2;
    QPropertyAnimation *animation3;
    QPropertyAnimation *animation4;
    QPropertyAnimation *animation5;
    QPropertyAnimation *animation6;

    int animation_duration() const;

    void setAnimation_duration(const int& newAnimation_duration);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void setRightShow() const;

    void execute_animation(Hollow_button::AnimationState status);

    void onAnimation3Finished();

    void onAnimation4Finished();

private:
    QPoint m_dragStartPosition;
    QPoint m_startWindowPosition;
    int currentSequence = 1;
    bool animation_execute_duration = false;
    bool animation_restore_duration = false;
    int m_animation_duration = 600;
};

#endif // LOGIN_REGISTER_FORM_H
