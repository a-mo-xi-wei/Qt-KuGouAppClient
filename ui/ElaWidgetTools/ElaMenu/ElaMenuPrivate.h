#ifndef ELAMENUPRIVATE_H
#define ELAMENUPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "stdafx.h"

class ElaMenu;
class ElaMenuStyle;

class ElaMenuPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaMenu)

    Q_PROPERTY_CREATE(int, AnimationImagePosY)

public:
    explicit ElaMenuPrivate(QObject *parent = nullptr);

    ~ElaMenuPrivate() override;

private:
    QPixmap _animationPix;
    bool _isCloseAnimation{false};
    QPoint _mousePressPoint;
    ElaMenuStyle *_menuStyle{nullptr};
    bool m_preventHide = false;// 是否阻止隐藏
};

#endif // ELAMENUPRIVATE_H
