/**
* @file DownloadedVideo.h
 * @brief 定义 DownloadedVideo 类，提供已下载视频界面
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#ifndef DOWNLOADEDVIDEO_H
#define DOWNLOADEDVIDEO_H

#include <QTimer>
#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE

namespace Ui {
class DownloadedVideo;
}

QT_END_NAMESPACE

/**
 * @class DownloadedVideo
 * @brief 已下载视频界面类，支持搜索
 */
class DownloadedVideo : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化已下载视频界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DownloadedVideo(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DownloadedVideo() override;

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

private:
    Ui::DownloadedVideo *ui; ///< UI 指针
};

#endif // DOWNLOADEDVIDEO_H