#ifndef ELATOOLTIP_H
#define ELATOOLTIP_H

#include <QWidget>

#include "stdafx.h"

class ElaToolTipPrivate;
class ELA_EXPORT ElaToolTip : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaToolTip)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)
    Q_PROPERTY_CREATE_Q_H(int, ShowDelayMsec)
    Q_PROPERTY_CREATE_Q_H(int, HideDelayMsec)
    Q_PROPERTY_CREATE_Q_H(QString, ToolTip)
    Q_PROPERTY_CREATE_Q_H(QWidget*, CustomWidget)
public:
    explicit ElaToolTip(QWidget* parent = nullptr);
    ~ElaToolTip() override;

    void setToolTipShowAlways();

    void updatePos();

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELATOOLTIP_H
