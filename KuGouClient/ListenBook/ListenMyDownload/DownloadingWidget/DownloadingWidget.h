/**
* @file DownloadingWidget.h
 * @brief 定义 DownloadingWidget 类，提供下载中界面
 * @author WeiWang
 * @date 2025-02-04
 * @version 1.0
 */

#ifndef DOWNLOADINGWIDGET_H
#define DOWNLOADINGWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class DownloadingWidget;
}
QT_END_NAMESPACE

/**
 * @class DownloadingWidget
 * @brief 下载中界面类，显示下载进度和操作按钮
 */
class DownloadingWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化下载中界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DownloadingWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DownloadingWidget() override;

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发查找更多有声书的信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 查找更多有声书的信号
         */
        void find_more_audio_book();

private:
    Ui::DownloadingWidget *ui; ///< UI 指针
};

#endif // DOWNLOADINGWIDGET_H