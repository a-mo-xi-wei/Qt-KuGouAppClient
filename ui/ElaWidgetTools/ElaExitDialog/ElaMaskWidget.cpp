/**
* @file ElaMaskWidget.cpp
 * @brief 实现 ElaMaskWidget 类，提供遮罩效果和动画
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMaskWidget.h"

#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief 构造函数，初始化遮罩控件
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMaskWidget::ElaMaskWidget(QWidget* parent)
    : QWidget{parent}
{
    setObjectName("ElaMaskWidget");
    setStyleSheet("#ElaMaskWidget{background-color:transparent;}");
    _pMaskAlpha = 0;
}

/**
 * @brief 析构函数，释放遮罩控件资源
 */
ElaMaskWidget::~ElaMaskWidget()
{
}

/**
 * @brief 执行遮罩透明度动画
 * @param endValue 动画结束时的透明度值
 */
void ElaMaskWidget::doMaskAnimation(int endValue)
{
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pMaskAlpha");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
        if (endValue == 0)
        {
            setVisible(false);
        }
    });
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(_pMaskAlpha);
    opacityAnimation->setEndValue(endValue);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @brief 重绘事件，绘制遮罩效果
 * @param event 绘图事件
 */
void ElaMaskWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x00, 0x00, 0x00, _pMaskAlpha));
    painter.drawRect(rect());
    painter.restore();
}