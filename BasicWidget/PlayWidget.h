#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QPaintEvent>

class PlayWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlayWidget(QWidget *parent = nullptr);

signals:
    void doubleClicked(); // 自定义的双击信号

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;
};

#endif // PLAYWIDGET_H
