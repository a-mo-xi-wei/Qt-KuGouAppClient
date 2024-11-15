//
// Created by WeiWang on 24-11-14.
//

#ifndef PURCHASEDMUSIC_H
#define PURCHASEDMUSIC_H

#include"PaidSingle.h"
#include"PurchasedAlbums.h"
#include"PurchasedVideos.h"

#include <QWidget>

class QButtonGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class PurchasedMusic; }
QT_END_NAMESPACE

class PurchasedMusic : public QWidget {
Q_OBJECT

public:
    explicit PurchasedMusic(QWidget *parent = nullptr);

    ~PurchasedMusic() override;

    void initStackedWidget();

    void initPaidSingle();

    void initPurchasedAlbums();

    void initPurchasedVideos();

private slots:
    void on_paid_single_pushButton_clicked();

    void on_purchased_albums_pushButton_clicked();

    void on_purchased_videos_pushButton_clicked();

private:
    Ui::PurchasedMusic *ui;
    std::unique_ptr<QButtonGroup>   m_buttonGroup{};
    //堆栈窗口
    std::unique_ptr<PaidSingle>     m_paidSingle{};
    std::unique_ptr<PurchasedAlbums> m_purchasedAlbums{};
    std::unique_ptr<PurchasedVideos> m_purchasedVideos{};
};


#endif //PURCHASEDMUSIC_H
