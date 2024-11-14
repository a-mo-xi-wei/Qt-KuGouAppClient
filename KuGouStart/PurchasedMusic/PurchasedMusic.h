//
// Created by WeiWang on 24-11-14.
//

#ifndef PURCHASEDMUSIC_H
#define PURCHASEDMUSIC_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class PurchasedMusic; }
QT_END_NAMESPACE

class PurchasedMusic : public QWidget {
Q_OBJECT

public:
    explicit PurchasedMusic(QWidget *parent = nullptr);
    ~PurchasedMusic() override;

private:
    Ui::PurchasedMusic *ui;
};


#endif //PURCHASEDMUSIC_H
