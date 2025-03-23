//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLSONGLIST_H
#define ALLSONGLIST_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AllSongList; }
QT_END_NAMESPACE

class AllSongList : public QWidget {
Q_OBJECT

public:
    explicit AllSongList(QWidget *parent = nullptr);
    ~AllSongList() override;

private:
    Ui::AllSongList *ui;
};


#endif //ALLSONGLIST_H
