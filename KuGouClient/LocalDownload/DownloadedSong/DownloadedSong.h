/**
 * @file DownloadedSong.h
 * @brief 定义 DownloadedSong 类，提供已下载歌曲界面
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#ifndef DOWNLOADEDSONG_H
#define DOWNLOADEDSONG_H

#include <QTimer>

#include "SortOptionMenu/SortOptionMenu.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE

namespace Ui {
class DownloadedSong;
}

QT_END_NAMESPACE

/**
 * @class DownloadedSong
 * @brief 已下载歌曲界面类，支持排序和搜索
 */
class DownloadedSong : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化已下载歌曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DownloadedSong(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DownloadedSong() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示、图标和搜索动作
     */
    void initUi();

private slots:
    /**
     * @brief 播放按钮点击槽函数
     * @note 显示暂无音乐提示
     */
    void on_local_play_toolButton_clicked();

    /**
     * @brief 排序按钮点击槽函数
     * @note 显示排序菜单
     */
    void on_local_sort_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_local_batch_toolButton_clicked();

public slots:
    /**
     * @brief 默认排序
     * @note 显示暂无音乐提示
     */
    void onDefaultSort();

    /**
     * @brief 添加时间排序
     * @param down 是否降序
     * @note 显示暂无音乐提示
     */
    void onAddTimeSort(const bool &down);

    /**
     * @brief 歌曲名称排序
     * @param down 是否降序
     * @note 显示暂无音乐提示
     */
    void onSongNameSort(const bool &down);

    /**
     * @brief 歌手排序
     * @param down 是否降序
     * @note 显示暂无音乐提示
     */
    void onSingerSort(const bool &down);

    /**
     * @brief 时长排序
     * @param down 是否降序
     * @note 显示暂无音乐提示
     */
    void onDurationSort(const bool &down);

    /**
     * @brief 播放次数排序
     * @param down 是否降序
     * @note 显示暂无音乐提示
     */
    void onPlayCountSort(const bool &down);

    /**
     * @brief 随机排序
     * @note 显示暂无音乐提示
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
     * @brief 搜索更多音乐信号
     */
    void find_more_music();

    void initialized();

public slots:
    void emitInitialized() { QTimer::singleShot(0, this, [this] { emit initialized(); }); }

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理搜索图标的动态切换
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::DownloadedSong *ui;          ///< UI 指针
    QAction *m_searchAction{};       ///< 搜索动作
    SortOptionMenu *m_sortOptMenu{}; ///< 排序选项菜单
    QPoint m_menuPosition;           ///< 菜单位置
};

#endif // DOWNLOADEDSONG_H