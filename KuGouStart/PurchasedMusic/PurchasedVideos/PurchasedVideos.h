//
// Created by WeiWang on 24-11-15.
//

#ifndef PURCHASEDVIDEOS_H
#define PURCHASEDVIDEOS_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class PurchasedVideos;
}

QT_END_NAMESPACE

class PurchasedVideos : public QWidget {
    Q_OBJECT

public:
    explicit PurchasedVideos(QWidget *parent = nullptr);

    ~PurchasedVideos() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::PurchasedVideos *ui;
};


#endif //PURCHASEDVIDEOS_H
