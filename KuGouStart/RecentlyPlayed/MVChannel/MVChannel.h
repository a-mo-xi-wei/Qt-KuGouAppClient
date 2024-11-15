//
// Created by WeiWang on 24-11-15.
//

#ifndef MVCHANNEL_H
#define MVCHANNEL_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MVChannel; }
QT_END_NAMESPACE

class MVChannel : public QWidget {
Q_OBJECT

public:
    explicit MVChannel(QWidget *parent = nullptr);
    ~MVChannel() override;

private:
    Ui::MVChannel *ui;
};


#endif //MVCHANNEL_H
