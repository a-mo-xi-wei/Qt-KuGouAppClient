#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>

class CenterWidget : public QWidget {
    Q_OBJECT

public:
    explicit CenterWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;
};

#endif // CENTERWIDGET_H
