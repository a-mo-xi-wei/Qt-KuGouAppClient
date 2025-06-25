/**
* @file ElaBaseListView.h
 * @brief 定义 ElaBaseListView 类，基础列表视图
 * @author WeiWang
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELABASELISTVIEW_H
#define ELABASELISTVIEW_H

#include <QListView>
#include <QModelIndex>

class ElaScrollBar;

/**
 * @class ElaBaseListView
 * @brief 基础列表视图，支持自定义滚动条和鼠标事件信号
 */
class ElaBaseListView : public QListView
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaBaseListView(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaBaseListView() override;

    Q_SIGNALS:
        /**
         * @brief 鼠标按下信号
         * @param index 模型索引
         */
        Q_SIGNAL void mousePress(const QModelIndex &index);

    /**
     * @brief 鼠标释放信号
     * @param index 模型索引
     */
    Q_SIGNAL void mouseRelease(const QModelIndex &index);

    /**
     * @brief 鼠标双击信号
     * @param index 模型索引
     */
    Q_SIGNAL void mouseDoubleClick(const QModelIndex &index);

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 发射 mousePress 信号
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 发射 mouseRelease 信号
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 发射 mouseDoubleClick 信号
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // ELABASELISTVIEW_H