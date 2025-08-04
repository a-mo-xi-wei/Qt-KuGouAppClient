#include "ElaSlider.h"
#include "ElaSliderStyle.h"

#include <QEvent>

ElaSlider::ElaSlider(QWidget* parent)
    : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    setStyle(new ElaSliderStyle(style()));
}

ElaSlider::ElaSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setStyle(new ElaSliderStyle(style()));
}

ElaSlider::~ElaSlider() = default;
