/**
* @file SongListWidget.h
 * @brief 定义 SongListWidget 类，管理歌单界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef SONGLISTWIDGET_H
#define SONGLISTWIDGET_H

#include "SongListBlockWidget.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class SongListWidget;
}
QT_END_NAMESPACE

/**
 * @class SongListWidget
 * @brief 歌单界面类，管理歌单的展示和操作
 */
class SongListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌单界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SongListWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~SongListWidget() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置批量操作按钮、搜索栏和事件过滤器
     */
    void initUi();

    /**
     * @brief 初始化歌单块
     * @note 添加歌单块到布局
     */
    void initBlock() const;

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换搜索图标颜色
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    private slots:
        /**
         * @brief 批量操作按钮点击槽函数
         * @note 显示未实现提示
         */
        void on_batch_toolButton_clicked();

    /**
     * @brief 导入按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_import_toolButton_clicked();

private:
    Ui::SongListWidget *ui;              ///< UI 指针
    QAction            *m_searchAction;   ///< 搜索动作
};

#endif // SONGLISTWIDGET_H