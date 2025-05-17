/**
 * @file PaidSingle.h
 * @brief 定义 PaidSingle 类，管理付费单曲界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef PAIDSINGLE_H
#define PAIDSINGLE_H

#include "SortOptionMenu.h"

#include <QWidget>
#include <QPoint>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class PaidSingle;
}
QT_END_NAMESPACE

/**
 * @class PaidSingle
 * @brief 付费单曲界面类，提供播放、下载、分享、排序、批量操作和搜索功能
 */
class PaidSingle : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化付费单曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PaidSingle(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~PaidSingle() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化工具按钮、搜索框、工具提示和事件过滤器
     */
    void initUi();

    /**
     * @brief 获取排序菜单位置
     * @param pos 鼠标位置
     * @note 调整菜单位置以适应屏幕边界
     */
    void getMenuPosition(const QPoint &pos);

private slots:
    /**
     * @brief 播放按钮点击槽函数
     * @note 显示无音乐提示
     */
    void on_single_play_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     * @note 显示功能未实现提示
     */
    void on_single_download_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 显示功能未实现提示
     */
    void on_single_share_toolButton_clicked();

    /**
     * @brief 排序按钮点击槽函数
     * @note 弹出排序菜单
     */
    void on_single_sort_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示功能未实现提示
     */
    void on_single_batch_toolButton_clicked();

public slots:
    /**
     * @brief 默认排序槽函数
     * @note 显示无音乐提示
     */
    void onDefaultSort();

    /**
     * @brief 添加时间排序槽函数
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onAddTimeSort(const bool &down);

    /**
     * @brief 歌曲名称排序槽函数
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onSongNameSort(const bool &down);

    /**
     * @brief 歌手排序槽函数
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onSingerSort(const bool &down);

    /**
     * @brief 时长排序槽函数
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onDurationSort(const bool &down);

    /**
     * @brief 播放次数排序槽函数
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onPlayCountSort(const bool &down);

    /**
     * @brief 随机排序槽函数
     * @note 显示无音乐提示
     */
    void onRandomSort();

private slots:
    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索信号
     */
    void on_search_pushButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐的信号
     */
    void find_more_music();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换下载和搜索图标
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::PaidSingle                     *ui;                 ///< UI 指针
    QAction                            *m_searchAction{};     ///< 搜索动作
    SortOptionMenu                     *m_sortOptMenu{};      ///< 排序菜单
    QPoint                              m_menuPosition;     ///< 菜单位置
};

#endif // PAIDSINGLE_H