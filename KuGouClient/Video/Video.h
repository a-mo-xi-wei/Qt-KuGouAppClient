/**
* @file Video.h
 * @brief 定义 Video 类，提供视频界面管理
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#ifndef VIDEO_H
#define VIDEO_H

#include "VideoChannelWidget/VideoChannelWidget.h"
#include "VideoWidget/VideoWidget.h"
#include "MVWidget/MVWidget.h"

#include <QWidget>
#include <QPointer>
#include <memory>
#include <array>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Video;
}
QT_END_NAMESPACE

/**
 * @class Video
 * @brief 视频界面类，支持视频频道、MV 和视频子界面切换
 */
class Video : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化视频界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit Video(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~Video() override;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化堆栈窗口
     */
    void initStackedWidget();

    /**
     * @brief 创建页面
     * @param id 页面索引
     * @return 创建的页面控件
     */
    QWidget* createPage(const int& id);

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     */
    void enableButton(bool flag) const;

private:
    Ui::Video *ui;                                     ///< UI 指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;       ///< 按钮组
    std::unique_ptr<VideoChannelWidget> m_videoChannelWidget; ///< 视频频道控件
    std::unique_ptr<VideoWidget> m_videoWidget;       ///< 视频控件
    std::unique_ptr<MVWidget> m_MVWidget;             ///< MV 控件
    std::array<QPointer<QWidget>, 3> m_pages{};       ///< 页面数组
    int m_currentIdx{0};                              ///< 当前页面索引
};

#endif // VIDEO_H