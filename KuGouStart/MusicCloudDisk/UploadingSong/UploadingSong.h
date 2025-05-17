/**
* @file UploadingSong.h
 * @brief 定义 UploadingSong 类，管理正在上传歌曲界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef UPLOADINGSONG_H
#define UPLOADINGSONG_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class UploadingSong;
}
QT_END_NAMESPACE

/**
 * @class UploadingSong
 * @brief 正在上传歌曲管理界面类，支持上传、开始、暂停和清除操作
 */
class UploadingSong : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化正在上传歌曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit UploadingSong(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~UploadingSong() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置上传、开始、暂停和清除按钮图标
     */
    void initUi();

    private slots:
        /**
         * @brief 上传按钮点击槽函数
         * @note 显示未实现提示
         */
        void on_cloud_upload_toolButton_clicked();

    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索信号
     */
    void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐信号
         */
        void find_more_music();

private:
    Ui::UploadingSong                   *ui;                    ///< UI 指针
};

#endif // UPLOADINGSONG_H