//
// Created by WeiWang on 24-11-14.
//

#ifndef DEFAULTLIST_H
#define DEFAULTLIST_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class DefaultList; }
QT_END_NAMESPACE

class DefaultList : public QWidget {
Q_OBJECT

public:
    explicit DefaultList(QWidget *parent = nullptr);
    ~DefaultList() override;

private:
    Ui::DefaultList *ui;
};


#endif //DEFAULTLIST_H
