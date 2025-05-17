/**
* @file RankListWidget.h
 * @brief 定义 RankListWidget 类，提供排行榜单项控件
 * @author WeiWang
 * @date 2025-02-08
 * @version 1.0
 */

#ifndef RANKLISTWIDGET_H
#define RANKLISTWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RankListWidget;
}
QT_END_NAMESPACE

/**
 * @class RankListWidget
 * @brief 排行榜单项控件类，显示描述、封面和排名信息
 */
class RankListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化排行榜单项控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RankListWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RankListWidget() override;

    /**
     * @brief 设置描述文本
     * @param text 描述文本
     */
    void setDescText(const QString &text) const;

    /**
     * @brief 设置信息文本
     * @param text 信息文本
     */
    void setInfoText(const QString &text) const;

    /**
     * @brief 设置封面图片
     * @param path 图片路径
     */
    void setCoverImg(const QString &path) const;

    /**
     * @brief 设置排名奖牌
     * @param path 奖牌图片路径
     */
    void setRankMedal(const QString &path) const;

    /**
     * @brief 设置排名数字
     * @param number 排名数字
     */
    void setRankNumber(const QString &number) const;

protected:
    /**
     * @brief 鼠标进入事件，改变描述颜色
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件，恢复描述颜色
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;

private:
    Ui::RankListWidget *ui; ///< UI 指针
};

#endif // RANKLISTWIDGET_H