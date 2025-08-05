#include "ValidationHint.h"

#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QTimer>

ValidationHint::ValidationHint(QWidget* parent)
    : QWidget(parent)
{
    this->setFixedHeight(15);
    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(12, 12);
    textLabel = new QLabel(this);
    textLabel->setStyleSheet("font-family: 'TaiwanPearl';font-size: 11px;color: gray;letter-spacing: 1px;");

    // 在 ValidationHint 构造函数中添加
    auto effect = new QGraphicsOpacityEffect(iconLabel);
    iconLabel->setGraphicsEffect(effect);
    effect->setOpacity(0.0); // 初始设为透明
    iconLabel->hide();       // 初始不显示

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 0, 0, 0);
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->addStretch();
}

ValidationHint::~ValidationHint() = default;

void ValidationHint::setHintText(const QString& text) const
{
    textLabel->setText(text);
}

void ValidationHint::setStatus(Status status)
{
    m_status = status;

    switch (status)
    {
    case Valid:
        iconLabel->setPixmap(QPixmap(":/Res/window/check-green.svg").scaled(12, 12));
        fadeInIcon();
        textLabel->setStyleSheet("font-family: 'TaiwanPearl'; font-size: 11px; color: green; letter-spacing: 1px;");
        break;

    case Invalid:
        iconLabel->setPixmap(QPixmap(":/Res/window/error-red.svg").scaled(12, 12));
        fadeInIcon();
        textLabel->setStyleSheet("font-family: 'TaiwanPearl'; font-size: 11px; color: red; letter-spacing: 1px;");
        break;

    case Neutral:
        fadeOutIcon();
        textLabel->setStyleSheet("font-family: 'TaiwanPearl'; font-size: 11px; color: gray; letter-spacing: 1px;");
        break;
    }
}

ValidationHint::Status ValidationHint::getStatus()
{
    return m_status;
}

void ValidationHint::smoothShow()
{
    if (isVisible())
        return;

    QPoint startPos = m_targetPos - QPoint(40, 0);
    this->move(startPos);
    this->show();

    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos", this);
    anim->setDuration(200);
    anim->setStartValue(startPos);
    anim->setEndValue(m_targetPos);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ValidationHint::smoothHide()
{
    QPoint endPos = m_targetPos - QPoint(40, 0); // 躲出去左边

    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos", this);
    anim->setDuration(150);
    anim->setStartValue(m_targetPos);
    anim->setEndValue(endPos);
    anim->setEasingCurve(QEasingCurve::InCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // 动画完后恢复位置再隐藏
    QTimer::singleShot(150, this, [this]()
    {
        this->hide();
        this->move(m_targetPos); // 恢复真正显示位置
    });
}

void ValidationHint::setTargetPos(const QPoint& pos)
{
    m_targetPos = pos;
    move(pos); // 默认移到正常位置
}

void ValidationHint::fadeInIcon()
{
    iconLabel->show(); // 必须先显示才能渐入
    auto effect = qobject_cast<QGraphicsOpacityEffect*>(iconLabel->graphicsEffect());
    if (!effect)
        return;

    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setDuration(200);
    anim->setStartValue(effect->opacity());
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ValidationHint::fadeOutIcon()
{
    auto effect = qobject_cast<QGraphicsOpacityEffect*>(iconLabel->graphicsEffect());
    if (!effect)
        return;

    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setDuration(150);
    anim->setStartValue(effect->opacity());
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    // 动画结束后 hide
    connect(anim, &QPropertyAnimation::finished, this, [this]()
    {
        iconLabel->hide();
    });
}
