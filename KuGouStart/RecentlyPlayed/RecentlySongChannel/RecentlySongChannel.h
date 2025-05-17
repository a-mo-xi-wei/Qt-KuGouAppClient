/**
* @file RecentlySongChannel.h
 * @brief 定义 RecentlySongChannel 类，管理最近歌曲频道界面
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#ifndef RECENTLYSONGCHANNEL_H
#define RECENTLYSONGCHANNEL_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RecentlySongChannel;
}
QT_END_NAMESPACE

/**
 * @class RecentlySongChannel
 * @brief 最近歌曲频道界面类，提供播放、分享、批量操作和搜索功能
 */
class RecentlySongChannel : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近歌曲频道界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlySongChannel(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlySongChannel() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化工具提示、搜索框和频道块
     */
    void initUi();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换搜索图标
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    private slots:
        /**
         * @brief 播放按钮点击槽函数
         * @note 显示无音乐提示
         */
        void on_recently_play_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 显示功能未实现提示
     */
    void on_recently_share_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示功能未实现提示
     */
    void on_recently_batch_toolButton_clicked();

    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索频道信号
     */
    void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多频道的信号
         */
        void find_more_channel();

private:
    Ui::RecentlySongChannel           *ui;                 ///< UI 指针
    QAction                           *m_searchAction{};     ///< 搜索动作
};

#endif // RECENTLYSONGCHANNEL_H