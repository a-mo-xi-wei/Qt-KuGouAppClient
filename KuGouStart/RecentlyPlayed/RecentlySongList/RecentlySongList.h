//
// Created by WeiWang on 24-11-15.
//

#ifndef RECENTLYSONGLIST_H
#define RECENTLYSONGLIST_H

#include <QWidget>

#if defined(RECENTLYSONGLIST_LIBRARY)
#define RECENTLYSONGLIST_EXPORT Q_DECL_EXPORT
#else
#define RECENTLYSONGLIST_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class RecentlySongList; }
QT_END_NAMESPACE

class RECENTLYSONGLIST_EXPORT RecentlySongList : public QWidget {
Q_OBJECT

public:
    explicit RecentlySongList(QWidget *parent = nullptr);
    ~RecentlySongList() override;

private:
    Ui::RecentlySongList *ui;
};


#endif //RECENTLYSONGLIST_H
