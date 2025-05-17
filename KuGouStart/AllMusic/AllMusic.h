/**
 * @file AllMusic.h
 * @brief 定义 AllMusic 类，提供音乐管理界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef ALLMUSIC_H
#define ALLMUSIC_H

#include "AllWidget/AllWidget.h"
#include "AllLove/AllLove.h"
#include "AllSongList/AllSongList.h"
#include "AllRecent/AllRecent.h"
#include "AllLocal/AllLocal.h"
#include "AllPaid/AllPaid.h"
#include "AllCloudDisk/AllCloudDisk.h"
#include "SortOptionMenu.h"

#include <QWidget>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AllMusic;
}
QT_END_NAMESPACE

/**
 * @class AllMusic
 * @brief 音乐管理界面类，支持全部、收藏、歌单、最近、本地、已购、云盘子界面切换及排序
 */
class AllMusic : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐管理界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AllMusic(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AllMusic() override;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化索引标签
     */
    void initIndexLab();

    /**
     * @brief 初始化堆栈窗口
     */
    void initStackedWidget();

    /**
     * @brief 初始化全部音乐控件
     */
    void initAllWidget();

    /**
     * @brief 初始化收藏音乐控件
     */
    void initAllLove();

    /**
     * @brief 初始化歌单控件
     */
    void initAllSongList();

    /**
     * @brief 初始化最近播放控件
     */
    void initAllRecent();

    /**
     * @brief 初始化本地音乐控件
     */
    void initAllLocal();

    /**
     * @brief 初始化已购音乐控件
     */
    void initAllPaid();

    /**
     * @brief 初始化云盘音乐控件
     */
    void initAllCloudDisk();

    /**
     * @brief 获取菜单显示位置
     * @param pos 鼠标位置
     */
    void getMenuPosition(const QPoint &pos);

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    /**
     * @brief 处理全部按钮点击
     */
    void on_all_pushButton_clicked();

    /**
     * @brief 处理收藏按钮点击
     */
    void on_love_pushButton_clicked();

    /**
     * @brief 处理歌单按钮点击
     */
    void on_song_list_pushButton_clicked();

    /**
     * @brief 处理最近按钮点击
     */
    void on_recent_pushButton_clicked();

    /**
     * @brief 处理本地按钮点击
     */
    void on_local_pushButton_clicked();

    /**
     * @brief 处理已购按钮点击
     */
    void on_paid_pushButton_clicked();

    /**
     * @brief 处理云盘按钮点击
     */
    void on_cloud_disk_pushButton_clicked();

    /**
     * @brief 处理全部播放按钮点击
     */
    void on_all_play_toolButton_clicked();

    /**
     * @brief 处理下载按钮点击
     */
    void on_all_download_toolButton_clicked();

    /**
     * @brief 处理分享按钮点击
     */
    void on_all_share_toolButton_clicked();

    /**
     * @brief 处理排序按钮点击
     */
    void on_all_sort_toolButton_clicked();

    /**
     * @brief 处理批量操作按钮点击
     */
    void on_all_batch_toolButton_clicked();

public slots:
    /**
     * @brief 默认排序
     */
    void onDefaultSort();

    /**
     * @brief 按添加时间排序
     * @param down 是否降序
     */
    void onAddTimeSort(const bool &down);

    /**
     * @brief 按歌曲名称排序
     * @param down 是否降序
     */
    void onSongNameSort(const bool &down);

    /**
     * @brief 按歌手排序
     * @param down 是否降序
     */
    void onSingerSort(const bool &down);

    /**
     * @brief 按时长排序
     * @param down 是否降序
     */
    void onDurationSort(const bool &down);

    /**
     * @brief 按播放次数排序
     * @param down 是否降序
     */
    void onPlayCountSort(const bool &down);

    /**
     * @brief 随机排序
     */
    void onRandomSort();

signals:
    /**
     * @brief 搜索更多音乐信号
     */
    void find_more_music();

private:
    Ui::AllMusic *ui;                                    ///< UI 指针
    std::unique_ptr<QButtonGroup>   m_buttonGroup;       ///< 按钮组
    std::unique_ptr<AllWidget>      m_allWidget;         ///< 全部音乐控件
    std::unique_ptr<AllLove>        m_allLove;           ///< 收藏音乐控件
    std::unique_ptr<AllSongList>    m_allSongList;       ///< 歌单控件
    std::unique_ptr<AllRecent>      m_allRecent;         ///< 最近播放控件
    std::unique_ptr<AllLocal>       m_allLocal;          ///< 本地音乐控件
    std::unique_ptr<AllPaid>        m_allPaid;           ///< 已购音乐控件
    std::unique_ptr<AllCloudDisk>   m_allCloudDisk;      ///< 云盘音乐控件
    QAction *m_searchAction{};                             ///< 搜索动作
    SortOptionMenu *m_sortOptMenu{};                       ///< 排序选项菜单
    QPoint m_menuPosition;                               ///< 菜单显示位置
};

#endif // ALLMUSIC_H