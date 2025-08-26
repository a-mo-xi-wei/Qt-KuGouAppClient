/**
* @file Downloading.h
 * @brief 定义 Downloading 类，提供下载管理界面
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#ifndef DOWNLOADING_H
#define DOWNLOADING_H

#include <QTimer>
#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE

namespace Ui {
class Downloading;
}

QT_END_NAMESPACE

/**
 * @class Downloading
 * @brief 下载管理界面类，支持开始、停止、清除和设置
 */
class Downloading : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化下载管理界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit Downloading(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~Downloading() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示和图标
     */
    void initUi();

private slots:
    /**
     * @brief 开始按钮点击槽函数
     * @note 显示暂无下载提示
     */
    void on_start_toolButton_clicked();

    /**
     * @brief 停止按钮点击槽函数
     * @note 显示暂无下载提示
     */
    void on_stop_toolButton_clicked();

    /**
     * @brief 清除按钮点击槽函数
     * @note 显示暂无下载提示
     */
    void on_clear_toolButton_clicked();

    /**
     * @brief 设置按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_setting_toolButton_clicked();

private slots:
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
    Ui::Downloading *ui; ///< UI 指针
};

#endif // DOWNLOADING_H