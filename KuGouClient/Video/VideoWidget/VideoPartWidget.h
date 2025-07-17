/**
* @file VideoPartWidget.h
 * @brief 视频分区界面的头文件，定义 VideoPartWidget 类
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef VIDEOPARTWIDGET_H
#define VIDEOPARTWIDGET_H

#include "VideoBlockWidget.h"

/**
 * @class QLabel
 * @brief Qt 标签类，用于显示标题
 */
class QLabel;

/**
 * @class VideoPartWidget
 * @brief 视频分区界面类，管理标题和视频块容器
 */
class VideoPartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VideoPartWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置标题名称
     * @param title 标题文本
     * @note 更新标题标签
     */
    void setTitleName(const QString &title) const;

    /**
     * @brief 添加视频块
     * @param x 网格行坐标
     * @param y 网格列坐标
     * @param block 视频块指针
     * @note 将视频块添加到网格布局
     */
    void addBlockWidget(const int &x, const int &y, VideoBlockWidget *block) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置标题、容器和网格布局
     */
    void initUi();

private:
    QLabel *m_titleLab{};   ///< 标题标签
    QWidget *m_tabWidget{}; ///< 视频块容器
};

#endif // VIDEOPARTWIDGET_H