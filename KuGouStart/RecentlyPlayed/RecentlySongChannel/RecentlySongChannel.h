//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYSONGCHANNEL_H
#define RECENTLYSONGCHANNEL_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class RecentlySongChannel;
}

QT_END_NAMESPACE

class RecentlySongChannel : public QWidget {
    Q_OBJECT

public:
    explicit RecentlySongChannel(QWidget *parent = nullptr);

    ~RecentlySongChannel() override;

private:
    void initUi();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::RecentlySongChannel *ui;
    QAction *m_searchAction; //专门为了设置图片

};


#endif //RECENTLYSONGCHANNEL_H
