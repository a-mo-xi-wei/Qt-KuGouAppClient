//
// Created by WeiWang on 25-1-27.
//

#ifndef DOWNLOADING_H
#define DOWNLOADING_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Downloading; }
QT_END_NAMESPACE

class Downloading : public QWidget {
Q_OBJECT

public:
    explicit Downloading(QWidget *parent = nullptr);
    ~Downloading() override;

private:
    Ui::Downloading *ui;
};


#endif //DOWNLOADING_H
