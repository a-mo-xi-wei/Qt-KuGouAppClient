#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

#include <QWidget>

class Scroll_bar : public QWidget {
    Q_OBJECT

public:
    explicit Scroll_bar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    void crop_corner(QPainter *painter);
};

#endif // SCROLL_BAR_H
