/**
 * @file UploadedSong.h
 * @brief 定义 UploadedSong 类，管理已上传云端歌曲界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef UPLOADEDSONG_H
#define UPLOADEDSONG_H

#include "SortOptionMenu.h"

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class UploadedSong;
}
QT_END_NAMESPACE

/**
 * @class UploadedSong
 * @brief 已上传云端歌曲管理界面类，支持排序、搜索、上传、下载、删除和分享
 */
class UploadedSong : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化已上传歌曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit UploadedSong(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~UploadedSong() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示、图标、搜索动作和排序连接
     */
    void initUi();

    /**
     * @brief 获取菜单位置
     * @param pos 鼠标位置
     * @note 动态调整菜单位置以适应屏幕
     */
    void getMenuPosition(const QPoint &pos);

private slots:
    /**
     * @brief 排序按钮点击槽函数
     * @note 显示排序菜单
     */
    void on_cloud_sort_toolButton_clicked();

    /**
     * @brief 上传按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_cloud_upload_toolButton_clicked();

    /**
     * @brief 下载按钮点击槽函数
     * @note 显示无音乐提示
     */
    void on_cloud_download_toolButton_clicked();

    /**
     * @brief 删除按钮点击槽函数
     * @note 显示无音乐提示
     */
    void on_cloud_delete_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_cloud_share_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_cloud_batch_toolButton_clicked();

public slots:
    /**
     * @brief 默认排序
     * @note 显示无音乐提示
     */
    void onDefaultSort();

    /**
     * @brief 添加时间排序
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onAddTimeSort(const bool &down);

    /**
     * @brief 歌曲名称排序
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onSongNameSort(const bool &down);

    /**
     * @brief 歌手排序
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onSingerSort(const bool &down);

    /**
     * @brief 时长排序
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onDurationSort(const bool &down);

    /**
     * @brief 播放次数排序
     * @param down 是否降序
     * @note 显示无音乐提示
     */
    void onPlayCountSort(const bool &down);

    /**
     * @brief 随机排序
     * @note 显示无音乐提示
     */
    void onRandomSort();

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

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理搜索图标的动态切换
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::UploadedSong                    *ui;                    ///< UI 指针
    QAction                             *m_searchAction{};      ///< 搜索动作
    SortOptionMenu                      *m_sortOptMenu{};       ///< 排序选项菜单
    QPoint                               m_menuPosition;        ///< 菜单位置
};

#endif // UPLOADEDSONG_H