/**
* @file CenterWidget.h
 * @brief 定义 CenterWidget 类，提供带线性渐变阴影的中心控件
 * @author WeiWang
 * @date 2024-09-18
 * @version 1.0
 */

#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class CenterWidget
 * @brief 自定义中心控件类，继承自 QWidget，支持线性渐变阴影
 */
class UIWIDGETS_EXPORT CenterWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化中心控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit CenterWidget(QWidget *parent = nullptr);

protected:
    /**
     * @brief 重写绘制事件，绘制多段线性渐变矩形
     * @param ev 绘制事件
     */
    void paintEvent(QPaintEvent *ev) override;
};

#endif // CENTERWIDGET_H