/**
* @file DailyRecommend.h
 * @brief 定义 DailyRecommend 类，提供每日推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef DAILYRECOMMEND_H
#define DAILYRECOMMEND_H

#include "MusicItemWidget.h"
#include <QWidget>

class QLabel;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class DailyRecommend;
}
QT_END_NAMESPACE

/**
 * @class DailyRecommend
 * @brief 每日推荐界面类，显示日期、歌曲列表和操作按钮
 */
class DailyRecommend : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化每日推荐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DailyRecommend(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DailyRecommend() override;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    /**
     * @brief 初始化日期标签
     */
    void initDateLab();

    /**
     * @brief 初始化歌曲列表控件
     */
    void initTableWidget();

    /**
     * @brief 初始化音乐项控件
     * @param item 音乐项控件指针
     */
    void initMusicItem(MusicItemWidget *item);

private slots:
    /**
     * @brief 历史推荐按钮点击槽函数
     */
    void on_history_recommend_toolButton_clicked();

    /**
     * @brief 播放按钮点击槽函数
     */
    void on_play_toolButton_clicked();

    /**
     * @brief VIP 按钮点击槽函数
     */
    void on_vip_toolButton_clicked();

    /**
     * @brief 收藏按钮点击槽函数
     */
    void on_collect_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     */
    void on_download_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     */
    void on_batch_toolButton_clicked();

private:
    Ui::DailyRecommend *ui;             ///< UI 指针
    QLabel             *m_monthLab{};   ///< 月份标签
    QLabel             *m_dayLab{};     ///< 日期标签
};

#endif // DAILYRECOMMEND_H