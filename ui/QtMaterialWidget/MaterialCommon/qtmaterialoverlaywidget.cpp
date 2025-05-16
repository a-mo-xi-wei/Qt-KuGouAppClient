/**
* @file qtmaterialoverlaywidget.cpp
 * @brief 实现 QtMaterialOverlayWidget 类，提供覆盖层控件功能
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialoverlaywidget.h"
#include <QEvent>

/**
 * @brief 构造函数，初始化覆盖层控件
 * @param parent 父控件指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialOverlayWidget::QtMaterialOverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    if (parent) {
        parent->installEventFilter(this); ///< 安装父窗口事件过滤器
    }
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialOverlayWidget::~QtMaterialOverlayWidget()
{
}

/**
 * @brief 重写事件处理函数，处理父窗口变化
 * @param event 事件对象
 * @return 是否处理事件
 */
bool QtMaterialOverlayWidget::event(QEvent *event)
{
    if (!parent()) {
        return QWidget::event(event);
    }
    switch (event->type()) {
        case QEvent::ParentChange:
            parent()->installEventFilter(this); ///< 安装新父窗口的事件过滤器
        setGeometry(overlayGeometry()); ///< 更新几何区域
        break;
        case QEvent::ParentAboutToChange:
            parent()->removeEventFilter(this); ///< 移除旧父窗口的事件过滤器
        break;
        default:
            break;
    }
    return QWidget::event(event);
}

/**
 * @brief 重写事件过滤器，同步父窗口几何变化
 * @param obj 事件目标对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool QtMaterialOverlayWidget::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
        case QEvent::Move:
        case QEvent::Resize:
            setGeometry(overlayGeometry()); ///< 同步父窗口几何变化
        break;
        default:
            break;
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief 获取覆盖层几何区域
 * @return 覆盖层矩形区域
 */
QRect QtMaterialOverlayWidget::overlayGeometry() const
{
    QWidget *widget = parentWidget();
    if (!widget) {
        return QRect(); ///< 无父窗口返回空矩形
    }
    return widget->rect(); ///< 返回父窗口矩形
}