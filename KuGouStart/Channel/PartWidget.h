/**
* @file PartWidget.h
 * @brief 定义 PartWidget 类，提供频道分区界面
 * @author WeiWang
 * @date 2024-12-03
 * @version 1.0
 */

#ifndef PARTWIDGET_H
#define PARTWIDGET_H

#include "ChannelBlock.h"

class QLabel;
class QWidget;

/**
 * @class PartWidget
 * @brief 频道分区界面类，显示标题和音乐块容器
 */
class PartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化频道分区界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PartWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置标题名称
     * @param title 标题内容
     */
    void setTitleName(const QString &title) const;

    /**
     * @brief 添加音乐块控件
     * @param block 音乐块控件指针
     */
    void addBlockWidget(ChannelBlock *block) const;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

private:
    QLabel  *m_titleLab{};    ///< 标题标签
    QLabel  *m_titleDeco{};   ///< 标题装饰标签
    QWidget *m_tabWidget{};   ///< 音乐块容器控件
};

#endif // PARTWIDGET_H