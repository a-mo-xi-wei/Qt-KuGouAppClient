#ifndef LOGIN_REGISTER_FORM_H
#define LOGIN_REGISTER_FORM_H

#include "scroll_bar.h"
#include "transparent_transition_interface.h"
#include "registration_form.h"
#include "login_form.h"
#include <QDialog>

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class UIWIDGETS_EXPORT LoginRegisterForm : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(int animation_duration READ animation_duration WRITE setAnimation_duration FINAL)

public:
    explicit LoginRegisterForm(QWidget* parent = nullptr);

    ~LoginRegisterForm() override = default;

    void setIsFirstShow(bool flag);

    bool getIsFirstShow();

private:
    void initUi();

    int animation_duration() const;

    void setAnimation_duration(const int& newAnimation_duration);

    void build_animation();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    bool eventFilter(QObject* watched, QEvent* event) override;

    void showEvent(QShowEvent* event) override;

public slots:
    void setRightShow() const;

    void execute_animation(Hollow_button::AnimationState status);

    void onAnimation3Finished();

    void onAnimation4Finished();

    void accept() override;

signals:
    void exit();

private:
    Scroll_bar* scroll_bar;
    Transparent_transition_interface* transparent_transition_interface;
    Transparent_transition_interface* transparent_transition_interface2;
    Registration_form* registration_form;
    Login_form* login_form;

    QPropertyAnimation* animation2;
    QPropertyAnimation* animation3;
    QPropertyAnimation* animation4;
    QPropertyAnimation* animation5;
    QPropertyAnimation* animation6;

    QPoint m_dragStartPosition;
    QPoint m_startWindowPosition;
    int currentSequence             = 1;
    bool animation_execute_duration = false;
    bool animation_restore_duration = false;
    int m_animation_duration        = 600;

    bool isFirstShow = true;
};

#endif // LOGIN_REGISTER_FORM_H
