/**
* @file ListenRankWidget.h
 * @brief 定义 ListenRankWidget 类，提供排行榜界面
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#ifndef LISTENRANKWIDGET_H
#define LISTENRANKWIDGET_H

#include "RankPartWidget.h"

#include <QWidget>

/**
 * @class ListenRankWidget
 * @brief 排行榜界面类，显示免费榜、飙升榜、热销榜和新品榜
 */
class ListenRankWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化排行榜界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenRankWidget(QWidget *parent = nullptr);

private:
    /**
     * @brief 初始化界面
     * @note 设置水平布局并添加榜单控件
     */
    void initUi();

    /**
     * @brief 初始化免费榜
     * @note 异步加载 JSON 数据并设置榜单内容
     */
    void initRankFree();

    /**
     * @brief 初始化飙升榜
     * @note 异步加载 JSON 数据并设置榜单内容
     */
    void initRankSkyrocket();

    /**
     * @brief 初始化热销榜
     * @note 异步加载 JSON 数据并设置榜单内容
     */
    void initRankSell();

    /**
     * @brief 初始化新品榜
     * @note 异步加载 JSON 数据并设置榜单内容
     */
    void initRankNew();

protected:
    /**
     * @brief 调整大小事件，动态显示或隐藏新品榜
     * @param event 调整大小事件
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    RankPartWidget *rank_free{};      ///< 免费榜控件
    RankPartWidget *rank_skyrocket{}; ///< 飙升榜控件
    RankPartWidget *rank_sell{};      ///< 热销榜控件
    RankPartWidget *rank_new{};       ///< 新品榜控件
};

#endif // LISTENRANKWIDGET_H