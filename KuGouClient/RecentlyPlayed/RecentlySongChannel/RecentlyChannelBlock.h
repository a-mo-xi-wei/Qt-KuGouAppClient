/**
* @file RecentlyChannelBlock.h
 * @brief 定义 RecentlyChannelBlock 类，管理最近频道块
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#ifndef RECENTLYCHANNELBLOCK_H
#define RECENTLYCHANNELBLOCK_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RecentlyChannelBlock;
}
QT_END_NAMESPACE

/**
 * @class RecentlyChannelBlock
 * @brief 最近频道块类，显示封面、标题、歌手/歌曲信息和交互效果
 */
class RecentlyChannelBlock : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近频道块
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlyChannelBlock(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlyChannelBlock() override;

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     */
    void setCoverPix(const QString &pixmapPath) const;

private:
    /**
     * @brief 初始化界面
     * @note 初始化标题字体、工具提示和遮罩效果
     */
    void initUi() const;

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
     * @brief 进入事件
     * @param event 进入事件
     * @note 显示遮罩效果
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 离开事件
     * @param event 离开事件
     * @note 隐藏遮罩效果
     */
    void leaveEvent(QEvent *event) override;

private:
    Ui::RecentlyChannelBlock          *ui;                 ///< UI 指针
};

#endif // RECENTLYCHANNELBLOCK_H