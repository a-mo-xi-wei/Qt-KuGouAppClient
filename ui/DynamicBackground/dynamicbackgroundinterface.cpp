#include "dynamicbackgroundinterface.h"

#include <QRandomGenerator>
#include <QTime>
#include <QTimer>

DynamicBackgroundInterface::DynamicBackgroundInterface(QWidget *parent) : QObject(parent), widget(parent),
                                                                          show_ani_progress(0), accumulation(0), accumulation_max(20)
{
    timer = new QTimer(this);
    timer->setInterval(interval = REFRESH_INTERVAL); // 刷新周期
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void DynamicBackgroundInterface::setWidget(QWidget *widget)
{
    this->widget = widget;
}

void DynamicBackgroundInterface::setInterval(int iv)
{
    timer->setInterval(interval = iv);
}

void DynamicBackgroundInterface::setAngle(double a)
{
    angle = PI * a / 180;
}

void DynamicBackgroundInterface::showAni()
{
    QTimer* ani = new QTimer(this);
    ani->setInterval(10);
    ani->start();
    show_ani_progress = 1;
    connect(ani, &QTimer::timeout, [=]{
        show_ani_progress++;
        if (show_ani_progress > 100)
        {
            show_ani_progress = 0;
            ani->stop();
            ani->deleteLater();
        }
        redraw();
    });
}

void DynamicBackgroundInterface::accumulate(int x)
{
    accumulation += x;
}

void DynamicBackgroundInterface::timeout()
{
    redraw();
}

QRect DynamicBackgroundInterface::getGeometry()
{
    if (!widget) return QRect();
    return QRect(-1, -1, widget->size().width()+2, widget->size().height()+2);
}

int DynamicBackgroundInterface::randRange(int min, int max)
{
    if (min > max)
        std::swap(min, max);
    return QRandomGenerator::global()->bounded(min, max + 1); // [min, max]
}

bool DynamicBackgroundInterface::randBool()
{
    return QRandomGenerator::global()->bounded(2) == 1;
}

int DynamicBackgroundInterface::intToUnity(int v)
{
    if (v == 0)
        return 0;
    if (v > 0)
        return 1;
    return -1;
}

void DynamicBackgroundInterface::colorToArray(int *arr, QColor c)
{
    arr[cR] = c.red();
    arr[cG] = c.green();
    arr[cB] = c.blue();
    arr[cA] = c.alpha();
}

void DynamicBackgroundInterface::redraw()
{
    emit signalRedraw();
}

