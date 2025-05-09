/*
 *  MIT License
    Copyright (c) 2020 Tim Schneeberger (ThePBone) <tim.schneeberger(at)outlook.de>
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 *
 *  Inspired by https://qt.shoutwiki.com/wiki/Extending_QStackedWidget_for_sliding_page_animations_in_Qt
 */

#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QEasingCurve>
#include <QParallelAnimationGroup>

#if defined(SLIDINGSTACKEDWIDGET_LIBRARY)
#define SLIDINGSTACKEDWIDGET_EXPORT Q_DECL_EXPORT
#else
#define SLIDINGSTACKEDWIDGET_EXPORT Q_DECL_IMPORT
#endif

class SLIDINGSTACKEDWIDGET_EXPORT SlidingStackedWidget : public QStackedWidget {
    Q_OBJECT

public:
    //! Animation direction
    enum t_direction {
        LEFT2RIGHT,
        RIGHT2LEFT,
        TOP2BOTTOM,
        BOTTOM2TOP,
        AUTOMATIC
    };

    explicit SlidingStackedWidget(QWidget *parent);

public slots:
    //! Set animation speed
    void setSpeed(int speed);

    //! Set easing curve
    void setAnimation(enum QEasingCurve::Type animationtype);

    //! Change positioning mode
    void setVerticalMode(bool vertical = true);

    //! Enables page wrap for \c slideInNext and \c slideInPrev
    void setWrap(bool wrap);

    //! Slide to next page
    bool slideInNext();

    //! Slide to previous page
    bool slideInPrev();

    //! Slide to page x
    void slideInIdx(int idx, enum t_direction direction = AUTOMATIC);

    //! Slide to page with widget
    void slideInWidget(const QWidget *newWidget, enum t_direction direction = AUTOMATIC);

signals:
    //! Animation is finished
    void animationFinished();

protected slots:
    void animationDoneSlot();

protected:
    QWidget *m_mainwindow{};
    int m_speed = 300;
    enum QEasingCurve::Type m_animationtype = QEasingCurve::OutQuart;
    bool m_vertical = false;
    int m_now = 0;
    int m_next = 0;
    bool m_wrap = false;
    QPoint m_pnow = QPoint(0, 0);
    bool m_active = false;
    QList<QWidget *> blockedPageList{};
    QParallelAnimationGroup *animgroup{};
};
#endif // SLIDINGSTACKEDWIDGET_H
