/**
* @file VideoChannelPartWidget.h
 * @brief 定义 VideoChannelPartWidget 类，提供视频频道分类部件
 * @author WeiWang
 * @date 2024-12-12
 * @version 1.0
 */

#ifndef VIDEOCHANNELPARTWIDGET_H
#define VIDEOCHANNELPARTWIDGET_H

#include "VideoChannelBlock.h"

#include <QWidget>

class QLabel;

/**
 * @class VideoChannelPartWidget
 * @brief 视频频道分类部件类，包含标题和视频块容器
 */
class VideoChannelPartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化视频频道分类部件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoChannelPartWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置标题名称
     * @param title 标题文本
     */
    void setTitleName(const QString &title) const;

    /**
     * @brief 添加视频块控件
     * @param block 视频块控件指针
     */
    void addBlockWidget(VideoChannelBlock *block) const;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法，当前为空实现
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel  *m_titleLab{};    ///< 标题标签
    QWidget *m_tabWidget{};   ///< 视频块容器
};

#endif // VIDEOCHANNELPARTWIDGET_H