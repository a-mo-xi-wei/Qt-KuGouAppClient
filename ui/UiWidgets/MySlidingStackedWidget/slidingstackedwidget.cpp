/**
 * @file slidingstackedwidget.cpp
 * @brief 实现 SlidingStackedWidget 类，提供带滑动动画的堆栈窗口
 * @author Tim Schneeberger (ThePBone) <tim.schneeberger@outlook.de>
 * @date 2020
 * @version 1.0
 *
 * @par MIT License
 * Copyright (c) 2020 Tim Schneeberger (ThePBone) <tim.schneeberger@outlook.de>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "slidingstackedwidget.h"
#include "MySearchLineEdit.h"   ///< 仅仅是为了切换窗口的时候停止扩张动画，否则会闪烁

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QApplication>

/**
 * @brief 构造函数，初始化堆栈窗口
 * @param parent 父控件指针，默认为 nullptr
 */
SlidingStackedWidget::SlidingStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    if (parent != nullptr) {
        m_mainwindow = parent; ///< 设置主窗口
    } else {
        m_mainwindow = this;
    }
}

bool SlidingStackedWidget::isSlideAnimationFinished() const
{
    return !m_active;
}

/**
 * @brief 设置是否使用垂直模式
 * @param vertical 是否为垂直模式，默认为 true
 */
void SlidingStackedWidget::setVerticalMode(const bool vertical)
{
    m_vertical = vertical;
}

/**
 * @brief 设置动画速度
 * @param speed 动画持续时间（毫秒）
 */
void SlidingStackedWidget::setSpeed(int speed)
{
    m_speed = speed;
}

/**
 * @brief 设置缓动曲线
 * @param animationtype 缓动曲线类型
 */
void SlidingStackedWidget::setAnimation(enum QEasingCurve::Type animationtype)
{
    m_animationtype = animationtype;
}

/**
 * @brief 设置是否启用页面循环
 * @param wrap 是否循环，默认为 true
 */
void SlidingStackedWidget::setWrap(bool wrap)
{
    m_wrap = wrap;
}

/**
 * @brief 滑动到下一页
 * @return 是否成功滑动
 */
bool SlidingStackedWidget::slideInNext()
{
    int now = currentIndex();
    if (m_wrap || (now < count() - 1))
        slideInIdx(now + 1); ///< 滑动到下一页
    else
        return false;
    return true;
}

/**
 * @brief 滑动到上一页
 * @return 是否成功滑动
 */
bool SlidingStackedWidget::slideInPrev()
{
    int now = currentIndex();
    if (m_wrap || (now > 0))
        slideInIdx(now - 1); ///< 滑动到上一页
    else
        return false;
    return true;
}

/**
 * @brief 滑动到指定索引的页面
 * @param idx 页面索引
 * @param direction 滑动方向，默认为 AUTOMATIC
 */
void SlidingStackedWidget::slideInIdx(int idx, enum t_direction direction)
{
    if (idx > count() - 1) {
        direction = m_vertical ? TOP2BOTTOM : RIGHT2LEFT; ///< 超出范围时选择默认方向
        idx = (idx) % count();                            ///< 循环索引
    } else if (idx < 0) {
        direction = m_vertical ? BOTTOM2TOP : LEFT2RIGHT;
        idx = (idx + count()) % count();
    }
    slideInWidget(widget(idx), direction); ///< 滑动到目标页面
}

/**
 * @brief 滑动到指定控件页面
 * @param newWidget 目标页面控件
 * @param direction 滑动方向，默认为 AUTOMATIC
 */
