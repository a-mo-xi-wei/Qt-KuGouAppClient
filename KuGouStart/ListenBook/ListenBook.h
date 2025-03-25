//
// Created by WeiWang on 24-11-18.
//

#ifndef LISTENBOOK_H
#define LISTENBOOK_H

#include "ListenRecommend/ListenRecommend.h"
#include "ListenMyDownload/ListenMyDownload.h"
#include "ListenRecentlyPlay/ListenRecentlyPlay.h"

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenBook;
}

QT_END_NAMESPACE

class ListenBook : public QWidget {
    Q_OBJECT

public:
    explicit ListenBook(QWidget *parent = nullptr);

    ~ListenBook() override;

private:
    void initUi();

    void initStackedWidget();

    void initListenRecommend();

    void initListenMyDownload();

    void initListenRecentlyPlay();

    void enableButton(const bool& flag) const;

private slots:
    void on_listen_recommend_toolButton_clicked();

    void on_listen_my_download_toolButton_clicked();

    void on_recently_play_toolButton_clicked();

private:
    Ui::ListenBook *ui;
    std::unique_ptr<QButtonGroup> m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<ListenRecommend>    m_listenRecommend{};
    std::unique_ptr<ListenMyDownload>   m_listenMyDownload{};
    std::unique_ptr<ListenRecentlyPlay> m_listenRecentlyPlay{};
};


#endif //LISTENBOOK_H
