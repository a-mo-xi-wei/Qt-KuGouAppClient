//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYSINGLESONG_H
#define RECENTLYSINGLESONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RecentlySingleSong; }
QT_END_NAMESPACE

class RecentlySingleSong : public QWidget {
Q_OBJECT

public:
    explicit RecentlySingleSong(QWidget *parent = nullptr);
    ~RecentlySingleSong() override;

private:
    Ui::RecentlySingleSong *ui;
};


#endif //RECENTLYSINGLESONG_H
