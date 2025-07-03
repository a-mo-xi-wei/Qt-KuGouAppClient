/**
 * @file ChannelBlock.h
 * @brief 定义 ChannelBlock 类，提供音乐频道块界面
 * @author WeiWang
 * @date 2024-12-03
 * @version 1.0
 */

#ifndef CHANNELBLOCK_H
#define CHANNELBLOCK_H

#include <QMouseEvent>
#include <QWidget>
#include "logger.hpp"

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ChannelBlock;
}
QT_END_NAMESPACE

/**
 * @class ChannelBlock
 * @brief 音乐频道块界面类，显示封面、标题、歌手歌曲信息，支持交互
 */
class ChannelBlock : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐频道块界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ChannelBlock(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ChannelBlock() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置标题文本
     * @param title 标题内容
     */
    void setTitleText(const QString &title);

    /**
     * @brief 设置歌手歌曲文本
     * @param singerSong 歌手歌曲内容
     */
    void setSingerSongText(const QString &singerSong);

private:
    /**
     * @brief 初始化界面
     */
    void initUi() const;

    /**
     * @brief 更新标题文本，处理文本截断
     */
    void updateTitleText() const;

    /**
     * @brief 更新歌手歌曲文本，处理文本截断
     */
    void updateSingerSongText() const;

protected:
    /**
     * @brief 鼠标进入事件，显示遮罩和更改标题颜色
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件，隐藏遮罩和恢复标题颜色
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 鼠标按下事件，忽略事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件，忽略事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标双击事件，忽略事件
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::ChannelBlock *ui;              ///< UI 指针
    QString           m_titleText;      ///< 标题文本
    QString           m_singerSongText; ///< 歌手歌曲文本
};

#endif // CHANNELBLOCK_H