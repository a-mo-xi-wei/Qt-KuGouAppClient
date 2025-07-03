/**
 * @file MyCollection.h
 * @brief 定义 MyCollection 类，管理收藏界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef MYCOLLECTION_H
#define MYCOLLECTION_H

#include "SingleSong/SingleSong.h"
#include "SongListWidget/SongListWidget.h"
#include "SpecialAlbum/SpecialAlbum.h"
#include "CollectVideo/CollectVideo.h"
#include "SingerWidget/SingerWidget.h"
#include "DeviceWidget/DeviceWidget.h"

#include <QWidget>
#include <memory>

/**
 * @class QButtonGroup
 * @brief Qt 按钮组类，用于管理互斥按钮
 */
class QButtonGroup;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class MyCollection;
}
QT_END_NAMESPACE

/**
 * @class MyCollection
 * @brief 收藏界面类，集成单曲、歌单、专辑、视频、歌手和设备界面
 */
class MyCollection : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化收藏界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MyCollection(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~MyCollection() override;

private:
    /**
     * @brief 初始化堆栈窗口
     * @note 初始化子界面并设置按钮互斥
     */
    void initStackedWidget();

    /**
     * @brief 初始化界面
     * @note 初始化堆栈窗口、索引标签和默认单曲界面
     */
    void initUi();

    /**
     * @brief 初始化索引标签
     * @note 设置索引图片和事件过滤器
     */
    void initIndexLab();

    /**
     * @brief 初始化单曲界面
     * @note 创建单曲界面并连接信号
     */
    void initSingleSong();

    /**
     * @brief 初始化歌单界面
     * @note 创建歌单界面
     */
    void initSongList();

    /**
     * @brief 初始化专辑界面
     * @note 创建专辑界面并连接信号
     */
    void initSpecialAlbum();

    /**
     * @brief 初始化视频界面
     * @note 创建视频界面并连接信号
     */
    void initCollectVideo();

    /**
     * @brief 初始化歌手界面
     * @note 创建歌手界面并连接信号
     */
    void initSinger();

    /**
     * @brief 初始化设备界面
     * @note 创建设备界面并连接信号
     */
    void initDevice();

    /**
     * @brief 启用/禁用按钮
     * @param flag 是否启用
     */
    void enableButton(const bool &flag) const;

protected:
    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 动态切换按钮和标签样式
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 点击标签切换界面
     */
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    /**
     * @brief 单曲按钮点击槽函数
     * @note 切换到单曲界面
     */
    void on_singleSong_pushButton_clicked();

    /**
     * @brief 歌单按钮点击槽函数
     * @note 切换到歌单界面
     */
    void on_songList_pushButton_clicked();

    /**
     * @brief 专辑按钮点击槽函数
     * @note 切换到专辑界面
     */
    void on_specialAlbum_pushButton_clicked();

    /**
     * @brief 视频按钮点击槽函数
     * @note 切换到视频界面
     */
    void on_collectVideo_pushButton_clicked();

    /**
     * @brief 歌手按钮点击槽函数
     * @note 切换到歌手界面
     */
    void on_singer_pushButton_clicked();

    /**
     * @brief 设备按钮点击槽函数
     * @note 切换到设备界面
     */
    void on_device_pushButton_clicked();

signals:
    /**
     * @brief 搜索更多音乐的信号
     */
    void find_more_music();

private:
    Ui::MyCollection                    *ui;                ///< UI 指针
    std::unique_ptr<QButtonGroup>        m_buttonGroup;     ///< 按钮组
    std::unique_ptr<SingleSong>          m_singleSong;      ///< 单曲界面
    std::unique_ptr<SongListWidget>      m_songList;        ///< 歌单界面
    std::unique_ptr<SpecialAlbum>        m_specialAlbum;    ///< 专辑界面
    std::unique_ptr<CollectVideo>        m_collectVideo;    ///< 视频界面
    std::unique_ptr<SingerWidget>        m_singerWidget;    ///< 歌手界面
    std::unique_ptr<DeviceWidget>        m_deviceWidget;    ///< 设备界面
};

#endif // MYCOLLECTION_H