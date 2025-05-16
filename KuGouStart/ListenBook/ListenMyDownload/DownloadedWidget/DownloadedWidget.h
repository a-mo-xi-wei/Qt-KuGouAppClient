/**
* @file DownloadedWidget.h
 * @brief 定义 DownloadedWidget 类，提供已下载界面
 * @author WeiWang
 * @date 2025-02-04
 * @version 1.0
 */

#ifndef DOWNLOADEDWIDGET_H
#define DOWNLOADEDWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class DownloadedWidget;
}
QT_END_NAMESPACE

/**
 * @class DownloadedWidget
 * @brief 已下载界面类，显示已下载内容并支持搜索
 */
class DownloadedWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化已下载界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit DownloadedWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~DownloadedWidget() override;

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
    Ui::DownloadedWidget *ui; ///< UI 指针
};

#endif // DOWNLOADEDWIDGET_H