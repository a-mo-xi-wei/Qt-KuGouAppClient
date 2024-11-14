//
// Created by WeiWang on 24-11-14.
//

#ifndef LIVE_H
#define LIVE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Live; }
QT_END_NAMESPACE

class Live : public QWidget {
Q_OBJECT

public:
    explicit Live(QWidget *parent = nullptr);
    ~Live() override;

private:
    Ui::Live *ui;
};


#endif //LIVE_H
