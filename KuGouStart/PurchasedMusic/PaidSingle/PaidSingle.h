//
// Created by WeiWang on 24-11-15.
//

#ifndef PAIDSINGLE_H
#define PAIDSINGLE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class PaidSingle; }
QT_END_NAMESPACE

class PaidSingle : public QWidget {
Q_OBJECT

public:
    explicit PaidSingle(QWidget *parent = nullptr);
    ~PaidSingle() override;

private:
    Ui::PaidSingle *ui;
};


#endif //PAIDSINGLE_H
