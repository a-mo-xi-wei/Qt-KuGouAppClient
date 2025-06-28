/**
 * @file MusicItemWidget.h
 * @brief 定义 MusicItemWidget 类，提供音乐条目控件功能
 * @author [WeiWang]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include "MyMenu.h"
#include "SongOptionMenu/SongOptionMenu.h"

#include <QDateTime>
#include <QFrame>

#if defined(MUSICITEMWIDGET_LIBRARY)
#define MUSICITEMWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MUSICITEMWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QPointF;
class QTimer;
class QLabel;
class QToolButton;
class QSpacerItem;

/**
 * @struct SongInformation
 * @brief 存储音乐条目信息的结构体
 */
typedef struct SongInformation
{
    int                     index;        ///< 音乐索引
    QString                 coverUrl;     ///< 网络封面图片路径
    QPixmap                 cover;        ///< 封面图片
    QString                 songName;     ///< 歌曲名称
    QString                 singer;       ///< 歌手名称
    QString                 duration;     ///< 歌曲时长
    QString                 album;        ///< 专辑名称
    QString                 hash;         ///< 歌曲hash值
    QString                 mediaPath;    ///< 本地媒体文件路径
    QString                 netUrl;       ///< 网络文件路径
    QDateTime               addTime;      ///< 添加时间
    int                     playCount;    ///< 播放次数

    /**
     * @brief 比较运算符
     * @param info 待比较的歌曲信息
     * @return 是否相等
     */
    bool operator==(const struct SongInformation &info) const
    {
        return info.songName == this->songName && info.singer == this->singer && info.duration == this->duration;
    }
} SongInfor;

/**
 * @class MusicItemWidget
 * @brief 音乐条目控件类，支持涟漪效果、菜单操作和播放状态
 */
class MUSICITEMWIDGET_EXPORT MusicItemWidget : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音乐条目控件
     * @param info 歌曲信息
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MusicItemWidget(SongInfor info, QWidget *parent = nullptr);

    /**
     * @brief 额外添加的设置封面图片的接口
     * @param pix 封面图片
     */
    void setCover(const QPixmap& pix);

    /**
     * @brief 设置索引文本
     * @param index 索引值
     */
    void setIndexText(const int &index);

    /**
     * @brief 设置定时器时间间隔，控制填充速度
     * @param timeInterval 时间间隔（毫秒）
     */
    void setInterval(const int &timeInterval) const;

    /**
     * @brief 设置涟漪填充颜色
     * @param fillcolor 填充颜色
     */
    void setFillColor(const QColor &fillcolor);

    /**
     * @brief 设置圆角半径
     * @param radius_ 圆角半径
     */
    void setRadius(const int &radius_);

    /**
     * @brief 设置歌曲信息
     * @param info 歌曲信息
     */
    void setInformation(const SongInfor &info);

    /**
     * @brief 设置播放状态
     * @param state 是否播放
     */
    void setPlayState(const bool &state);

    /**
     * @brief 获取菜单显示位置
     * @param pos 鼠标位置
     */
    void getMenuPosition(const QPoint &pos);

    /**
     * @brief 设置高亮提示
     * @param highlight 是否高亮
     * @note 高亮item项
     */
    void setHighlight(bool highlight);

protected:
    /**
     * @brief 鼠标进入事件
     * @param event 进入事件对象
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件
     * @param event 事件对象
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 绘制事件
     * @param event 绘图事件对象
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 大小调整事件
     * @param event 大小调整事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 鼠标双击事件
     * @param event 鼠标事件对象
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    //smallWidget
    /**
     * @brief 播放按钮点击处理
     */
    void onPlayToolBtnClicked();

    /**
     * @brief 下一首按钮点击处理
     */
    void onPlayNextToolBtnClicked();

    /**
     * @brief 下载按钮点击处理
     */
    void onDownloadToolBtnClicked();

    /**
     * @brief 收藏按钮点击处理
     */
    void onCollectToolBtnClicked();

    /**
     * @brief 更多按钮点击处理
     */
    void onMoreToolBtnClicked();

    //menu (中转)
    /**
     * @brief 播放菜单项处理
     */
    void onPlay();

    /**
     * @brief 下一首播放菜单项处理
     */
    void onNextPlay();

    /**
     * @brief 添加到播放队列菜单项处理
     */
    void onAddToPlayQueue();

    /**
     * @brief 添加到新歌单菜单项处理
     */
    void onAddToNewSongList();

    /**
     * @brief 添加到喜欢菜单项处理
     */
    void onAddToLove();

    /**
     * @brief 添加到收藏菜单项处理
     */
    void onAddToCollect();

    /**
     * @brief 添加到播放列表菜单项处理
     */
    void onAddToPlayList();

    /**
     * @brief 下载菜单项处理
     */
    void onDownload();

    /**
     * @brief 分享菜单项处理
     */
    void onShare();

    /**
     * @brief 评论菜单项处理
     */
    void onComment();

    /**
     * @brief 相似歌曲菜单项处理
     */
    void onSameSong();

    /**
     * @brief 查看歌曲信息菜单项处理
     */
    void onViewSongInfo();

    /**
     * @brief 删除歌曲菜单项处理
     * @param idx 歌曲索引
     */
    void onDeleteSong(const int &idx);

    /**
     * @brief 在文件管理器中打开菜单项处理
     */
    void onOpenInFile();

    /**
     * @brief 搜索菜单项处理
     */
    void onSearch();

    /**
     * @brief 上传菜单项处理
     */
    void onUpLoad();

