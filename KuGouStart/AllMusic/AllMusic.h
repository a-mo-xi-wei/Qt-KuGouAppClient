//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLMUSIC_H
#define ALLMUSIC_H

#include"AllWidget.h"
#include"AllLove.h"
#include"AllSongList.h"
#include"AllRecent.h"
#include"AllLocal.h"
#include"AllPaid.h"
#include"AllCloudDisk.h"

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class AllMusic; }
QT_END_NAMESPACE

class AllMusic : public QWidget {
Q_OBJECT

public:
    explicit AllMusic(QWidget *parent = nullptr);

    ~AllMusic() override;

    void initStackedWidget();

    void initAllWidget();

    void initAllLove();

    void initAllSongList();

    void initAllRecent();

    void initAllLocal();

    void initAllPaid();

    void initAllCloudDisk();

private slots:
    void on_all_pushButton_clicked();

    void on_love_pushButton_clicked();

    void on_song_list_pushButton_clicked();

    void on_recent_pushButton_clicked();

    void on_local_pushButton_clicked();

    void on_paid_pushButton_clicked();

    void on_cloud_disk_pushButton_clicked();

private:
    Ui::AllMusic *ui;
    std::unique_ptr<QButtonGroup>   m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<AllWidget>      m_allWidget{};
    std::unique_ptr<AllLove>        m_allLove{};
    std::unique_ptr<AllSongList>    m_allSongList{};
    std::unique_ptr<AllRecent>      m_allRecent{};
    std::unique_ptr<AllLocal>       m_allLocal{};
    std::unique_ptr<AllPaid>        m_allPaid{};
    std::unique_ptr<AllCloudDisk>   m_allCloudDisk{};
};


#endif //ALLMUSIC_H
