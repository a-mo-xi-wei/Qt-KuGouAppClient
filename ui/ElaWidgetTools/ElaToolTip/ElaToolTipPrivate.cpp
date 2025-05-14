/**
 * @file ElaToolTipPrivate.cpp
 * @brief 实现 ElaToolTipPrivate 类，管理工具提示的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaToolTipPrivate.h"
#include "ElaToolTip.h"

#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>

/**
 * @brief 构造函数，初始化工具提示私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaToolTipPrivate::ElaToolTipPrivate(QObject* parent)
    : QObject{parent}
{
    _pOpacity = 1;
}

/**
 * @brief 析构函数，释放工具提示私有资源
 */
ElaToolTipPrivate::~ElaToolTipPrivate()
{
}

/**
 * @brief 事件过滤器
 * @param watched 监视对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool ElaToolTipPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaToolTip);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        QTimer::singleShot(_pShowDelayMsec, this, [=]() {
            _doShowAnimation();
        });
        if (_pDisplayMsec > -1)
        {
            QTimer::singleShot(_pDisplayMsec, this, [=]() {
                q->hide();
            });
        }
        break;
    }
    case QEvent::Leave:
    {
        QTimer::singleShot(_pHideDelayMsec, this, [=]() {
            q->hide();
        });
        break;
    }
    case QEvent::HoverLeave:
    {
        QTimer::singleShot(_pHideDelayMsec, this, [=]() {
            q->hide();
        });
        break;
    }
    case QEvent::HoverMove:
    case QEvent::MouseMove:
    {
        _updatePos();
        break;
    }
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(watched, event);
}

/**
 * @brief 执行显示动画
 */
void ElaToolTipPrivate::_doShowAnimation()
{
    Q_Q(ElaToolTip);
    QPoint cursorPoint = QCursor::pos();
    q->move(cursorPoint.x() + 10, cursorPoint.y());
    q->show();
    QPropertyAnimation* showAnimation = new QPropertyAnimation(q, "windowOpacity");
    showAnimation->setEasingCurve(QEasingCurve::InOutSine);
    showAnimation->setDuration(250);
    showAnimation->setStartValue(0);
    showAnimation->setEndValue(1);
    showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

/**
 * @brief 更新工具提示位置
 */
void ElaToolTipPrivate::_updatePos()
{
    Q_Q(ElaToolTip);
    if (q->isVisible())
    {
        QPoint cursorPoint = QCursor::pos();
        q->move(cursorPoint.x() + 10, cursorPoint.y());
    }
}