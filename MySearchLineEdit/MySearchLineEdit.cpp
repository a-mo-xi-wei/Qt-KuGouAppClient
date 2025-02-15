//
// Created by WeiWang on 24-10-10.
//

#include "MySearchLineEdit.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPropertyAnimation>

MySearchLineEdit::MySearchLineEdit(QWidget *parent)
    :QLineEdit(parent)
    ,m_animation(new QPropertyAnimation(this,"minimumWidth",this))
    ,m_maxWidth(250)
{
    this->installEventFilter(this);
}

void MySearchLineEdit::setWidth(const int &width) {
    this->m_maxWidth = width;
}

void MySearchLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);

    //使用应用程序级的事件过滤器，在输入框获取焦点时，鼠标点击其他控件时，输入框会丢失焦点，导致输入框收起。
    QApplication::instance()->installEventFilter(this);

    // 动画开始的大小（当前控件大小）
    this->m_startWidth = this->width();
    //qDebug()<<this->m_startWidth;
    // 动画结束的大小，宽度变为 m_maxWidth
    this->m_endWidth = this->m_maxWidth;
    //qDebug()<<this->m_endWidth;
    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_startWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_endWidth);      // 动画的结束值
    this->m_animation->start();
}

void MySearchLineEdit::focusOutEvent(QFocusEvent *event) {
    QLineEdit::focusOutEvent(event);

    //使用应用程序级的事件过滤器，在输入框获取焦点时，鼠标点击其他控件时，输入框会丢失焦点，导致输入框收起。
    QApplication::instance()->installEventFilter(this);

    // 动画结束的大小，宽度还原
    this->m_endWidth = this->m_startWidth;
    // 动画开始的大小（当前控件大小）
    this->m_startWidth = this->width();
    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_startWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_endWidth);      // 动画的结束值
    this->m_animation->start();
}

bool MySearchLineEdit::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto me = static_cast<QMouseEvent*>(event);
        //qDebug()<<"鼠标按下 : "<<mapFromGlobal(me->globalPosition().toPoint());
        if (!rect().contains(mapFromGlobal(me->globalPosition().toPoint()))) {
            this->clearFocus();
            //qDebug()<<"清除焦点";
        }
    }
    return QLineEdit::eventFilter(watched, event);
}

void MySearchLineEdit::keyPressEvent(QKeyEvent *event) {
    if (echoMode() == QLineEdit::Password)
    {
        if (event->matches(QKeySequence::SelectAll))
        {
            return; //如果lineEdit设置成密码的形式，不可以全选
        }
        if (event->matches(QKeySequence::Copy))
        {
            return; //如果lineEdit设置成密码的形式，不可以拷贝
        }
        if (event->matches(QKeySequence::Paste))
        {
            return; //如果lineEdit设置成密码的形式，不可以粘贴
        }
    }
    return QLineEdit::keyPressEvent(event);
}