void SlidingStackedWidget::slideInWidget(const QWidget *newWidget, enum t_direction direction)
{
    if (m_active) {
        return; ///< 动画进行中，直接返回
    }
    ///< 停止当前页面中所有输入框的动画
    {
        // 停止当前页面中所有输入框的动画
        QWidget *currentPage = currentWidget();
        if (currentPage) {
            // 查找并停止所有 MySearchLineEdit 的动画
            QList<MySearchLineEdit *> lineEdits = currentPage->findChildren<MySearchLineEdit *>();
            for (MySearchLineEdit *edit : lineEdits) {
                edit->stopAnimations();
                edit->resetState();
            }
        }

        // 确保输入框失去焦点
        if (QApplication::focusWidget()) {
            QApplication::focusWidget()->clearFocus();
        }
    }
    m_active = true;

    enum t_direction directionhint;
    int now = currentIndex();
    int next = indexOf(newWidget);
    if (now == next) {
        m_active = false;
        return; ///< 当前页面无需切换
    } else if (now < next) {
        directionhint = m_vertical ? TOP2BOTTOM : RIGHT2LEFT; ///< 自动方向提示
    } else {
        directionhint = m_vertical ? BOTTOM2TOP : LEFT2RIGHT;
    }
    if (direction == AUTOMATIC) {
        direction = directionhint; ///< 使用提示方向
    }

    int offsetx = frameRect().width();
    int offsety = frameRect().height();

    widget(next)->setGeometry(0, 0, offsetx, offsety); ///< 设置下一页面几何尺寸
    if (direction == BOTTOM2TOP) {
        offsetx = 0;
        offsety = -offsety;
    } else if (direction == TOP2BOTTOM) {
        offsetx = 0;
    } else if (direction == RIGHT2LEFT) {
        offsetx = -offsetx;
        offsety = 0;
    } else if (direction == LEFT2RIGHT) {
        offsety = 0;
    }

    QPoint pnext = widget(next)->pos();
    QPoint pnow = widget(now)->pos();
    m_pnow = pnow;
    widget(next)->move(pnext.x() - offsetx, pnext.y() - offsety); ///< 调整下一页面起始位置

    widget(next)->show();
    widget(next)->raise(); ///< 显示并提升下一页面

    // 当前页面动画
    auto *animnow = new QPropertyAnimation(widget(now), "pos");
    animnow->setDuration(m_speed);
    animnow->setEasingCurve(m_animationtype);
    animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
    animnow->setEndValue(QPoint(offsetx + pnow.x(), offsety + pnow.y()));

    // 当前页面透明度动画
    auto *animnow_op_eff = new QGraphicsOpacityEffect();
    widget(now)->setGraphicsEffect(animnow_op_eff);
    auto *animnow_op = new QPropertyAnimation(animnow_op_eff, "opacity");
    animnow_op->setDuration(m_speed / 2);
    animnow_op->setStartValue(1);
    animnow_op->setEndValue(0);
    connect(animnow_op,
            &QPropertyAnimation::finished,
            [=]() {
                if (animnow_op_eff != nullptr) {
                    animnow_op_eff->deleteLater(); ///< 动画结束时清理效果
                }
            });

    // 下一页面透明度动画
    auto *animnext_op_eff = new QGraphicsOpacityEffect();
    animnext_op_eff->setOpacity(0);
    widget(next)->setGraphicsEffect(animnext_op_eff);
    auto *animnext_op = new QPropertyAnimation(animnext_op_eff, "opacity");
    animnext_op->setDuration(m_speed / 2);
    animnext_op->setStartValue(0);
    animnext_op->setEndValue(1);
    connect(animnext_op,
            &QPropertyAnimation::finished,
            [=]() {
                if (animnext_op_eff != nullptr) {
                    animnext_op_eff->deleteLater(); ///< 动画结束时清理效果
                }
            });

    // 下一页面动画
    auto *animnext = new QPropertyAnimation(widget(next), "pos");
    animnext->setDuration(m_speed);
    animnext->setEasingCurve(m_animationtype);
    animnext->setStartValue(QPoint(-offsetx + pnext.x(), offsety + pnext.y()));
    animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

    // 并行动画组
    animgroup = new QParallelAnimationGroup;
    animgroup->addAnimation(animnow);
    animgroup->addAnimation(animnext);
    animgroup->addAnimation(animnow_op);
    animgroup->addAnimation(animnext_op);

    connect(animgroup,
            &QParallelAnimationGroup::finished,
            this,
            &SlidingStackedWidget::animationDoneSlot);
    m_next = next;
    m_now = now;
    m_active = true;
    animgroup->start(QAbstractAnimation::DeleteWhenStopped); ///< 动画结束后自动删除
}

/**
 * @brief 处理动画完成事件
 */
void SlidingStackedWidget::animationDoneSlot()
{
    setCurrentIndex(m_next);     ///< 设置当前页面
    widget(m_now)->hide();       ///< 隐藏上一页面
    widget(m_now)->move(m_pnow); ///< 恢复上一页面位置
    m_active = false;
    emit animationFinished(); ///< 发出动画完成信号
    // qDebug() << "发送动画结束信号"; ///< 调试用，记录信号发送
}