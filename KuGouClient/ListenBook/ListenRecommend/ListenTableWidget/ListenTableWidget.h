/**
* @file ListenTableWidget.h
 * @brief 定义 ListenTableWidget 类，提供表格控件
 * @author WeiWang
 * @date 2025-02-07
 * @version 1.0
 */

#ifndef LISTENTABLEWIDGET_H
#define LISTENTABLEWIDGET_H

#include "GalleryWidget.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ListenTableWidget;
}
QT_END_NAMESPACE

/**
 * @class ListenTableWidget
 * @brief 表格控件类，包含标题和画廊，支持刷新功能
 */
class ListenTableWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化表格控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenTableWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ListenTableWidget() override;

    /**
     * @brief 设置标题
     * @param title 标题文本
     */
    void setTitle(const QString &title) const;

    /**
     * @brief 获取画廊控件
     * @return 画廊控件指针
     */
    GalleryWidget *getGalleryWidget() const;

    /**
     * @brief 设置计数
     * @param cnt 计数值
     */
    void setCnt(const int &cnt);

    /**
     * @brief 获取计数
     * @return 计数值
     */
    int getCnt() const;

    /**
     * @brief 获取标题
     * @return 标题文本
     */
    QString getTitle() const;

private:
    /**
     * @brief 初始化界面
     * @note 设置刷新按钮图标和样式
     */
    void initUi();

protected:
    /**
     * @brief 事件过滤器，处理刷新按钮的鼠标事件
     * @param watched 监视的对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    private slots:
        /**
         * @brief 刷新按钮点击槽函数
         * @note 触发刷新信号
         */
        void on_toolButton_clicked();

    signals:
        /**
         * @brief 刷新按钮点击信号
         */
        void toolBtnClicked();

private:
    Ui::ListenTableWidget *ui; ///< UI 指针
    int             m_cnt = 0; ///< 计数值，用于标识表格
};

#endif // LISTENTABLEWIDGET_H