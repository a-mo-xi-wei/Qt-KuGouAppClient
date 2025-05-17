/**
* @file PurchasedAlbums.h
 * @brief 定义 PurchasedAlbums 类，管理已购专辑界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef PURCHASEDALBUMS_H
#define PURCHASEDALBUMS_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class PurchasedAlbums;
}
QT_END_NAMESPACE

/**
 * @class PurchasedAlbums
 * @brief 已购专辑界面类，提供搜索功能
 */
class PurchasedAlbums : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化已购专辑界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit PurchasedAlbums(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~PurchasedAlbums() override;

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发搜索信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐的信号
         */
        void find_more_music();

private:
    Ui::PurchasedAlbums                *ui;                 ///< UI 指针
};

#endif // PURCHASEDALBUMS_H