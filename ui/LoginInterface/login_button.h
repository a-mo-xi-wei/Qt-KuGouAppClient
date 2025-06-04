#ifndef LOGIN_BUTTON_H
#define LOGIN_BUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>

class Login_button : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(int color_opacity READ color_opacity WRITE setColor_opacity FINAL)
    Q_PROPERTY(QString center_text READ center_text WRITE setCenter_text FINAL)

public:
    explicit Login_button(QWidget *parent = nullptr);

    QPropertyAnimation *animation;

    enum AnimationState {
        Execute,
        Restore
    };

    void draw_text();

    void start_animation();

    int color_opacity() const;

    void setColor_opacity(int newColor_opacity);

    QString center_text() const;

    void setCenter_text(const QString &newCenter_text);

protected:
    void paintEvent(QPaintEvent *event) override;

    bool event(QEvent *e) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void execute_animation_signal(Login_button::AnimationState state);

private:
    int m_color_opacity = 255;
    QString m_center_text;
};

#endif // LOGIN_BUTTON_H
