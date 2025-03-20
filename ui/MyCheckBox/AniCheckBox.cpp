#include "AniCheckBox.h"
#include "logger.hpp"

#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>

AniCheckBox::AniCheckBox(QWidget *parent) : QCheckBox(parent)
{
    setCursor(Qt::PointingHandCursor);
    connect(this, &QCheckBox::stateChanged, this, [=](int state) {
        qDebug() << "状态改变";
        STREAM_INFO() << "选中框状态改变";
        checkStateChanged(state);
    });
}

void AniCheckBox::setForeColor(QColor c)
{
    this->foreColor = c;
}

void AniCheckBox::paintEvent(QPaintEvent *)
{
    // QCheckBox::paintEvent(e);
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect;
    double textLeft;
    if (boxSide <= 0)
    {
        // 自适应大小：优先一行文字大小，其次按比例
        constexpr double fixedProp = 0.8; // 默认比例
        const QFontMetricsF fm(painter.font());
        double side = fm.height(); // 一行文字的高度
        if (side >= this->height() * fixedProp)
            side = this->height() * fixedProp;

        const double margin = side / 2;
        rect = QRectF(margin, (height() - side) / 2, side, side);
        textLeft = rect.right() + margin;
    }
    else
    {
        // 固定大小
        const double margin = (this->height() - boxSide) / 2;
        rect = QRectF(margin, margin, boxSide, boxSide);
        textLeft = rect.right() + margin;
    }

    // 绘制选择框
    painter.save();
    drawBox(painter, rect);
    painter.restore();

    // 绘制文字
    painter.save();
    painter.drawText(QRectF(textLeft, 0, this->width() - textLeft, this->height()), this->text(), Qt::AlignVCenter | Qt::AlignLeft);
    painter.restore();
}

void AniCheckBox::enterEvent(QEnterEvent *e)
{
    QCheckBox::enterEvent(e);
    startAnimation("hover_prog", getHoverProg(), 1);
}

void AniCheckBox::leaveEvent(QEvent *e)
{
    QCheckBox::leaveEvent(e);
    startAnimation("hover_prog", getHoverProg(), 0);
}

bool AniCheckBox::hitButton(const QPoint &) const
{
    return true;
}

void AniCheckBox::checkStateChanged(const int& state)
{
    if (state == Qt::Unchecked)
    {
        startAnimation("check_prog", getCheckProg(), 0, 800, QEasingCurve::OutQuad);
    }
    else if (state == Qt::PartiallyChecked)
    {

    }
    else if (state == Qt::Checked)
    {
        startAnimation("check_prog", getCheckProg(), 1, 500, QEasingCurve::OutBack);
    }
}

void AniCheckBox::drawBox(QPainter& painter, QRectF rect)
{
    painter.setPen(foreColor);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制边缘方框，和悬浮状态有关
    double radius = 3;
    radius *= (1 - hoverProg);
    painter.drawRoundedRect(rect, radius, radius);

    // 绘制选中状态
    const int state = this->checkState();
    double prop = 0.6;
    prop *= checkProg;
    rect = QRectF(
                rect.left() + rect.width() * (1 - prop) / 2,
                rect.top() + rect.height() * (1 - prop) / 2,
                rect.width() * prop,
                rect.height() * prop
                );
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.fillPath(path, foreColor);

    if (state == Qt::Unchecked)
    {

    }
    else if (state == Qt::PartiallyChecked)
    {

    }
    else if (state == Qt::Checked)
    {

    }
}

QPropertyAnimation *AniCheckBox::startAnimation(const QByteArray &property, double begin, double end, int duration, QEasingCurve curve)
{
    const auto ani = new QPropertyAnimation(this, property);
    ani->setStartValue(begin);
    ani->setEndValue(end);
    ani->setDuration(duration);
    ani->setEasingCurve(curve);
    connect(ani, &QPropertyAnimation::finished, ani, &QObject::deleteLater);
    connect(ani, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    ani->start();
    return ani;
}

double AniCheckBox::getHoverProg() const
{
    return hoverProg;
}

void AniCheckBox::setHoverProg(double prog)
{
    this->hoverProg = prog;
}

double AniCheckBox::getPartProg() const
{
    return partyProg;
}

void AniCheckBox::setPartProg(double prog)
{
    this->partyProg = prog;
}

double AniCheckBox::getCheckProg() const
{
    return checkProg;
}

void AniCheckBox::setCheckProg(double prog)
{
    this->checkProg = prog;
}
