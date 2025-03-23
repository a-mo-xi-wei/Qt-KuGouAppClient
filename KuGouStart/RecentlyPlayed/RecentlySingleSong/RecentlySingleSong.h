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
    void on_recently_sort_toolButton_clicked();

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
