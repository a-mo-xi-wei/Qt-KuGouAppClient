/**
* @file PlayWidget.h
 * @brief 定义 PlayWidget 类，提供带圆角阴影和双击信号的播放控件
 * @author WeiWang
 * @date 2024-10-13
 * @version 1.0
 */

#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class PlayWidget
 * @brief 自定义播放控件类，继承自 QWidget，支持圆角阴影和双击信号
 */
class MYWINDOW_EXPORT PlayWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化播放控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PlayWidget(QWidget *parent = nullptr);

    signals:
        /**
         * @brief 双击信号，当控件被左键双击时触发
         */
        void doubleClicked();

protected:
    /**
     * @brief 重写鼠标双击事件，触发双击信号
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
     * @param ev 绘制事件
     */
    void paintEvent(QPaintEvent *ev) override;
};

#endif // PLAYWIDGET_H