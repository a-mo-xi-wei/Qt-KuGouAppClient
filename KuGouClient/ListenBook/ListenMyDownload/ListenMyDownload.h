/**
 * @file ListenMyDownload.h
 * @brief 定义 ListenMyDownload 类，提供下载管理界面
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#ifndef LISTENMYDOWNLOAD_H
#define LISTENMYDOWNLOAD_H

#include "DownloadedWidget/DownloadedWidget.h"
#include "DownloadingWidget/DownloadingWidget.h"

#include <QWidget>
#include <QPointer>
#include <QTimer>

#include <array>
#include <memory>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE

namespace Ui {
class ListenMyDownload;
}

QT_END_NAMESPACE

/**
 * @class ListenMyDownload
 * @brief 下载管理界面类，切换已下载和下载中页面
 */
class ListenMyDownload : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化下载管理界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenMyDownload(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ListenMyDownload() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化堆栈窗口和下标标签
     */
    void initUi();

    /**
     * @brief 初始化下标标签
     * @note 设置下标图片、样式和事件过滤器
     */
    void initIndexLab();

    /**
     * @brief 初始化堆栈窗口
     * @note 初始化子界面和按钮组
     */
    void initStackedWidget();

    /**
     * @brief 创建页面
     * @param id 页面索引
     * @return 创建的页面控件
     */
    QWidget *createPage(int id);

    /**
     * @brief 启用或禁用按钮
     * @param flag 启用标志
     */
    void enableButton(bool flag) const;

protected:
    /**
     * @brief 事件过滤器，处理鼠标悬停事件
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件，处理标签点击
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

signals:
    /**
     * @brief 切换到推荐界面的信号
     */
    void switch_to_listen_recommend();

    void initialized();

public slots:
    void emitInitialized() { QTimer::singleShot(0, this, [this] { emit initialized(); }); }

private:
    Ui::ListenMyDownload *ui;                         ///< UI 指针
    std::unique_ptr<QButtonGroup> m_buttonGroup;      ///< 按钮组
    std::unique_ptr<DownloadedWidget> m_downloaded;   ///< 已下载控件
    std::unique_ptr<DownloadingWidget> m_downloading; ///< 下载中控件
    std::array<QPointer<QWidget>, 2> m_pages{};       ///< 页面数组
    int m_currentIdx{0};                              ///< 当前页面索引
};

#endif // LISTENMYDOWNLOAD_H