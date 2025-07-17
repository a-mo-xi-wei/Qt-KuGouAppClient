/**
 * @file ListenRecommend.h
 * @brief 定义 ListenRecommend 类，提供推荐界面
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#ifndef LISTENRECOMMEND_H
#define LISTENRECOMMEND_H

#include "ListenOptionMenu/ListenOptionMenu.h"
#include "ListenTableWidget/ListenTableWidget.h"

#include <QWidget>
#include <vector>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ListenRecommend;
}
QT_END_NAMESPACE

class QTimer;
class RefreshMask;

/**
 * @class ListenRecommend
 * @brief 推荐界面类，包含分类菜单和多个表格控件
 */
class ListenRecommend : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化推荐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenRecommend(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ListenRecommend() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置全部分类按钮、定时器和画廊
     */
    void initUi();

    /**
     * @brief 初始化表格控件
     * @note 创建并布局多个分类表格
     */
    void initTableWidgets();

    /**
     * @brief 初始化每日推荐画廊
     * @note 异步加载 JSON 数据并填充画廊
     */
    void initDailyRecommendGalleryWidget();

    /**
     * @brief 初始化其他分类画廊
     * @param jsonFileName JSON 文件名
     * @param gallery 表格控件指针
     * @note 异步加载 JSON 数据并填充画廊
     */
    void initOtherGalleryWidget(const QString &jsonFileName, const ListenTableWidget *gallery);

private slots:
    /**
     * @brief 全部分类按钮点击槽函数
     * @note 显示分类菜单并切换图标
     */
    void on_all_classify_toolButton_clicked();

    /**
     * @brief 刷新按钮点击槽函数
     * @note 启动定时器延迟刷新
     */
    void onToolButtonClicked();

    /**
     * @brief 刷新定时器超时槽函数
     * @note 更新画廊内容并显示成功提示
     */
    void onRefreshTimeout();

    /**
     * @brief 菜单功能点击槽函数
     * @param funcName 功能名称
     * @note 显示功能未实现的提示
     */
    void onMenuFuncClicked(const QString &funcName);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    Ui::ListenRecommend                    *ui;                 ///< UI 指针
    ListenOptionMenu                       *m_menu{};           ///< 分类菜单
    QList<std::pair<QString, QString>>      m_galleryVector[17];///< 画廊数据数组
    QTimer                                 *m_refreshTimer{};   ///< 刷新定时器
    std::unique_ptr<RefreshMask>            m_refreshMask;      ///< 刷新遮罩
};

#endif // LISTENRECOMMEND_H