/**
 * @file RecentlyPlayed.h
 * @brief 定义 RecentlyPlayed 类，管理最近播放界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef RECENTLYPLAYED_H
#define RECENTLYPLAYED_H

#include "RecentlySingleSong/RecentlySingleSong.h"
#include "RecentlySongList/RecentlySongList.h"
#include "RecentlyVideoWidget/RecentlyVideoWidget.h"
#include "RecentlySongChannel/RecentlySongChannel.h"
#include "RecentlyMVChannel/RecentlyMVChannel.h"

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
    class RecentlyPlayed;
}
QT_END_NAMESPACE

/**
 * @class RecentlyPlayed
 * @brief 最近播放界面类，管理单曲、歌单、视频、歌曲频道和 MV 频道界面
 */
class RecentlyPlayed : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近播放界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlyPlayed(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlyPlayed() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化堆栈窗口、索引标签和默认单曲界面
     */
    void initUi();

    /**
     * @brief 初始化索引标签
     * @note 设置索引图片、隐藏非当前索引和数字标签样式
     */
    void initIndexLab();

    /**
     * @brief 初始化堆栈窗口
     * @note 初始化子界面和按钮组
     */
    void initStackedWidget();

    /**
     * @brief 初始化单曲界面
     * @note 创建单曲界面并添加到堆栈窗口
     */
    void initSingleSong();

    /**
     * @brief 初始化歌单界面
     * @note 创建歌单界面并添加到堆栈窗口
     */
    void initSongList();

    /**
     * @brief 初始化视频界面
     * @note 创建视频界面并添加到堆栈窗口
     */
    void initVideoWidget();

    /**
     * @brief 初始化歌曲频道界面
     * @note 创建歌曲频道界面并添加到堆栈窗口
     */
    void initSongChannel();

    /**
     * @brief 初始化 MV 频道界面
     * @note 创建 MV 频道界面并添加到堆栈窗口
     */
    void initMVChannel();

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

private slots:
    /**
     * @brief 单曲按钮点击槽函数
     * @note 切换到单曲界面
     */
    void on_single_song_pushButton_clicked();

    /**
     * @brief 歌单按钮点击槽函数
     * @note 切换到歌单界面
     */
    void on_song_list_pushButton_clicked();

    /**
     * @brief 视频按钮点击槽函数
     * @note 切换到视频界面
     */
    void on_video_pushButton_clicked();

    /**
     * @brief 歌曲频道按钮点击槽函数
     * @note 切换到歌曲频道界面
     */
    void on_song_channel_pushButton_clicked();

    /**
     * @brief MV 频道按钮点击槽函数
     * @note 切换到 MV 频道界面
     */
    void on_MV_channel_pushButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐的信号
     */
    void find_more_music();

    /**
     * @brief 搜索更多频道的信号
     */
    void find_more_channel();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换按钮和数字标签样式
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 处理数字标签点击以切换界面
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 窗口显示事件
     * @param event 显示事件
     * @note 处理窗口显示逻辑
     */
    void showEvent(QShowEvent *event) override;

private:
    Ui::RecentlyPlayed                *ui;                 ///< UI 指针
    std::unique_ptr<QButtonGroup>      m_buttonGroup;      ///< 按钮组
    std::unique_ptr<RecentlySingleSong> m_singleSong;      ///< 单曲界面
    std::unique_ptr<RecentlySongList>  m_songList;         ///< 歌单界面
    std::unique_ptr<RecentlyVideoWidget> m_videoWidget;    ///< 视频界面
    std::unique_ptr<RecentlySongChannel> m_songChannel;    ///< 歌曲频道界面
    std::unique_ptr<RecentlyMVChannel> m_mvChannel;        ///< MV 频道界面
};

#endif // RECENTLYPLAYED_H