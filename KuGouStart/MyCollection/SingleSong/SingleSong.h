/**
 * @file SingleSong.h
 * @brief 定义 SingleSong 类，管理单曲界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SINGLESONG_H
#define SINGLESONG_H

#include "SortOptionMenu.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class SingleSong;
}
QT_END_NAMESPACE

/**
 * @class SingleSong
 * @brief 单曲界面类，管理单曲的展示和操作（如排序、播放、下载）
 */
class SingleSong : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化单曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SingleSong(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~SingleSong() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示、图标、搜索动作和事件过滤器
     */
    void initUi();

    /**
     * @brief 获取排序菜单位置
     * @param pos 鼠标位置
     * @note 调整菜单位置以适应屏幕
     */
    void getMenuPosition(const QPoint &pos);

private slots:
    /**
     * @brief 播放按钮点击槽函数
     * @note 显示无音乐提示
     */
    void on_collect_play_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_collect_download_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_collect_share_toolButton_clicked();

    /**
     * @brief 专辑按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_collect_album_toolButton_clicked();

    /**
     * @brief 排序按钮点击槽函数
     * @note 显示排序菜单
     */
    void on_collect_sort_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_collect_batch_toolButton_clicked();

    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索更多音乐的信号
     */
    void on_search_pushButton_clicked();

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

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换下载和搜索图标颜色
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    /**
     * @brief 搜索更多音乐的信号
     */
    void find_more_music();

private:
    Ui::SingleSong     *ui;              ///< UI 指针
    QAction            *m_searchAction;   ///< 搜索动作
    SortOptionMenu     *m_sortOptMenu;   ///< 排序选项菜单
    QPoint              m_menuPosition;   ///< 菜单位置
};

#endif // SINGLESONG_H