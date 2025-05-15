/**
 * @file AniCheckBox.cpp
 * @brief 实现 AniCheckBox 类，提供带动画的复选框基类功能
 * @author [iwxyi]
 * @date 2025-05-15
 * @version 1.0
 */

#include "AniCheckBox.h"
#include "logger.hpp"
#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

/**
 * @brief 构造函数，初始化复选框
 * @param parent 父控件指针，默认为 nullptr
 */
AniCheckBox::AniCheckBox(QWidget *parent) : QCheckBox(parent)
{
    setCursor(Qt::PointingHandCursor);
    connect(this, &QCheckBox::stateChanged, this, [=](int state) {
        qDebug() << "状态改变";
        STREAM_INFO() << "选中框状态改变";
        checkStateChanged(state);
    });
}

/**
 * @brief 设置前景颜色
 * @param c 颜色值
 */
void AniCheckBox::setForeColor(QColor c)
{
    this->foreColor = c;
}

/**
 * @brief 绘制事件，绘制复选框和文字
 * @param e 绘图事件
 */
void AniCheckBox::paintEvent(QPaintEvent *)
{
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

/**
 * @brief 鼠标进入事件，触发悬浮动画
 * @param e 进入事件
 */
void AniCheckBox::enterEvent(QEnterEvent *e)
{
    QCheckBox::enterEvent(e);
    startAnimation("hover_prog", getHoverProg(), 1);
}

/**
 * @brief 鼠标离开事件，触发离开动画
 * @param e 事件
 */
void AniCheckBox::leaveEvent(QEvent *e)
{
    QCheckBox::leaveEvent(e);
    startAnimation("hover_prog", getHoverProg(), 0);
}

/**
 * @brief 判断点击是否在按钮区域
 * @param pos 点击位置
 * @return 是否在按钮区域
 */
bool AniCheckBox::hitButton(const QPoint &) const
{
    return true;
}

/**
 * @brief 复选框状态变化处理
 * @param state 复选框状态
 */
void AniCheckBox::checkStateChanged(const int& state)
{
    if (state == Qt::Unchecked) {
        startAnimation("check_prog", getCheckProg(), 0, 800, QEasingCurve::OutQuad);
    } else if (state == Qt::PartiallyChecked) {
        // 部分选中状态暂未实现
    } else if (state == Qt::Checked) {
        startAnimation("check_prog", getCheckProg(), 1, 500, QEasingCurve::OutBack);
    }
}

/**
 * @brief 绘制复选框
 * @param painter 绘图对象
 * @param rect 绘制区域
 */
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

/**
 * @brief 启动动画
 * @param property 属性名称
 * @param begin 起始值
 * @param end 结束值
 * @param duration 动画时长，默认为 500 毫秒
 * @param curve 缓动曲线，默认为 OutQuad
 * @return 动画对象
 */
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

/**
 * @brief 获取悬浮进度
 * @return 悬浮进度值
 */
double AniCheckBox::getHoverProg() const
{
    return hoverProg;
}

/**
 * @brief 设置悬浮进度
 * @param prog 进度值
 */
void AniCheckBox::setHoverProg(double prog)
{
    this->hoverProg = prog;
}

/**
 * @brief 获取部分选中进度
 * @return 部分选中进度值
 */
double AniCheckBox::getPartProg() const
{
    return partyProg;
}

/**
 * @brief 设置部分选中进度
 * @param prog 进度值
 */
void AniCheckBox::setPartProg(double prog)
{
    this->partyProg = prog;
}

/**
 * @brief 获取选中进度
 * @return 选中进度值
 */
double AniCheckBox::getCheckProg() const
{
    return checkProg;
}

/**
 * @brief 设置选中进度
 * @param prog 进度值
 */
void AniCheckBox::setCheckProg(double prog)
{
    this->checkProg = prog;
}