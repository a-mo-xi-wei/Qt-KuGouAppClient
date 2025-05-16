/**
* @file AllCloudDisk.h
 * @brief 定义 AllCloudDisk 类，提供云盘音乐界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef ALLCLOUDDISK_H
#define ALLCLOUDDISK_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AllCloudDisk;
}
QT_END_NAMESPACE

/**
 * @class AllCloudDisk
 * @brief 云盘音乐界面类，支持搜索更多音乐
 */
class AllCloudDisk : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化云盘音乐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AllCloudDisk(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AllCloudDisk() override;

    private slots:
        /**
         * @brief 处理搜索按钮点击
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐信号
         */
        void find_more_music();

private:
    Ui::AllCloudDisk *ui; ///< UI 指针
};

#endif // ALLCLOUDDISK_H