//
// Created by WeiWang on 24-11-14.
//

#ifndef MYCOLLECTION_H
#define MYCOLLECTION_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MyCollection; }
QT_END_NAMESPACE

class MyCollection : public QWidget {
Q_OBJECT

public:
    explicit MyCollection(QWidget *parent = nullptr);
    ~MyCollection() override;

private:
    Ui::MyCollection *ui;
};


#endif //MYCOLLECTION_H
