//
// Created by WeiWang on 25-3-25.
//

#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QStack>

#include "TitleOptionMenu/TitleOptionMenu.h"
#include "ElaExitDialog.h"

#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

class RippleButton;

QT_BEGIN_NAMESPACE

namespace Ui {
    class TitleWidget;
}

QT_END_NAMESPACE

class MYWINDOW_EXPORT TitleWidget : public QWidget {
    Q_OBJECT

public:
    enum StackType {
        TitleLive = 0,
        ListenBook = 1,
        Search = 2,
        RecommendForYou = 3,
        MusicRepository = 4,
        Channel = 5,
        Video = 6,
        SongList = 7,
        DailyRecommend = 8,
        Collection = 9,
        LocalDownload = 10,
        MusicCloudDisk = 11,
        PurchasedMusic = 12,
        RecentlyPlayed = 13,
        AllMusic = 14,
    };

    explicit TitleWidget(QWidget *parent = nullptr);

    void showMenu(const QPoint &pos);

    RippleButton* max_toolButton() const;

    ~TitleWidget() override;

private:
    void initUi();

    void setMenuPosition(const QPoint &pos);

    static QPixmap getRoundedPixmap(const QPixmap &src, QSize size, int radius);

    void setTitleIndex(const int& index) const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    //title
    void on_title_return_toolButton_clicked();

    void on_title_refresh_toolButton_clicked();

    void on_title_music_pushButton_clicked();

    void on_title_live_pushButton_clicked();

    void on_title_listen_book_pushButton_clicked();

    void on_title_search_pushButton_clicked();

    void on_menu_toolButton_clicked();

    void on_min_toolButton_clicked();

    void on_max_toolButton_clicked();

    void on_close_toolButton_clicked();

public slots:
    void onLeftMenu_recommend_clicked();

    void onLeftMenu_musicRepository_clicked();

    void onLeftMenu_channel_clicked();

    void onLeftMenu_video_clicked();

    void onLeftMenu_live_clicked();

    void onLeftMenu_songList_clicked();

    void onLeftMenu_dailyRecommend_clicked();

    void onLeftMenu_collection_clicked();

    void onLeftMenu_localDownload_clicked();

    void onLeftMenu_musicCloudDisk_clicked();

    void onLeftMenu_purchasedMusic_clicked();

    void onLeftMenu_recentlyPlayed_clicked();

    void onLeftMenu_allMusic_clicked();

signals:
    void currentStackChange(const int& index, const bool& slide = false);

    void leftMenuShow(const bool& flag);

    void doubleClicked(); // 自定义的双击信号

    void showAboutDialog();

    void exit();

    void maxScreen();

    void refresh();

private:
    Ui::TitleWidget *ui;
    QStack<StackType>               m_backTypeStack;
    QStack<StackType>               m_frontTypeStack;
    StackType                       m_lastType;
    StackType                       m_curType;
    std::unique_ptr<ElaExitDialog>  m_closeDialog{};
    //标题菜单相关
    QPixmap                         m_originalCover;
    TitleOptionMenu*                m_titleOptMenu{};
    QPoint                          m_menuPosition;
};

#endif //TITLEWIDGET_H
