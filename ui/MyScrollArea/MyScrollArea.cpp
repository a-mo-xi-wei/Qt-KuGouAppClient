/**
 * @file MyScrollArea.cpp
 * @brief 实现 MyScrollArea 类，提供带平滑滚动和返回顶部按钮的滚动区域
 * @author WeiWang
 * @date 2024-10-16
 * @version 1.0
 */

#include "MyScrollArea.h"
#include "logger.hpp"

#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTimer>

/**
 * @brief 构造函数，初始化滚动区域
 * @param parent 父控件指针，默认为 nullptr
 */
MyScrollArea::MyScrollArea(QWidget *parent)
    : QScrollArea(parent)
    , m_upBtn(std::make_unique<UpToolButton>(this))
    , m_scrollBarTimer(new QTimer(this))
{
    this->setMouseTracking(true);
    this->setWidgetResizable(true);

    // 连接返回顶部按钮的点击信号
    connect(this->m_upBtn.get(), &QToolButton::clicked, this, &MyScrollArea::onUpBtnClicked);
    // 连接定时器信号以控制返回顶部按钮的显示
    connect(this->m_scrollBarTimer, &QTimer::timeout, this, &MyScrollArea::onUpBtnShowOrNot);
}

/**
 * @brief 设置动画状态
 * @param animating 是否正在进行动画
 */
void MyScrollArea::setAnimating(const bool &animating) {
    this->m_isAnimating = animating;
}

/**
 * @brief 设置是否忽略滚轮事件
 * @param ignore 是否忽略
 */
void MyScrollArea::setIgnore(const bool &ignore) {
    this->m_ignore = ignore;
}

/**
 * @brief 设置缓动曲线
 * @param curve 缓动曲线类型
 */
void MyScrollArea::setEasingCurve(const QEasingCurve &curve) {
    this->m_curve = curve;
}

/**
 * @brief 平滑滚动到指定位置
 * @param targetValue 目标滚动条值
 */
void MyScrollArea::smoothScrollTo(const int &targetValue) {
    this->setAnimating(true); ///< 禁用滚轮以避免动画干扰

    auto *animation = new QPropertyAnimation(this->verticalScrollBar(), "value");
    animation->setDuration(300); ///< 动画持续 300 毫秒
    animation->setEasingCurve(QEasingCurve::InOutQuad); ///< 使用平滑的二次缓动曲线
    animation->setStartValue(this->verticalScrollBar()->value()); ///< 从当前值开始
    animation->setEndValue(targetValue); ///< 滚动到目标值

    connect(animation, &QPropertyAnimation::finished, this, [=] {
        this->setAnimating(false); ///< 动画结束，启用滚轮
    });

    animation->start(QPropertyAnimation::DeleteWhenStopped); ///< 动画结束后自动删除
}

/**
 * @brief 重写滚轮事件，处理滚动和返回顶部按钮显示逻辑
 * @param event 滚轮事件
 */
void MyScrollArea::wheelEvent(QWheelEvent *event) {
    if (this->verticalScrollBar()) {
        // qDebug() << "value : " << this->verticalScrollBar()->value(); ///< 调试用，记录滚动条值
        emit wheelValue(this->verticalScrollBar()->value()); ///< 发出当前滚动条值信号
        if (!this->m_scrollBarTimer->isActive()) {
            this->m_scrollBarTimer->start(500); ///< 启动 500 毫秒延迟定时器
        }
    }

    if (!this->m_isAnimating) {
        // qDebug() << "MyScrollArea接受滚轮事件"; ///< 调试用，记录滚轮事件
        if (this->m_ignore) {
            event->ignore(); ///< 忽略滚轮事件
            return;
        }
        QScrollArea::wheelEvent(event); ///< 正常处理滚轮事件
    } else {
        event->ignore(); ///< 动画期间忽略滚轮事件
    }
}

/**
 * @brief 重写调整大小事件，更新返回顶部按钮位置
 * @param event 调整大小事件
 */
void MyScrollArea::resizeEvent(QResizeEvent *event) {
    QScrollArea::resizeEvent(event);
    // 更新返回顶部按钮位置，位于右下角
    this->m_upBtn->move(this->width() - this->m_upBtn->width() - 12,
                        this->height() - this->m_upBtn->height() - 20);
    this->m_upBtn->raise(); ///< 确保按钮在最上层
}

/**
 * @brief 重写隐藏事件，停止定时器
 * @param event 隐藏事件
 */
void MyScrollArea::hideEvent(QHideEvent *event) {
    QScrollArea::hideEvent(event);
    this->m_scrollBarTimer->stop(); ///< 停止定时器
}

/**
 * @brief 处理返回顶部按钮点击事件，触发平滑滚动到顶部
 */
void MyScrollArea::onUpBtnClicked() {
    this->setAnimating(true); ///< 禁用滚轮

    auto animation = new QPropertyAnimation(this->verticalScrollBar(), "value", this);
    animation->setStartValue(this->verticalScrollBar()->value()); ///< 从当前值开始
    animation->setEndValue(0); ///< 滚动到顶部
    animation->setDuration(500); ///< 动画持续 500 毫秒
    animation->setEasingCurve(this->m_curve); ///< 使用设置的缓动曲线

    connect(animation, &QPropertyAnimation::finished, this, [=] {
        this->setAnimating(false); ///< 动画结束，启用滚轮
        // qDebug() << "回到顶部动画结束"; ///< 调试用，记录动画结束
        STREAM_INFO() << "回到顶部动画结束"; ///< 记录动画结束日志
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped); ///< 动画结束后自动删除
    // qDebug() << "回到顶部动画开始"; ///< 调试用，记录动画开始
    STREAM_INFO() << "回到顶部动画开始"; ///< 记录动画开始日志
}

/**
 * @brief 处理返回顶部按钮的显示或隐藏逻辑
 */
void MyScrollArea::onUpBtnShowOrNot() {
    if (this->verticalScrollBar()->value() > 200) {
        // qDebug() << "show upBtn pos() : " << this->m_upBtn->geometry(); ///< 调试用，记录按钮位置
        this->m_upBtn->show(); ///< 显示返回顶部按钮
    } else {
        this->m_upBtn->hide(); ///< 隐藏返回顶部按钮
    }
}