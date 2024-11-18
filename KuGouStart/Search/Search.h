//
// Created by WeiWang on 24-11-18.
//

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Search; }
QT_END_NAMESPACE

class Search : public QWidget {
Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search() override;

private:
    Ui::Search *ui;
};


#endif //SEARCH_H
