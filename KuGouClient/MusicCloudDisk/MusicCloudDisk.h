/**
 * @file MusicCloudDisk.h
 * @brief 定义 MusicCloudDisk 类，管理云端歌曲（已上传和正在上传）界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef MUSICCLOUDDISK_H
#define MUSICCLOUDDISK_H

#include "UploadedSong/UploadedSong.h"
#include "UploadingSong/UploadingSong.h"
#include "SlidingStackedWidget.h"

#include <QWidget>
#include <memory>
#include <array>
#include <QPointer>

class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicCloudDisk;
}
QT_END_NAMESPACE

/**
 * @class MusicCloudDisk
 * @brief 云端歌曲管理界面类，整合已上传和正在上传歌曲功能
 */
class MusicCloudDisk : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化云端歌曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicCloudDisk(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MusicCloudDisk() override;

private:
    /**
     * @brief 初始化界面
     * @note 初始化下标标签、新增歌曲按钮和堆栈窗口
     */
    void initUi();

    /**
     * @brief 初始化下标标签
     * @note 设置下标图片、默认样式和事件过滤器
     */
    void initIndexLab();

    /**
     * @brief 初始化堆栈窗口
     * @note 初始化已上传和正在上传歌曲界面并设置按钮互斥
     */
    void initStackedWidget();

    QWidget* createPage(int id);

    /**
     * @brief 启用/禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

private slots:
    /**
     * @brief 新增歌曲按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_new_add_toolButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐信号
     */
    void find_more_music();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理按钮和标签样式动态切换
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 处理标签点击触发按钮点击
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MusicCloudDisk                  *ui;                    ///< UI 指针
    std::unique_ptr<QButtonGroup>        m_buttonGroup;         ///< 按钮组
    std::unique_ptr<UploadedSong>        m_uploadedSong;        ///< 已上传歌曲界面
    std::unique_ptr<UploadingSong>       m_uploadingSong;       ///< 正在上传歌曲界面
    std::array<QPointer<QWidget>, 2> m_pages{};                 ///< 页面占位符数组
    int m_currentIdx{0};                                        ///< 当前页面索引
};

#endif // MUSICCLOUDDISK_H