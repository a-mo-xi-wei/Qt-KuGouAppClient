#include "transparent_transition_interface.h"

#include <QPainter>

Transparent_transition_interface::Transparent_transition_interface(const QString &large_text, const QString &small_text,
                                                                   const QString& btn_text, QWidget *parent)
    : QWidget{parent} {
    this->large_text = large_text;
    this->small_text = small_text;

    this->resize(477, 620);

    button = new Hollow_button(this);
    button->setCenter_text(btn_text);
    QRect rect1(0, 0, 180, 55);
    rect1.moveCenter(QPoint(width() / 2, height() * 0.59));
    button->setGeometry(rect1);
}

void Transparent_transition_interface::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setViewport(0, 0, 477, 620);
    painter.setWindow(0, 0, 477, 620);

    draw_text(&painter);
    draw_text2(&painter);
}

void Transparent_transition_interface::draw_text(QPainter *painter) {
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(29);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter->setFont(font1);
    QColor semiTransparent(255, 255, 255, 255);
    painter->setPen(semiTransparent);

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, large_text);
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() * 0.41));
    painter->drawText(rect1, large_text);
}

void Transparent_transition_interface::draw_text2(QPainter *painter) {
    painter->setRenderHint(QPainter::TextAntialiasing);

    QRect rect1(0, 0, 0, 0);
    QFont font1;
    font1.setPointSize(11);
    font1.setBold(true);
    font1.setWordSpacing(1);
    painter->setFont(font1);
    QColor semiTransparent(255, 255, 255, 255);
    painter->setPen(semiTransparent);

    QRect actualRect = painter->boundingRect(rect1, Qt::AlignCenter, small_text);
    rect1.setHeight(actualRect.height());
    rect1.setWidth(actualRect.width());
    rect1.moveCenter(QPoint(width() / 2, height() * 0.49));
    painter->drawText(rect1, small_text);
}
