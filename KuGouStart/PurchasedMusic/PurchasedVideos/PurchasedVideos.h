//
// Created by WeiWang on 24-11-15.
//

#ifndef PURCHASEDVIDEOS_H
#define PURCHASEDVIDEOS_H

#include <QWidget>

#if defined(PURCHASEDVIDEOS_LIBRARY)
#define PURCHASEDVIDEOS_EXPORT Q_DECL_EXPORT
#else
#define PURCHASEDVIDEOS_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class PurchasedVideos; }
QT_END_NAMESPACE

class PURCHASEDVIDEOS_EXPORT PurchasedVideos : public QWidget {
Q_OBJECT

public:
    explicit PurchasedVideos(QWidget *parent = nullptr);
    ~PurchasedVideos() override;

private:
    Ui::PurchasedVideos *ui;
};


#endif //PURCHASEDVIDEOS_H
