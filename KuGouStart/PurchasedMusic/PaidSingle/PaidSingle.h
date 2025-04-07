//
// Created by WeiWang on 24-11-15.
//

#ifndef PAIDSINGLE_H
#define PAIDSINGLE_H

#include "SortOptionMenu.h"

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PaidSingle;
}

QT_END_NAMESPACE

class PaidSingle : public QWidget {
    Q_OBJECT

public:
    explicit PaidSingle(QWidget *parent = nullptr);

    ~PaidSingle() override;

private:
    void initUi();

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_single_play_toolButton_clicked();

    void on_single_download_toolButton_clicked();

    void on_single_share_toolButton_clicked();

    void on_single_sort_toolButton_clicked();

    void on_single_batch_toolButton_clicked();

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
    Ui::PaidSingle *ui;
    QAction *m_searchAction; //专门为了设置图片

    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;
};


#endif //PAIDSINGLE_H
