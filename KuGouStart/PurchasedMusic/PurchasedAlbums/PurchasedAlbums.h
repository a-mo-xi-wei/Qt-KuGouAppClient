//
// Created by WeiWang on 24-11-15.
//

#ifndef PURCHASEDALBUMS_H
#define PURCHASEDALBUMS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PurchasedAlbums; }
QT_END_NAMESPACE

class PurchasedAlbums : public QWidget {
Q_OBJECT

public:
    explicit PurchasedAlbums(QWidget *parent = nullptr);
    ~PurchasedAlbums() override;

private:
    Ui::PurchasedAlbums *ui;
};


#endif //PURCHASEDALBUMS_H
