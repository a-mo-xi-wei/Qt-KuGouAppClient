/**
 * @file RecentlyVideoWidget.h
 * @brief 定义 RecentlyVideoWidget 类，管理最近视频界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef RECENTLYVIDEOWIDGET_H
#define RECENTLYVIDEOWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class RecentlyVideoWidget;
}
QT_END_NAMESPACE

/**
 * @class RecentlyVideoWidget
 * @brief 最近视频界面类，提供搜索功能
 */
class RecentlyVideoWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近视频界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RecentlyVideoWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~RecentlyVideoWidget() override;

private slots:
    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索音乐信号，可能应为 find_more_video
     */
    void on_search_pushButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐的信号
     * @note 信号名称可能为误用，应为 find_more_video
     */
    void find_more_music();

private:
    Ui::RecentlyVideoWidget           *ui;                 ///< UI 指针
};

#endif // RECENTLYVIDEOWIDGET_H