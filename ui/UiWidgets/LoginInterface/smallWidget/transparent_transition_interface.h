#ifndef TRANSPARENT_TRANSITION_INTERFACE_H
#define TRANSPARENT_TRANSITION_INTERFACE_H

#include <QRegularExpressionValidator>
#include "hollow_button.h"

class Transparent_transition_interface : public QWidget {
    Q_OBJECT

public:
    explicit Transparent_transition_interface(const QString &large_text, const QString &small_text, const QString& btn_text,
                                              QWidget *parent = nullptr);

    void draw_text(QPainter *painter);

    void draw_text2(QPainter *painter);

    Hollow_button *button;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString large_text;
    QString small_text;
};

#endif // TRANSPARENT_TRANSITION_INTERFACE_H
