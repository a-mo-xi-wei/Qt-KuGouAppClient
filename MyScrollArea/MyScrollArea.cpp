//
// Created by WeiWang on 24-10-16.
//
#include "MyScrollArea.h"

#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTimer>

MyScrollArea::MyScrollArea(QWidget *parent)
    : QScrollArea(parent)
    , m_upBtn(std::make_unique<UpToolButton>(this))
    , m_scrollBarTimer(new QTimer(this))
    , m_parent(this->window())
{
    //回到最顶部信号
    connect(this->m_upBtn.get(), &QToolButton::clicked, this, &MyScrollArea::onUpBtnClicked);
    connect(this->m_scrollBarTimer, &QTimer::timeout, this, &MyScrollArea::onUpBtnShowOrNot);
}

void MyScrollArea::setAnimating(const bool &animating) {
    this->m_isAnimating = animating;
}

void MyScrollArea::setIgnore(const bool &ignore) {
    this->m_ignore = ignore;
}

void MyScrollArea::setEasingCurve(const QEasingCurve &curve) {
    this->m_curve = curve;
}

void MyScrollArea::wheelEvent(QWheelEvent *event) {
    if (this->verticalScrollBar()) {
        emit wheelValue(this->verticalScrollBar()->value());
        // 启动定时器，延迟处理
        if (!this->m_scrollBarTimer->isActive()) {
            this->m_scrollBarTimer->start(500); // 500ms 延迟，避免过于频繁地触发
        }
    }
    if (!this->m_isAnimating) {
        // 如果没有动画进行，正常处理滚轮事件
        //qDebug() << "MyScrollArea接受滚轮事件";
        if (this->m_ignore) {
            event->ignore();
            return;
        }
        QScrollArea::wheelEvent(event);
    }
    else {
        // 如果正在进行动画，忽略滚轮事件
        event->ignore();
    }
}

void MyScrollArea::resizeEvent(QResizeEvent *event) {
    QScrollArea::resizeEvent(event);
    //UpWidget移动
    this->m_upBtn->move(this->m_parent->width() - this->m_upBtn->width() - 206,
                        this->m_parent->height() - this->m_upBtn->height() - 190);
    this->m_upBtn->raise();
}

void MyScrollArea::onUpBtnClicked() {

    // 标记动画开始
    this->setAnimating(true); //开始禁用滚轮

    auto animation = new QPropertyAnimation(this->verticalScrollBar(), "value", this);
    // 设置动画的起始值（当前滚动条位置）和结束值（最顶部）
    animation->setStartValue(this->verticalScrollBar()->value()); // 当前滚动条位置
    animation->setEndValue(0); // 滚动到顶部（0 表示最上方）
    animation->setDuration(500); // 动画持续时间，500ms
    animation->setEasingCurve(this->m_curve); // 缓动曲线

    // 在动画结束后标记动画停止
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        this->setAnimating(false); //动画结束启用滚轮
    });

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped); // 动画结束后自动删除
}


void MyScrollArea::onUpBtnShowOrNot() {
    if (this->verticalScrollBar()->value() > 200)this->m_upBtn->show();
    else this->m_upBtn->hide();

}
