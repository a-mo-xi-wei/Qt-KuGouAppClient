/**
 * @file MusicRepoBlock.h
 * @brief 定义 MusicRepoBlock 类，显示音乐仓库块状项
 * @author WeiWang
 * @date 2024-11-30
 * @version 1.0
 */

#ifndef MUSICREPOBLOCK_H
#define MUSICREPOBLOCK_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicRepoBlock;
}
QT_END_NAMESPACE

/**
 * @class MusicRepoBlock
 * @brief 音乐仓库块状项类，显示歌曲封面、名称和歌手信息
 */
class MusicRepoBlock : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐仓库块状项
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicRepoBlock(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MusicRepoBlock() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath);

    /**
     * @brief 设置歌曲名称
     * @param song 歌曲名称
     */
    void setSongName(const QString &song);

    /**
     * @brief 设置歌手名称
     * @param singer 歌手名称
     */
    void setSinger(const QString &singer);

private:
    /**
     * @brief 更新歌曲名称文本
     * @note 根据控件宽度截断文本
     */
    void updateSongText();

    /**
     * @brief 更新歌手名称文本
     * @note 根据控件宽度截断文本
     */
    void updateSingerText();

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
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整封面高度和更新文本
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoBlock                  *ui;                    ///< UI 指针
    QString                              m_songName;            ///< 歌曲名称
    QString                              m_singer;              ///< 歌手名称
};

#endif // MUSICREPOBLOCK_H