/**
 * @file LocalDownload.h
 * @brief 定义 LocalDownload 类，管理本地歌曲、已下载歌曲、已下载视频和正在下载界面
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.0
 */

#ifndef LOCALDOWNLOAD_H
#define LOCALDOWNLOAD_H

#include "LocalSong/LocalSong.h"
#include "DownloadedSong/DownloadedSong.h"
#include "DownloadedVideo/DownloadedVideo.h"
#include "Downloading/Downloading.h"
#include "SlidingStackedWidget.h"

#include <QWidget>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LocalDownload;
}
QT_END_NAMESPACE

/**
 * @class LocalDownload
 * @brief 本地下载管理界面类，整合本地歌曲、已下载歌曲、已下载视频和正在下载功能
 */
class LocalDownload : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化本地下载界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit LocalDownload(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~LocalDownload() override;

    /**
     * @brief 音频播放结束处理
     * @note 转发给 LocalSong 处理
     */
    void audioFinished();

    /**
     * @brief 播放下一首本地歌曲
     */
    void playLocalSongNextSong();

    /**
     * @brief 播放上一首本地歌曲
     */
    void playLocalSongPrevSong();

public slots:
    /**
     * @brief 最大化屏幕处理
     * @note 更新当前播放高亮
     */
    void onMaxScreenHandle();

private:
    /**
     * @brief 初始化堆栈窗口
     * @note 初始化四个子界面并设置按钮互斥
     */
    void initStackedWidget();

    /**
     * @brief 初始化界面
     * @note 隐藏下载历史按钮、设置下标图片、初始化堆栈窗口和默认点击本地音乐
     */
    void initUi();

    /**
     * @brief 初始化本地歌曲界面
     */
    void initLocalSong();

    /**
     * @brief 初始化已下载歌曲界面
     */
    void initDownloadedSong();

    /**
     * @brief 初始化已下载视频界面
     */
    void initDownloadedVideo();

    /**
     * @brief 初始化正在下载界面
     */
    void initDownloading();

    /**
     * @brief 启用/禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

private slots:
    /**
     * @brief 本地音乐按钮点击槽函数
     * @note 切换到本地歌曲界面，更新下标和标签样式
     */
    void on_local_music_pushButton_clicked();

    /**
     * @brief 已下载音乐按钮点击槽函数
     * @note 切换到已下载歌曲界面，更新下标和标签样式
     */
    void on_downloaded_music_pushButton_clicked();

    /**
     * @brief 已下载视频按钮点击槽函数
     * @note 切换到已下载视频界面，更新下标和标签样式
     */
    void on_downloaded_video_pushButton_clicked();

    /**
     * @brief 正在下载按钮点击槽函数
     * @note 切换到正在下载界面，更新下标和标签样式
     */
    void on_downloading_pushButton_clicked();

    /**
     * @brief 下载历史按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_download_history_toolButton_clicked();

    /**
     * @brief 本地音乐数量标签变化槽函数
     * @param num 歌曲数量
     * @note 更新本地音乐数量标签
     */
    void local_music_label_changed(const int &num);

signals:
    /**
     * @brief 播放音乐信号
     * @param localPath 本地歌曲路径
     */
    void playMusic(const QString &localPath);

    /**
     * @brief 取消循环播放信号
     */
    void cancelLoopPlay();

    /**
     * @brief 搜索更多音乐信号
     */
    void find_more_music();

private:
    Ui::LocalDownload                   *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>        m_buttonGroup;         ///< 按钮组
    std::unique_ptr<LocalSong>           m_localSong;           ///< 本地歌曲界面
    std::unique_ptr<DownloadedSong>      m_downloadedSong;      ///< 已下载歌曲界面
    std::unique_ptr<DownloadedVideo>     m_downloadedVideo;     ///< 已下载视频界面
    std::unique_ptr<Downloading>         m_downloading;         ///< 正在下载界面
};

#endif // LOCALDOWNLOAD_H