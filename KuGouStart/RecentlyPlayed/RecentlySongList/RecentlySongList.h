//
// Created by WeiWang on 24-11-15.
//

#ifndef RECENTLYSONGLIST_H
#define RECENTLYSONGLIST_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RecentlySongList; }
QT_END_NAMESPACE

class RecentlySongList : public QWidget {
Q_OBJECT

public:
    explicit RecentlySongList(QWidget *parent = nullptr);
    ~RecentlySongList() override;

private:
    Ui::RecentlySongList *ui;
};


#endif //RECENTLYSONGLIST_H
