#include "SideHideLabel.h"
#include <QPainter>
#include <QPainterPath>

SideHideLabel::SideHideLabel(QWidget *parent)
    : QLabel(parent)
{
}

SideHideLabel *SideHideLabel::copy(const SideHideLabel *other)
{
    const auto label = new SideHideLabel(other->parentWidget());
    label->setGeometry(other->geometry());
    label->sideOffset = other->sideOffset;
    label->startX = other->startX;
    label->radius = other->radius;
    label->pixmap = other->pixmap;
    return label;
}

void SideHideLabel::setPixmap(const QPixmap &pixmap,const double& sideMaxOffset)
{
//    QLabel::setPixmap(pixmap);
    this->pixmap = pixmap;
    this->startX = -sideMaxOffset; // 单边的长度
}

double SideHideLabel::getMaxOffset() const
{
    return -startX;
}

void SideHideLabel::paintEvent(QPaintEvent *e)
{
//    QLabel::paintEvent(e);
//    return ;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius);
    painter.setClipPath(path);

    const QPixmap newPixmap = pixmap.scaledToHeight(this->height(), Qt::FastTransformation);

    painter.drawPixmap(static_cast<int>(startX + sideOffset), 0,
                       newPixmap.width(), newPixmap.height(),
                       newPixmap);
}

void SideHideLabel::setSideOffset(const double& of)
{
    this->sideOffset = of;
    update();
}

double SideHideLabel::getSideOffset() const
{
    return sideOffset;
}

