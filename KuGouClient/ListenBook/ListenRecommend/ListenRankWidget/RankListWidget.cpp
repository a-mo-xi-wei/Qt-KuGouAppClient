/**
 * @file RankListWidget.cpp
 * @brief 实现 RankListWidget 类，提供排行榜单项控件功能
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#include "RankListWidget.h"
#include "ui_RankListWidget.h"
#include "ElaToolTip.h"

/**
 * @brief 构造函数，初始化排行榜单项控件
 * @param parent 父控件指针，默认为 nullptr
 */
RankListWidget::RankListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RankListWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    ui ->desc_label ->setFont(QFont("TaiwanPearl", 10 ));
    ui ->info_label ->setFont(QFont("TaiwanPearl", 9 ));
    ui->desc_label->setStyleSheet(QStringLiteral("color: black; ")); ///< 设置描述标签默认颜色
    ui->fire_label->setStyleSheet(QStringLiteral("border-image: url(\":/ListenBook/Res/listenbook/fire.svg\");")); ///< 设置火焰图标
    ui->info_label->setStyleSheet(QStringLiteral("color: gray;")); ///< 设置信息标签颜色
}

/**
 * @brief 析构函数，清理资源
 */
RankListWidget::~RankListWidget()
{
    delete ui;
}

/**
 * @brief 设置描述文本
 * @param text 描述文本
 * @note 使用字体测量工具进行文本省略
 */
void RankListWidget::setDescText(const QString &text) const
{
    const auto font = ui->desc_label->font();            ///< 获取字体
    const QFontMetrics fm(font);                         ///< 创建字体测量工具
    const QString elidedText = fm.elidedText(text, Qt::ElideRight, this->width() - 110); ///< 省略文本
    ui->desc_label->setText(elidedText);                 ///< 设置描述标签文本
    auto desc_label_toolTip = new ElaToolTip(ui->desc_label); ///< 创建工具提示
    desc_label_toolTip->setToolTip(text);                ///< 设置工具提示内容
}

/**
 * @brief 设置信息文本
 * @param text 信息文本
 * @note 使用字体测量工具进行文本省略
 */
void RankListWidget::setInfoText(const QString &text) const
{
    const auto font = ui->info_label->font();            ///< 获取字体
    const QFontMetrics fm(font);                         ///< 创建字体测量工具
    const QString elidedText = fm.elidedText(text, Qt::ElideRight, ui->info_label->width()); ///< 省略文本
    ui->info_label->setText(elidedText);                 ///< 设置信息标签文本
}

/**
 * @brief 设置封面图片
 * @param path 图片路径
 */
void RankListWidget::setCoverImg(const QString &path) const
{
    ui->rank_cover_label->setStyleSheet(QString("border-radius: 5px;border-image: url(%1);").arg(path)); ///< 设置封面图片和圆角
}

/**
 * @brief 设置排名奖牌
 * @param path 奖牌图片路径
 */
void RankListWidget::setRankMedal(const QString &path) const
{
    ui->rank_number_label->setStyleSheet(QString("border-image: url(%1);").arg(path)); ///< 设置奖牌图标
}

/**
 * @brief 设置排名数字
 * @param number 排名数字
 */
void RankListWidget::setRankNumber(const QString &number) const
{
    ui->rank_number_label->setText(number);              ///< 设置排名文本
    ui->rank_number_label->setStyleSheet(QString("font-size: 15px;color: gray;")); ///< 设置排名样式
}

/**
 * @brief 鼠标进入事件，改变描述颜色
 * @param event 进入事件
 */
void RankListWidget::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);                          ///< 调用父类进入事件
    ui->desc_label->setStyleSheet(QStringLiteral("color: #26A1FF;")); ///< 设置悬停颜色
}

/**
 * @brief 鼠标离开事件，恢复描述颜色
 * @param event 事件
 */
void RankListWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);                          ///< 调用父类离开事件
    ui->desc_label->setStyleSheet(QStringLiteral("color: black;")); ///< 恢复默认颜色
}