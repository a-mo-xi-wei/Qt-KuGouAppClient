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
#include <memory>

/**
 * @class QButtonGroup
 * @brief 按钮组类，用于管理互斥按钮
 */
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
     * @brief 初始化推荐页面
     * @note 创建推荐页面控件并添加到堆栈窗口
     */
    void initListenRecommend();

    /**
     * @brief 初始化下载页面
     * @note 创建下载页面控件并连接切换信号
     */
    void initListenMyDownload();

    /**
     * @brief 初始化最近播放页面
     * @note 创建最近播放页面控件并连接切换信号
     */
    void initListenRecentlyPlay();

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

private slots:
    /**
     * @brief 推荐按钮点击槽函数
     * @note 切换到推荐页面并触发动画
     */
    void on_listen_recommend_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     * @note 切换到下载页面并触发动画
     */
    void on_listen_my_download_toolButton_clicked();

    /**
     * @brief 最近播放按钮点击槽函数
     * @note 切换到最近播放页面并触发动画
     */
    void on_recently_play_toolButton_clicked();

private:
    Ui::ListenBook                       *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>         m_buttonGroup{};       ///< 按钮组
    std::unique_ptr<ListenRecommend>      m_listenRecommend{};   ///< 推荐页面
    std::unique_ptr<ListenMyDownload>     m_listenMyDownload{};  ///< 下载页面
    std::unique_ptr<ListenRecentlyPlay>   m_listenRecentlyPlay{}; ///< 最近播放页面
};

#endif // LISTENBOOK_H