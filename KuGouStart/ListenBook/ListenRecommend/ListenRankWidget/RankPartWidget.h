/**
* @file RankPartWidget.h
 * @brief 定义 RankPartWidget 类，提供排行榜分区控件
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#ifndef RANKPARTWIDGET_H
#define RANKPARTWIDGET_H

#include "RankListWidget.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RankPartWidget;
}
QT_END_NAMESPACE

/**
 * @class RankPartWidget
 * @brief 排行榜分区控件类，管理排行榜单项
 */
class RankPartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化排行榜分区控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RankPartWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RankPartWidget() override;

    /**
     * @brief 设置标题
     * @param title 标题文本
     */
    void setTitle(const QString &title) const;

    /**
     * @brief 获取排行榜单项控件
     * @param index 索引
     * @return 排行榜单项控件指针
     */
    RankListWidget *getRankListWidget(const int &index) const;

private:
    /**
     * @brief 初始化排行榜单项数组
     */
    void initRankListArr();

private:
    Ui::RankPartWidget   *ui;              ///< UI 指针
    RankListWidget       *m_rankListArr[5]; ///< 排行榜单项数组
};

#endif // RANKPARTWIDGET_H