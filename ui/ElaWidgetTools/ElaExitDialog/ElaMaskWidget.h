#ifndef ELAMASKWIDGET_H
#define ELAMASKWIDGET_H

#include <QWidget>

#include "stdafx.h"

class ELA_EXPORT ElaMaskWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MaskAlpha)
public:
    explicit ElaMaskWidget(QWidget* parent = nullptr);
    ~ElaMaskWidget();
    void doMaskAnimation(int endValue);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMASKWIDGET_H
