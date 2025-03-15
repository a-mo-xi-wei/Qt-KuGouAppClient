//
// Created by WeiWang on 25-1-27.
//

#ifndef DOWNLOADEDSONG_H
#define DOWNLOADEDSONG_H

#include "SortOptionMenu/SortOptionMenu.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DownloadedSong;
}

QT_END_NAMESPACE

class DownloadedSong : public QWidget {
    Q_OBJECT

public:
    explicit DownloadedSong(QWidget *parent = nullptr);

    ~DownloadedSong() override;

private:
    void initUi();

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_local_sort_toolButton_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::DownloadedSong *ui;
    QAction *m_searchAction; //专门为了设置图片
    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;

};


#endif //DOWNLOADEDSONG_H
