//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENRECENTLYPLAY_H
#define LISTENRECENTLYPLAY_H

#include <QWidget>

#if defined(LISTENRECENTLYPLAY_LIBRARY)
#define LISTENRECENTLYPLAY_EXPORT Q_DECL_EXPORT
#else
#define LISTENRECENTLYPLAY_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class ListenRecentlyPlay; }
QT_END_NAMESPACE

class LISTENRECENTLYPLAY_EXPORT ListenRecentlyPlay : public QWidget {
Q_OBJECT

public:
    explicit ListenRecentlyPlay(QWidget *parent = nullptr);
    ~ListenRecentlyPlay() override;

private:
    Ui::ListenRecentlyPlay *ui;
};


#endif //LISTENRECENTLYPLAY_H
