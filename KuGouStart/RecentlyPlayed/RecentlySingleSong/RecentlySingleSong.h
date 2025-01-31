//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYSINGLESONG_H
#define RECENTLYSINGLESONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class RecentlySingleSong;
}

QT_END_NAMESPACE

class RecentlySingleSong : public QWidget {
    Q_OBJECT

public:
    explicit RecentlySingleSong(QWidget *parent = nullptr);

    ~RecentlySingleSong() override;

private:
    void initUi();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::RecentlySingleSong *ui;
    QAction *m_searchAction; //专门为了设置图片

};
#endif //RECENTLYSINGLESONG_H
