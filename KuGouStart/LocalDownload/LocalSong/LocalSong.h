/**
 * @file LocalSong.h
 * @brief 定义 LocalSong 类，提供本地歌曲管理界面
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#ifndef LOCALSONG_H
#define LOCALSONG_H

#include "MusicItemWidget.h"
#include "SortOptionMenu/SortOptionMenu.h"
#include "libhttp.h"

#include <QQueue>
#include <QWidget>

class QMediaPlayer;
class QScrollBar;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LocalSong;
}
QT_END_NAMESPACE

/**
 * @class LocalSong
 * @brief 本地歌曲管理界面类，支持播放、排序、搜索、上传和服务器同步
 */
class LocalSong : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化本地歌曲界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit LocalSong(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~LocalSong() override;

    /**
     * @brief 播放下一首歌曲
     */
    void playNextSong();

    /**
     * @brief 播放上一首歌曲
     */
    void playPrevSong();

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示、图标、搜索动作和布局
     */
    void initUi();

    /**
     * @brief 获取媒体元数据
     * @note 提取标题、歌手、封面、时长并上传至服务器
     */
    void getMetaData();

    /**
     * @brief 加载下一首歌曲
     * @note 从队列中取出并播放
     */
    void loadNextSong();

    /**
     * @brief 获取菜单位置
     * @param pos 鼠标位置
     * @note 动态调整菜单位置以适应屏幕
     */
    void getMenuPosition(const QPoint &pos);

    /**
     * @brief 自定义排序
     * @param comparator 比较器函数
     * @note 根据比较器重新排列歌曲
     */
    void MySort(std::function<bool(const MusicItemWidget *, const MusicItemWidget *)> comparator);

    /**
     * @brief 更新当前播放索引
     * @note 在排序或删除后更新播放位置
     */
    void updateCurPlayIndex();

    /**
     * @brief 初始化音乐项
     * @param item 音乐项控件
     * @note 设置高亮颜色、圆角和信号连接
     */
    void initMusicItem(MusicItemWidget *item);

    /**
     * @brief 从服务器同步歌曲列表
     * @note 获取服务器歌曲列表并更新本地
     */
    void fetchAndSyncServerSongList();

    /**
     * @brief 设置播放高亮
     * @param item 音乐项控件
     * @note 高亮当前播放歌曲并更新播放次数
     */
    void setPlayItemHighlight(MusicItemWidget *item);

private slots:
    /**
     * @brief 全部播放按钮点击槽函数
     * @note 播放第一首歌曲并取消循环
     */
    void on_local_all_play_toolButton_clicked();

    /**
     * @brief 添加歌曲按钮点击槽函数
     * @note 打开文件对话框添加歌曲
     */
    void on_local_add_toolButton_clicked();

    /**
     * @brief 上传按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_upload_toolButton_clicked();

    /**
     * @brief 分享按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_local_share_toolButton_clicked();

    /**
     * @brief 专辑按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_local_album_toolButton_clicked();

    /**
     * @brief 排序按钮点击槽函数
     * @note 显示排序菜单
     */
    void on_local_sort_toolButton_clicked();

    /**
     * @brief 批量操作按钮点击槽函数
     * @note 显示未实现提示
     */
    void on_local_batch_toolButton_clicked();

    /**
     * @brief 搜索按钮点击槽函数
     * @note 触发搜索信号
     */
    void on_search_pushButton_clicked();

public slots:
    /**
     * @brief 音频播放结束槽函数
     * @note 顺序播放时自动播放下一首
     */
    void onAudioFinished();

    /**
     * @brief 最大化屏幕处理槽函数
     * @note 更新当前播放高亮
     */
    void onMaxScreenHandle();

public slots:
    /**
     * @brief 默认排序
     * @note 按添加时间升序
     */
    void onDefaultSort();

    /**
     * @brief 添加时间排序
     * @param down 是否降序
     */
    void onAddTimeSort(const bool &down);

    /**
     * @brief 歌曲名称排序
     * @param down 是否降序
     */
    void onSongNameSort(const bool &down);

    /**
     * @brief 歌手排序
     * @param down 是否降序
     */
    void onSingerSort(const bool &down);

    /**
     * @brief 时长排序
     * @param down 是否降序
     */
    void onDurationSort(const bool &down);

    /**
     * @brief 播放次数排序
     * @param down 是否降序
     */
    void onPlayCountSort(const bool &down);

    /**
     * @brief 随机排序
     */
    void onRandomSort();

public slots:
    /**
     * @brief 下一首播放槽函数
     * @note 显示未实现提示
     */
    void onItemNextPlay();

    /**
     * @brief 添加到播放队列槽函数
     * @note 显示未实现提示
     */
    void onItemAddToPlayQueue();

    /**
     * @brief 添加到新歌单槽函数
     * @note 显示未实现提示
     */
    void onItemAddToNewSongList();

    /**
     * @brief 添加到喜爱槽函数
     * @note 显示未实现提示
     */
    void onItemAddToLove();

    /**
     * @brief 添加到收藏槽函数
     * @note 显示未实现提示
     */
    void onItemAddToCollect();

    /**
     * @brief 添加到播放列表槽函数
     * @note 显示未实现提示
     */
    void onItemAddToPlayList();

    /**
     * @brief 下载槽函数
     * @note 显示未实现提示
     */
    void onItemDownload();

    /**
     * @brief 分享槽函数
     * @note 显示未实现提示
     */
    void onItemShare();

    /**
     * @brief 评论槽函数
     * @note 显示未实现提示
     */
    void onItemComment();

    /**
     * @brief 相似歌曲槽函数
     * @note 显示未实现提示
     */
    void onItemSameSong();

    /**
     * @brief 查看歌曲信息槽函数
     * @note 显示未实现提示
     */
    void onItemViewSongInfo();

    /**
     * @brief 删除歌曲槽函数
     * @param idx 歌曲索引
     * @note 删除本地和服务器歌曲
     */
    void onItemDeleteSong(const int &idx);

    /**
     * @brief 在文件资源管理器中打开槽函数
     * @note 显示未实现提示
     */
    void onItemOpenInFile();

    /**
     * @brief 搜索槽函数
     * @note 显示未实现提示
     */
    void onItemSearch();

    /**
     * @brief 上传槽函数
     * @note 显示未实现提示
     */
    void onItemUpLoad();

signals:
    /**
     * @brief 搜索更多音乐信号
     */
    void find_more_music();

    /**
     * @brief 播放音乐信号
     * @param localPath 本地歌曲路径
     */
    void playMusic(const QString &localPath);

    /**
     * @brief 更新数量标签信号
     * @param count 歌曲数量
     */
    void updateCountLabel(const int &count);

    /**
     * @brief 取消循环播放信号
     */
    void cancelLoopPlay();

protected:
    /**
     * @brief 窗口大小调整事件
     * @param event 大小调整事件
     * @note 调整滚动区域高度
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理搜索图标的动态切换
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::LocalSong                       *ui;                        ///< UI 指针
    std::unique_ptr<QMediaPlayer>        m_player;                  ///< 媒体播放器
    QVector<SongInfor>                   m_locationMusicVector;     ///< 本地歌曲信息
    QVector<SongInfor>                   m_lastLocationMusicVector; ///< 上次歌曲信息
    QVector<MusicItemWidget *>           m_musicItemVector;         ///< 音乐项控件
    MusicItemWidget                     *m_curPlayItemWidget{};     ///< 当前播放控件
    bool                                 m_isOrderPlay = false;     ///< 是否顺序播放
    QAction                             *m_searchAction{};          ///< 搜索动作
    QString                              m_mediaPath;               ///< 当前媒体路径
    QQueue<QString>                      m_songQueue;               ///< 歌曲队列
    int                                  m_curPlayIndex = -1;       ///< 当前播放索引
    int                                  m_setPlayIndex = -1;       ///< 设置播放索引
    bool                                 m_deleteSelf = -false;     ///< 是否删除自身
    SortOptionMenu                      *m_sortOptMenu{};           ///< 排序选项菜单
    bool                                 m_isSorting = false;       ///< 是否正在排序
    QPoint                               m_menuPosition;            ///< 菜单位置
    CLibhttp                             m_libHttp;                 ///< HTTP 请求库
};

#endif // LOCALSONG_H