#ifndef HOLLOW_BUTTON_H
#define HOLLOW_BUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>

class Hollow_button : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(QString center_text READ center_text WRITE setCenter_text FINAL)
    Q_PROPERTY(int radius READ radius WRITE setRadius FINAL)
    Q_PROPERTY(int opacity READ opacity WRITE setOpacity FINAL)

public:
    explicit Hollow_button(QWidget *parent = nullptr);

    enum AnimationState {
        ANIMATION_STATE_EXECUTING,
        ANIMATION_STATE_RESET
    };

    QString center_text() const;

    void setCenter_text(const QString &newCenter_text);

    int radius() const;

    void setRadius(int newRadius);

    int opacity() const;

    void setOpacity(int newOpacity);

    void reset_animation();

    void execute_animation();

    void animation_status(bool status);

private:
    void draw_disappearing_circle(QPainter *event);

    void draw_border(QPainter *event);

    void draw_text(QPainter *event);

signals:
    void page_changed(AnimationState status);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint mouse_coordinates;
    QPropertyAnimation *animation1;
    QPropertyAnimation *animation3;
    QString m_center_text;
    int m_radius = 0;
    int m_opacity = 255;
    bool status = true;
};

#endif // HOLLOW_BUTTON_H
