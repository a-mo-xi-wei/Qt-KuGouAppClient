#ifndef ELAMESSAGEBUTTON_H
#define ELAMESSAGEBUTTON_H

#include <QPushButton>
#include <QWidget>
#include "Def.h"

class ElaMessageButtonPrivate;
class ELA_EXPORT ElaMessageButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, BarTitle);
    Q_PROPERTY_CREATE_Q_H(QString, BarText);
    Q_PROPERTY_CREATE_Q_H(int, DisplayMsec);
    Q_PROPERTY_CREATE_Q_H(QWidget*, MessageTargetWidget)
    Q_PROPERTY_CREATE_Q_H(ElaMessageBarType::MessageMode, MessageMode);
    Q_PROPERTY_CREATE_Q_H(ElaMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit ElaMessageButton(QWidget* parent = nullptr);
    explicit ElaMessageButton(const QString& text, QWidget* parent = nullptr);
    ~ElaMessageButton();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMESSAGEBUTTON_H
