/**
 * @file slidingstackedwidget.h
 * @brief 定义 SlidingStackedWidget 类，提供带滑动动画的堆栈窗口
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
 *
 * @note Inspired by https://qt.shoutwiki.com/wiki/Extending_QStackedWidget_for_sliding_page_animations_in_Qt
 */

#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QEasingCurve>
#include <QParallelAnimationGroup>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(SLIDINGSTACKEDWIDGET_LIBRARY)
#define SLIDINGSTACKEDWIDGET_EXPORT Q_DECL_EXPORT
#else
#define SLIDINGSTACKEDWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class SlidingStackedWidget
 * @brief 自定义堆栈窗口类，继承自 QStackedWidget，支持页面滑动动画
 */
class SLIDINGSTACKEDWIDGET_EXPORT SlidingStackedWidget : public QStackedWidget {
    Q_OBJECT

public:
    /**
     * @brief 动画方向枚举
     */
    enum t_direction {
        LEFT2RIGHT,   ///< 从左到右
        RIGHT2LEFT,   ///< 从右到左
        TOP2BOTTOM,   ///< 从上到下
        BOTTOM2TOP,   ///< 从下到上
        AUTOMATIC     ///< 自动选择方向
    };

    /**
     * @brief 构造函数，初始化堆栈窗口
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SlidingStackedWidget(QWidget *parent = nullptr);

public slots:
    /**
     * @brief 设置动画速度
     * @param speed 动画持续时间（毫秒）
     */
    void setSpeed(int speed);

    /**
     * @brief 设置缓动曲线
     * @param animationtype 缓动曲线类型
     */
    void setAnimation(enum QEasingCurve::Type animationtype);

    /**
     * @brief 设置是否使用垂直模式
     * @param vertical 是否为垂直模式，默认为 true
     */
    void setVerticalMode(bool vertical = true);

    /**
     * @brief 设置是否启用页面循环
     * @param wrap 是否循环，默认为 true
     */
    void setWrap(bool wrap);

    /**
     * @brief 滑动到下一页
     * @return 是否成功滑动
     */
    bool slideInNext();

    /**
     * @brief 滑动到上一页
     * @return 是否成功滑动
     */
    bool slideInPrev();

    /**
     * @brief 滑动到指定索引的页面
     * @param idx 页面索引
     * @param direction 滑动方向，默认为 AUTOMATIC
     */
    void slideInIdx(int idx, enum t_direction direction = AUTOMATIC);

    /**
     * @brief 滑动到指定控件页面
     * @param newWidget 目标页面控件
     * @param direction 滑动方向，默认为 AUTOMATIC
     */
    void slideInWidget(const QWidget *newWidget, enum t_direction direction = AUTOMATIC);

signals:
    /**
     * @brief 动画完成信号
     */
    void animationFinished();

protected slots:
    /**
     * @brief 处理动画完成事件
     */
    void animationDoneSlot();

protected:
    QWidget *m_mainwindow{};                  ///< 主窗口指针
    int m_speed = 300;                        ///< 动画速度（毫秒）
    enum QEasingCurve::Type m_animationtype = QEasingCurve::OutQuart; ///< 缓动曲线类型
    bool m_vertical = false;                  ///< 是否垂直模式
    int m_now = 0;                            ///< 当前页面索引
    int m_next = 0;                           ///< 下一页面索引
    bool m_wrap = false;                      ///< 是否启用页面循环
    QPoint m_pnow = QPoint(0, 0);             ///< 当前页面位置
    bool m_active = false;                    ///< 是否正在进行动画
    QList<QWidget *> blockedPageList{};       ///< 阻止切换的页面列表
    QParallelAnimationGroup *animgroup{};     ///< 并行动画组
};

#endif // SLIDINGSTACKEDWIDGET_H