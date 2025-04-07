//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYSINGLESONG_H
#define RECENTLYSINGLESONG_H

#include "SortOptionMenu.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class RecentlySingleSong;
}

QT_END_NAMESPACE

class RecentlySingleSong : public QWidget {
    Q_OBJECT

public:
    explicit RecentlySingleSong(QWidget *parent = nullptr);

    ~RecentlySingleSong() override;

private:
    void initUi();

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_recently_play_toolButton_clicked();

    void on_recently_download_toolButton_clicked();

    void on_recently_share_toolButton_clicked();

    void on_recently_sort_toolButton_clicked();

    void on_recently_batch_toolButton_clicked();

public slots:
    //排序相关
    void onDefaultSort();

    void onAddTimeSort(const bool &down);

    void onSongNameSort(const bool &down);

    void onSingerSort(const bool &down);

    void onDurationSort(const bool &down);

    void onPlayCountSort(const bool &down);

    void onRandomSort();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::RecentlySingleSong *ui;
    QAction *m_searchAction; //专门为了设置图片
    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;

};
#endif //RECENTLYSINGLESONG_H
