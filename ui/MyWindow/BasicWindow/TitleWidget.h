/**
 * @file TitleWidget.h
 * @brief 定义 TitleWidget 类，提供标题栏功能和界面导航管理
 * @author WeiWang
 * @date 2025-03-25
 * @version 1.0
 */

#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QStack>
#include "TitleOptionMenu/TitleOptionMenu.h"
#include "ElaExitDialog.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class RippleButton;
namespace Ui {
    class TitleWidget;
}

/**
 * @class TitleWidget
 * @brief 自定义标题栏控件类，继承自 QWidget，支持界面导航、菜单显示和窗口控制
 */
class MYWINDOW_EXPORT TitleWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 界面类型枚举
     */
    enum StackType {
        TitleLive = 0,        ///< 直播界面
        ListenBook = 1,       ///< 听书界面
        Search = 2,           ///< 探索界面
        RecommendForYou = 3,  ///< 为你推荐界面
        MusicRepository = 4,  ///< 乐库界面
        Channel = 5,          ///< 频道界面
        Video = 6,            ///< 视频界面
        AiChat = 7,           ///< AI对话界面
        SongList = 8,         ///< 歌单界面
        DailyRecommend = 9,   ///< 每日推荐界面
        Collection = 10,      ///< 我的收藏界面
        LocalDownload = 11,   ///< 本地下载界面
        MusicCloudDisk = 12,  ///< 音乐云盘界面
        PurchasedMusic = 13,  ///< 已购音乐界面
        RecentlyPlayed = 14,  ///< 最近播放界面
        AllMusic = 15,        ///< 全部音乐界面
        SearchResult = 16     ///< 搜索结果界面
    };

    /**
     * @brief 构造函数，初始化标题栏控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit TitleWidget(QWidget *parent = nullptr);

    /**
     * @brief 获取最大化按钮
     * @return 最大化按钮指针
     */
    RippleButton *max_toolButton() const;

    /**
     * @brief 析构函数，释放资源
     */
    ~TitleWidget() override;

    /**
     * @brief 设置是否允许界面切换
     * @param flag 是否允许切换
     */
    void setEnableChange(const bool &flag);

    void setEnableTitleButton(const bool &flag);

private:
    /**
     * @brief 初始化界面元素
     */
    void initUi();

    /**
     * @brief 生成圆角图片
     * @param src 原始图片
     * @param size 目标尺寸
     * @param radius 圆角半径
     * @return 圆角图片
     */
    static QPixmap getRoundedPixmap(const QPixmap &src, QSize size, int radius);

    /**
     * @brief 设置标题索引，控制界面指示器显示
     * @param index 标题索引（1-4）
     */
    void setTitleIndex(const int &index) const;


protected:
    /**
     * @brief 重写鼠标双击事件，触发最大化
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief 重写鼠标按下事件，处理右键菜单
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 重写绘制事件，绘制带圆角的线性渐变阴影
     * @param ev 绘制事件
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 重写事件过滤器，处理鼠标返回/前进键和头像动画
     * @param watched 监视的对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 重写窗口大小改变事件，更新界面元素，初衷：发送信号给ElaSuggestBox，防止下拉框位置不改变
     * @param event 事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 返回按钮点击事件，触发界面回退
     */
    void on_title_return_toolButton_clicked();

    /**
     * @brief 刷新按钮点击事件，触发界面刷新
     */
    void on_title_refresh_toolButton_clicked();

    /**
     * @brief 音乐按钮点击事件，切换到音乐界面
     */
    void on_title_music_pushButton_clicked();

    /**
     * @brief 直播按钮点击事件，切换到直播界面
     */
    void on_title_live_pushButton_clicked();

    /**
     * @brief 听书按钮点击事件，切换到听书界面
     */
    void on_title_listen_book_pushButton_clicked();

    /**
     * @brief 探索按钮点击事件，切换到探索界面
     */
    void on_title_search_pushButton_clicked();

    /**
     * @brief 听歌识曲按钮点击事件，显示未实现提示
     */
    void on_listen_toolButton_clicked();

    /**
     * @brief 主题按钮点击事件，显示未实现提示
     */
    void on_theme_toolButton_clicked();

    /**
     * @brief 消息按钮点击事件，显示未实现提示
     */
    void on_message_toolButton_clicked();

    /**
     * @brief 菜单按钮点击事件，显示标题选项菜单
     */
    void on_menu_toolButton_clicked();

    /**
     * @brief 最小化按钮点击事件，最小化窗口
     */
    void on_min_toolButton_clicked();

    /**
     * @brief 最大化按钮点击事件，触发最大化信号
     */
    void on_max_toolButton_clicked();

