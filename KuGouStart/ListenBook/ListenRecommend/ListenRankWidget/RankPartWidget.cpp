/**
* @file RankPartWidget.cpp
 * @brief 实现 RankPartWidget 类，提供排行榜分区控件功能
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#include "RankPartWidget.h"
#include "ui_RankPartWidget.h"

/**
 * @brief 构造函数，初始化排行榜分区控件
 * @param parent 父控件指针，默认为 nullptr
 */
RankPartWidget::RankPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RankPartWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    ui->title_label->setStyleSheet(QStringLiteral("font-style: italic;")); ///< 设置标题斜体样式
    initRankListArr();                                   ///< 初始化排行榜单项数组
}

/**
 * @brief 析构函数，清理资源
 */
RankPartWidget::~RankPartWidget()
{
    delete ui;
}

/**
 * @brief 设置标题
 * @param title 标题文本
 */
void RankPartWidget::setTitle(const QString &title) const
{
    ui->title_label->setText(title);                     ///< 设置标题文本
}

/**
 * @brief 获取排行榜单项控件
 * @param index 索引
 * @return 排行榜单项控件指针
 */
RankListWidget *RankPartWidget::getRankListWidget(const int &index) const
{
    return this->m_rankListArr[index];                   ///< 返回指定索引的榜单项
}

/**
 * @brief 初始化排行榜单项数组
 */
void RankPartWidget::initRankListArr()
{
    this->m_rankListArr[0] = ui->widget_1;              ///< 设置榜单项 1
    this->m_rankListArr[1] = ui->widget_2;              ///< 设置榜单项 2
    this->m_rankListArr[2] = ui->widget_3;              ///< 设置榜单项 3
    this->m_rankListArr[3] = ui->widget_4;              ///< 设置榜单项 4
    this->m_rankListArr[4] = ui->widget_5;              ///< 设置榜单项 5
}