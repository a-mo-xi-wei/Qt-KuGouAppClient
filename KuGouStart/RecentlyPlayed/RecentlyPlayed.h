//
// Created by WeiWang on 24-11-14.
//

#ifndef RECENTLYPLAYED_H
#define RECENTLYPLAYED_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyPlayed; }
QT_END_NAMESPACE

class RecentlyPlayed : public QWidget {
Q_OBJECT

public:
    explicit RecentlyPlayed(QWidget *parent = nullptr);
    ~RecentlyPlayed() override;

private:
    Ui::RecentlyPlayed *ui;
};


#endif //RECENTLYPLAYED_H