public slots:
    /**
     * @brief 关闭按钮点击事件，显示退出对话框
     */
    void on_close_toolButton_clicked();

    /**
     * @brief 左侧菜单推荐项点击事件，切换到推荐界面
     */
    void onLeftMenu_recommend_clicked();

    /**
     * @brief 左侧菜单乐库项点击事件，切换到乐库界面
     */
    void onLeftMenu_musicRepository_clicked();

    /**
     * @brief 左侧菜单频道项点击事件，切换到频道界面
     */
    void onLeftMenu_channel_clicked();

    /**
     * @brief 左侧菜单视频项点击事件，切换到视频界面
     */
    void onLeftMenu_video_clicked();

    /**
     * @brief 左侧菜单直播项点击事件，切换到直播界面
     */
    void onLeftMenu_live_clicked();

    /**
     * @brief 左侧菜单AI对话项点击事件，切换到AI对话界面
     */
    void onLeftMenu_ai_chat_clicked();

    /**
     * @brief 左侧菜单歌单项点击事件，切换到歌单界面
     */
    void onLeftMenu_songList_clicked();

    /**
     * @brief 左侧菜单每日推荐项点击事件，切换到每日推荐界面
     */
    void onLeftMenu_dailyRecommend_clicked();

    /**
     * @brief 左侧菜单收藏项点击事件，切换到收藏界面
     */
    void onLeftMenu_collection_clicked();

    /**
     * @brief 左侧菜单本地下载项点击事件，切换到本地下载界面
     */
    void onLeftMenu_localDownload_clicked();

    /**
     * @brief 左侧菜单音乐云盘项点击事件，切换到音乐云盘界面
     */
    void onLeftMenu_musicCloudDisk_clicked();

    /**
     * @brief 左侧菜单已购音乐项点击事件，切换到已购音乐界面
     */
    void onLeftMenu_purchasedMusic_clicked();

    /**
     * @brief 左侧菜单最近播放项点击事件，切换到最近播放界面
     */
    void onLeftMenu_recentlyPlayed_clicked();

    /**
     * @brief 左侧菜单全部音乐项点击事件，切换到全部音乐界面
     */
    void onLeftMenu_allMusic_clicked();

signals:
    /**
     * @brief 界面切换信号，通知界面索引和是否滑动
     * @param index 目标界面索引
     * @param slide 是否使用滑动动画
     */
    void currentStackChange(const int &index);

    /**
     * @brief 左侧菜单显示信号，控制菜单显示状态
     * @param flag 是否显示菜单
     */
    void leftMenuShow(const bool &flag);

    /**
     * @brief 双击信号，当标题栏被双击时触发
     */
    void doubleClicked();

    /**
     * @brief 显示关于对话框信号
     */
    void showAboutDialog();

    /**
     * @brief 最大化信号，通知窗口最大化
     */
    void maxScreen();

    /**
     * @brief 刷新信号，通知界面刷新
     */
    void refresh();

    /**
     * @brief 搜索信号，通知搜索界面显示
     */
    void suggestionClicked(const QString &suggestText, const QVariantMap &suggestData);

    /**
     * @brief 输入框回车
     */
    void searchTextReturnPressed(const QString &searchText, const QVariantMap &suggestData = {});

private:
    Ui::TitleWidget *ui;                        ///< UI 界面对象
    QStack<StackType> m_backTypeStack;          ///< 后退导航栈
    QStack<StackType> m_frontTypeStack;         ///< 前进导航栈
    StackType m_lastType;                       ///< 上一次界面类型
    StackType m_curType;                        ///< 当前界面类型
    std::unique_ptr<ElaExitDialog> m_closeDialog; ///< 退出对话框
    QPixmap m_originalCover;                    ///< 原始头像图片
    TitleOptionMenu *m_titleOptMenu{};          ///< 标题选项菜单
    QPoint m_menuPosition;                      ///< 菜单显示位置
    bool m_enableChange = true;                 ///< 是否允许界面切换
};

#endif // TITLEWIDGET_H