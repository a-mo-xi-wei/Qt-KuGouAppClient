/**
 * @file PurchasedMusic.h
 * @brief 定义 PurchasedMusic 类，管理付费音乐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef PURCHASEDMUSIC_H
#define PURCHASEDMUSIC_H

#include "PaidSingle/PaidSingle.h"
#include "PurchasedAlbums/PurchasedAlbums.h"
#include "PurchasedVideos/PurchasedVideos.h"

#include <QWidget>
#include <memory>

/**
 * @class QButtonGroup
 * @brief Qt 按钮组类，用于管理互斥按钮
 */
class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class PurchasedMusic;
}
QT_END_NAMESPACE

/**
 * @class PurchasedMusic
 * @brief 付费音乐界面类，集成付费单曲、已购专辑和已购视频界面
 */
class PurchasedMusic : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化付费音乐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PurchasedMusic(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~PurchasedMusic() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化索引标签、堆栈窗口和默认付费单曲界面
     */
    void initUi();

    /**
     * @brief 初始化索引标签
     * @note 设置索引图片和事件过滤器
     */
    void initIndexLab();

    /**
     * @brief 初始化堆栈窗口
     * @note 初始化子界面并设置按钮互斥
     */
    void initStackedWidget();

    /**
     * @brief 初始化付费单曲界面
     * @note 创建付费单曲界面并连接信号
     */
    void initPaidSingle();

    /**
     * @brief 初始化已购专辑界面
     * @note 创建已购专辑界面并连接信号
     */
    void initPurchasedAlbums();

    /**
     * @brief 初始化已购视频界面
     * @note 创建已购视频界面并连接信号
     */
    void initPurchasedVideos();

    /**
     * @brief 启用/禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

private slots:
    /**
     * @brief 付费单曲按钮点击槽函数
     * @note 切换到付费单曲界面
     */
    void on_paid_single_pushButton_clicked();

    /**
     * @brief 已购专辑按钮点击槽函数
     * @note 切换到已购专辑界面
     */
    void on_purchased_albums_pushButton_clicked();

    /**
     * @brief 已购视频按钮点击槽函数
     * @note 切换到已购视频界面
     */
    void on_purchased_video_pushButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐的信号
     */
    void find_more_music();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换按钮和标签样式
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 点击标签切换界面
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::PurchasedMusic                 *ui;                 ///< UI 指针
    std::unique_ptr<QButtonGroup>       m_buttonGroup;      ///< 按钮组
    std::unique_ptr<PaidSingle>         m_paidSingle;       ///< 付费单曲界面
    std::unique_ptr<PurchasedAlbums>    m_purchasedAlbums;  ///< 已购专辑界面
    std::unique_ptr<PurchasedVideos>    m_purchasedVideos;  ///< 已购视频界面
};

#endif // PURCHASEDMUSIC_H