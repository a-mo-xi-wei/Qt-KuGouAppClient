/**
 * @file ElaScrollBar.h
 * @brief 定义 ElaScrollBar 类，提供自定义滚动条功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELASCROLLBAR_H
#define ELASCROLLBAR_H

#include <QAbstractScrollArea>
#include <QScrollBar>

#include "stdafx.h"

class ElaScrollBarPrivate;

/**
 * @class ElaScrollBar
 * @brief 自定义滚动条类，支持动画和上下文菜单
 */
class ELA_EXPORT ElaScrollBar : public QScrollBar
{
    Q_OBJECT
    Q_Q_CREATE(ElaScrollBar)
    Q_PROPERTY_CREATE_Q_H(bool,   IsAnimation)
    Q_PROPERTY_CREATE_Q_H(qreal,  SpeedLimit)

public:
    /**
     * @brief 构造函数，初始化滚动条
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaScrollBar(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化指定方向的滚动条
     * @param orientation 滚动条方向
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化基于现有滚动条的滚动条
     * @param originScrollBar 原始滚动条
     * @param parent 父滚动区域，默认为 nullptr
     */
    explicit ElaScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);

    /**
     * @brief 析构函数，释放滚动条资源
     */
    ~ElaScrollBar();

Q_SIGNALS:
    /**
     * @brief 范围动画完成信号
     */
    Q_SIGNAL void rangeAnimationFinished();

protected:
    /**
     * @brief 事件处理
     * @param event 事件对象
     * @return 是否处理事件
     */
    virtual bool event(QEvent* event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监视对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件对象
     */
    virtual void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件对象
     */
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件对象
     */
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief 鼠标滚轮事件处理
     * @param event 滚轮事件对象
     */
    virtual void wheelEvent(QWheelEvent* event) override;

    /**
     * @brief 上下文菜单事件处理
     * @param event 上下文菜单事件对象
     */
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELASCROLLBAR_H