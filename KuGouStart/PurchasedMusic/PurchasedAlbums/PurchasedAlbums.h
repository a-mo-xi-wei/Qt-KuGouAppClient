//
// Created by WeiWang on 24-11-15.
//

#ifndef PURCHASEDALBUMS_H
#define PURCHASEDALBUMS_H

#include <QWidget>

#if defined(PURCHASEDALBUMS_LIBRARY)
#define PURCHASEDALBUMS_EXPORT Q_DECL_EXPORT
#else
#define PURCHASEDALBUMS_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class PurchasedAlbums; }
QT_END_NAMESPACE

class PURCHASEDALBUMS_EXPORT PurchasedAlbums : public QWidget {
Q_OBJECT

public:
    explicit PurchasedAlbums(QWidget *parent = nullptr);
    ~PurchasedAlbums() override;

private:
    Ui::PurchasedAlbums *ui;
};


#endif //PURCHASEDALBUMS_H
