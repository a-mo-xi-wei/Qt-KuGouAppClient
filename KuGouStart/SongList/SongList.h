//
// Created by WeiWang on 24-11-14.
//

#ifndef SONGLIST_H
#define SONGLIST_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SongList; }
QT_END_NAMESPACE

class SongList : public QWidget {
Q_OBJECT

public:
    explicit SongList(QWidget *parent = nullptr);
    ~SongList() override;

private:
    Ui::SongList *ui;
};


#endif //SONGLIST_H
