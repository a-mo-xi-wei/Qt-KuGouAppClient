/**
* @file RecentlyMVChannel.h
 * @brief 定义 RecentlyMVChannel 类，管理最近MV频道界面
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#ifndef RECENTLYMVCHANNEL_H
#define RECENTLYMVCHANNEL_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RecentlyMVChannel;
}
QT_END_NAMESPACE

/**
 * @class RecentlyMVChannel
 * @brief 最近MV频道界面类，提供搜索功能
 */
class RecentlyMVChannel : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近MV频道界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlyMVChannel(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlyMVChannel() override;

    private slots:
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
    Ui::RecentlyMVChannel             *ui;                 ///< UI 指针
};

#endif // RECENTLYMVCHANNEL_H