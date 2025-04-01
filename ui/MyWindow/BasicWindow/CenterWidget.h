#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>

#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

class MYWINDOW_EXPORT CenterWidget : public QWidget {
    Q_OBJECT

public:
    explicit CenterWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *ev) override;
};

#endif // CENTERWIDGET_H
