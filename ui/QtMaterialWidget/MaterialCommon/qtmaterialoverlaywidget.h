/**
* @file qtmaterialoverlaywidget.h
 * @brief 定义 QtMaterialOverlayWidget 类，提供覆盖层控件功能
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALOVERLAYWIDGET_H
#define QTMATERIALOVERLAYWIDGET_H

#include <QWidget>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class QtMaterialOverlayWidget
 * @brief 覆盖层控件类，继承自 QWidget，用于在父窗口上绘制覆盖效果
 * @note 内部实现
 */
class QTMATERIALWIDGET_EXPORT QtMaterialOverlayWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化覆盖层控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialOverlayWidget(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialOverlayWidget();

protected:
    /**
     * @brief 重写事件处理函数，处理父窗口变化
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 重写事件过滤器，同步父窗口几何变化
     * @param obj 事件目标对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 获取覆盖层几何区域
     * @return 覆盖层矩形区域
     */
    virtual QRect overlayGeometry() const;

private:
    Q_DISABLE_COPY(QtMaterialOverlayWidget) ///< 禁用拷贝
};

#endif // QTMATERIALOVERLAYWIDGET_H