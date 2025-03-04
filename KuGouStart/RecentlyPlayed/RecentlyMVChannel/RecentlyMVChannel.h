//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYMVCHANNEL_H
#define RECENTLYMVCHANNEL_H

#include <QWidget>

#if defined(RECENTLYMVCHANNEL_LIBRARY)
#define RECENTLYMVCHANNEL_EXPORT Q_DECL_EXPORT
#else
#define RECENTLYMVCHANNEL_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyMVChannel; }
QT_END_NAMESPACE

class RECENTLYMVCHANNEL_EXPORT RecentlyMVChannel : public QWidget {
Q_OBJECT

public:
    explicit RecentlyMVChannel(QWidget *parent = nullptr);
    ~RecentlyMVChannel() override;

private:
    Ui::RecentlyMVChannel *ui;
};


#endif //RECENTLYMVCHANNEL_H
