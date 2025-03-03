//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLSONGLIST_H
#define ALLSONGLIST_H

#include <QWidget>

#if defined(ALLSONGLIST_LIBRARY)
#define ALLSONGLIST_EXPORT Q_DECL_EXPORT
#else
#define ALLSONGLIST_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class AllSongList; }
QT_END_NAMESPACE

class ALLSONGLIST_EXPORT AllSongList : public QWidget {
Q_OBJECT

public:
    explicit AllSongList(QWidget *parent = nullptr);
    ~AllSongList() override;

private:
    Ui::AllSongList *ui;
};


#endif //ALLSONGLIST_H
