//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENRECENTLYPLAY_H
#define LISTENRECENTLYPLAY_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenRecentlyPlay;
}

QT_END_NAMESPACE

class ListenRecentlyPlay : public QWidget {
    Q_OBJECT

public:
    explicit ListenRecentlyPlay(QWidget *parent = nullptr);

    ~ListenRecentlyPlay() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void switch_to_listen_recommend();

private:
    Ui::ListenRecentlyPlay *ui;
};


#endif //LISTENRECENTLYPLAY_H
