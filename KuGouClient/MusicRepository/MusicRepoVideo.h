/**
 * @file MusicRepoVideo.h
 * @brief 定义 MusicRepoVideo 类，显示音乐仓库视频项
 * @author WeiWang
 * @date 2024-11-30
 * @version 1.0
 */

#ifndef MUSICREPOVIDEO_H
#define MUSICREPOVIDEO_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicRepoVideo;
}
QT_END_NAMESPACE

/**
 * @class MusicRepoVideo
 * @brief 音乐仓库视频项类，显示视频封面、名称、图标和作者信息
 */
class MusicRepoVideo : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐仓库视频项
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicRepoVideo(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MusicRepoVideo() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置视频名称
     * @param name 视频名称
     */
    void setVideoName(const QString &name);

    /**
     * @brief 设置图标
     * @param pix 图标路径
     */
    void setIconPix(const QString &pix) const;

    /**
     * @brief 设置作者名称
     * @param author 作者名称
     */
    void setAuthor(const QString &author);

private:
    /**
     * @brief 初始化界面
     * @note 设置封面控件（图标、方向、宽高比、播放量、左填充）和事件过滤器
     */
    void initUi();

    /**
     * @brief 更新视频名称文本
     * @note 根据控件宽度截断文本
     */
    void updateVideoNameText() const;

    /**
     * @brief 更新作者名称文本
     * @note 根据控件宽度截断文本
     */
    void updateVideoAuthorText() const;

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件
     * @note 忽略事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换播放量方向（悬停/离开）
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整封面高度和更新文本
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoVideo                  *ui;                    ///< UI 指针
    QString                              m_videoName;           ///< 视频名称
    QString                              m_videoAuthor;         ///< 视频作者
    bool                                 m_isEnter = false;     ///< 是否进入状态
};

#endif // MUSICREPOVIDEO_H