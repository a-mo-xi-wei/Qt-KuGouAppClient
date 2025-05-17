/**
* @file SongBlock.h
 * @brief 歌曲块界面的头文件，定义 SongBlock 类
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SONGBLOCK_H
#define SONGBLOCK_H

#include "MyBlockWidget.h"

class QLabel;

/**
 * @class SongBlock
 * @brief 歌曲块界面类，显示封面和描述
 */
class SongBlock : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SongBlock(QWidget *parent = nullptr);

    /**
     * @brief 设置封面图片
     * @param pixmapPath 图片路径
     * @note 更新封面显示
     */
    void setCoverPix(const QString &pixmapPath) const;

    /**
     * @brief 设置描述文本
     * @param text 描述文本
     * @note 更新描述标签并调整换行
     */
    void setDescText(const QString &text) const;

    /**
     * @brief 显示提示标签
     * @note 启用封面的提示标签
     */
    void setShowTip() const;

    /**
     * @brief 设置提示文本
     * @param text 提示文本
     * @note 更新封面的提示标签内容
     */
    void setTipText(const QString &text) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置布局、样式和遮罩
     */
    void initUi();

    /**
     * @brief 更新描述标签
     * @note 处理文本换行和省略
     */
    void updateDescLab() const;

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 更新描述标签宽度
     */
    void resizeEvent(QResizeEvent *event) override;

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
     * @brief 显示事件
     * @param event 显示事件
     * @note 更新描述标签
     */
    void showEvent(QShowEvent *event) override;

private:
    MyBlockWidget *m_cover{}; ///< 封面控件
    QLabel *m_descLab{};      ///< 描述标签
};

#endif // SONGBLOCK_H