//
// Created by WeiWang on 24-11-14.
//

#ifndef SINGLESONG_H
#define SINGLESONG_H

#include "SortOptionMenu.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SingleSong;
}

QT_END_NAMESPACE

class SingleSong : public QWidget {
    Q_OBJECT

public:
    explicit SingleSong(QWidget *parent = nullptr);

    ~SingleSong() override;

private:
    void initUi();

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_collect_play_toolButton_clicked();

    void on_collect_download_toolButton_clicked();

    void on_collect_share_toolButton_clicked();

    void on_collect_zhuanji_toolButton_clicked();

    void on_collect_sort_toolButton_clicked();

    void on_collect_batch_toolButton_clicked();

    void on_search_pushButton_clicked();

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

signals:
    void find_more_music();

private:
    Ui::SingleSong *ui;
    QAction *m_searchAction; //专门为了设置图片
    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;
};


#endif //SINGLESONG_H