signals:
    /**
     * @brief 播放信号
     */
    void play();

    /**
     * @brief 下一首播放信号
     */
    void nextPlay();

    /**
     * @brief 添加到播放队列信号
     */
    void addToPLayQueue();

    /**
     * @brief 添加到新歌单信号
     */
    void addToNewSongList();

    /**
     * @brief 添加到喜欢信号
     */
    void addToLove();

    /**
     * @brief 添加到收藏信号
     */
    void addToCollect();

    /**
     * @brief 添加到播放列表信号
     */
    void addToPlayList();

    /**
     * @brief 下载信号
     */
    void download();

    /**
     * @brief 分享信号
     */
    void share();

    /**
     * @brief 评论信号
     */
    void comment();

    /**
     * @brief 相似歌曲信号
     */
    void sameSong();

    /**
     * @brief 查看歌曲信息信号
     */
    void viewSongInfo();

    /**
     * @brief 删除歌曲信号
     * @param idx 歌曲索引
     */
    void deleteSong(const int &idx);

    /**
     * @brief 在文件管理器中打开信号
     */
    void openInFile();

    /**
     * @brief 搜索信号
     */
    void search();

    /**
     * @brief 上传信号
     */
    void upload();

private:
    /**
     * @brief 初始化用户界面
     */
    void initUi();

private:
    //基础控件
    QLabel*                 m_indexLab{};          ///< 索引标签
    QLabel*                 m_coverLab{};          ///< 封面标签
    QLabel*                 m_nameLab{};           ///< 歌曲名称标签
    QLabel*                 m_singerLab{};         ///< 歌手名称标签
    QLabel*                 m_albumLab{};          ///< 专辑名称标签
    QLabel*                 m_durationLab{};       ///< 时长标签
    QToolButton*            m_playToolBtn{};       ///< 播放按钮
    QToolButton*            m_playNextToolBtn{};   ///< 下一首按钮
    QToolButton*            m_downloadToolBtn{};   ///< 下载按钮
    QToolButton*            m_collectToolBtn{};    ///< 收藏按钮
    QToolButton*            m_moreToolBtn{};       ///< 更多按钮

    //跳转选中高亮
    bool m_isHighlighted = false;                  ///< 是否高亮
    int m_highlightAlpha = 0;                      ///< 当前透明度 (0-255)
    int m_highlightDirection = 1;                  ///< 透明度变化方向 (1增加, -1减少)
    QTimer* m_blinkTimer;                          ///< 闪烁定时器

    bool                    m_isPlaying = false;   ///< 是否正在播放
    //菜单相关
    SongOptionMenu*         m_songOptMenu{};       ///< 歌曲选项菜单
    QPoint                  m_menuPosition;        ///< 菜单显示位置
    //歌曲信息相关
    int                     m_index;               ///< 歌曲索引
    QPixmap                 m_cover;               ///< 封面图片
    QString                 m_name;                ///< 歌曲名称
    QString                 m_singer;              ///< 歌手名称
    QString                 m_album ;              ///< 专辑名称
    QString                 m_duration;            ///< 歌曲时长

public:
    SongInfor               m_information;         ///< 歌曲信息

private:
    //涟漪效果相关
    int                     timeInterval = 5;      ///< 定时器时间间隔（毫秒）
    QTimer*                 timer{};               ///< 定时器对象
    QPointF                 mouse_point;           ///< 鼠标位置
    int                     max_radius;            ///< 最大涟漪半径
    int                     radius = 0;            ///< 当前涟漪半径
    int                     radius_var = 10;       ///< 半径变化量
    QColor                  fill_color;            ///< 填充颜色
    int                     frame_radius = 0;      ///< 圆角半径
};

#endif // MUSICITEMWIDGET_H