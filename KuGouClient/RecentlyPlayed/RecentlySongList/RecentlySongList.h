/**
* @file RecentlySongList.h
 * @brief 定义 RecentlySongList 类，管理最近歌单界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef RECENTLYSONGLIST_H
#define RECENTLYSONGLIST_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RecentlySongList;
}
QT_END_NAMESPACE

/**
 * @class RecentlySongList
 * @brief 最近歌单界面类，提供搜索功能
 */
class RecentlySongList : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近歌单界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlySongList(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlySongList() override;

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发搜索音乐信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐的信号
         */
        void find_more_music();

private:
    Ui::RecentlySongList              *ui;                 ///< UI 指针
};

#endif // RECENTLYSONGLIST_H