/**
* @file CollectVideo.h
 * @brief 定义 CollectVideo 类，管理收藏视频界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef COLLECTVIDEO_H
#define COLLECTVIDEO_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class CollectVideo;
}
QT_END_NAMESPACE

/**
 * @class CollectVideo
 * @brief 收藏视频界面类，管理收藏视频的展示和操作
 */
class CollectVideo : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化收藏视频界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit CollectVideo(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~CollectVideo() override;

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发搜索更多音乐的信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐的信号
         */
        void find_more_music();

private:
    Ui::CollectVideo *ui; ///< UI 指针
};

#endif // COLLECTVIDEO_H