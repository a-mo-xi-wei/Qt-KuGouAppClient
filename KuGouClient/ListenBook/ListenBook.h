/**
* @file ListenBook.h
 * @brief 定义 ListenBook 类，提供听书主界面
 * @author WeiWang
 * @date 2024-11-18
 * @version 1.0
 */

#ifndef LISTENBOOK_H
#define LISTENBOOK_H

#include "ListenRecommend/ListenRecommend.h"
#include "ListenMyDownload/ListenMyDownload.h"
#include "ListenRecentlyPlay/ListenRecentlyPlay.h"

#include <QWidget>
#include <QPointer>
#include <memory>
#include <array>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ListenBook;
}
QT_END_NAMESPACE

/**
 * @class ListenBook
 * @brief 听书主界面类，包含推荐、下载和最近播放页面的堆栈窗口
 */
class ListenBook : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化听书主界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenBook(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ListenBook() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置按钮图标、文本和指示器样式
     */
    void initUi();

    /**
     * @brief 初始化堆栈窗口
     * @note 创建页面控件并设置按钮互斥
     */
    void initStackedWidget();

    /**
     * @brief 创建页面
     * @param id 页面索引
     * @return 创建的页面控件
     */
    QWidget* createPage(int id);

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(bool flag) const;

private:
    Ui::ListenBook *ui;                                ///< UI 指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;       ///< 按钮组
    std::unique_ptr<ListenRecommend> m_listenRecommend;   ///< 推荐页面
    std::unique_ptr<ListenMyDownload> m_listenMyDownload;  ///< 下载页面
    std::unique_ptr<ListenRecentlyPlay> m_listenRecentlyPlay; ///< 最近播放页面
    std::array<QPointer<QWidget>, 3> m_pages{};        ///< 页面数组
    int m_currentIdx{0};                               ///< 当前页面索引
};

#endif // LISTENBOOK_H