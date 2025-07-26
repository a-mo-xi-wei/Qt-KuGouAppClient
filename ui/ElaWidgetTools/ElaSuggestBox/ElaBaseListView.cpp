/**
 * @file ElaBaseListView.cpp
 * @brief 实现 ElaBaseListView 类，基础列表视图
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaBaseListView.h"
#include "ElaScrollBar.h"

#include <QMouseEvent>

/**
 * @brief 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
ElaBaseListView::ElaBaseListView(QWidget *parent)
    : QListView(parent)
{
    setObjectName("ElaBaseListView");
    setStyleSheet(
        "ElaBaseListView{background-color: transparent;border:0px;}"
        "ElaBaseListView::item{border:none;}");               ///< 设置样式
    setAutoScroll(false);                                     ///< 禁用自动滚动
    setFocusPolicy(Qt::NoFocus);                              ///< 禁用焦点
    setVerticalScrollBar(new ElaScrollBar(this));             ///< 设置垂直滚动条
    setHorizontalScrollBar(new ElaScrollBar(this));           ///< 设置水平滚动条
    setSelectionMode(QAbstractItemView::NoSelection);         ///< 禁用选择
    setMouseTracking(true);                                   ///< 启用鼠标追踪
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);///< 设置水平滚动条策略
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  ///< 设置垂直滚动条策略
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);///< 设置水平滚动模式
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); ///< 设置垂直滚动模式
}

/**
 * @brief 析构函数
 */
ElaBaseListView::~ElaBaseListView()
= default;

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 发射 mousePress 信号
 */
void ElaBaseListView::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT mousePress(indexAt(event->pos()));                 ///< 发射鼠标按下信号
    QListView::mousePressEvent(event);                        ///< 调用父类处理
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 发射 mouseRelease 信号
 */
void ElaBaseListView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_EMIT mouseRelease(indexAt(event->pos()));               ///< 发射鼠标释放信号
    QListView::mouseReleaseEvent(event);                      ///< 调用父类处理
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 发射 mouseDoubleClick 信号
 */
void ElaBaseListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_EMIT mouseDoubleClick(indexAt(event->pos()));           ///< 发射鼠标双击信号
    QListView::mouseDoubleClickEvent(event);                  ///< 调用父类处理
}