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
#include <QPointer>
#include <array>
#include <memory>

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
     * @brief 创建页面
     * @param id 页面索引
     * @return 创建的页面控件
     */
    QWidget* createPage(int id);

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
    std::array<QPointer<QWidget>, 6>     m_pages{};         ///< 页面数组
    int                                  m_currentIdx{0};   ///< 当前页面索引
};

#endif // MYCOLLECTION_H