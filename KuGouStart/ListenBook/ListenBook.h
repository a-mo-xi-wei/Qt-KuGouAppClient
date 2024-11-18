//
// Created by WeiWang on 24-11-18.
//

#ifndef LISTENBOOK_H
#define LISTENBOOK_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ListenBook; }
QT_END_NAMESPACE

class ListenBook : public QWidget {
Q_OBJECT

public:
    explicit ListenBook(QWidget *parent = nullptr);
    ~ListenBook() override;

private:
    Ui::ListenBook *ui;
};


#endif //LISTENBOOK_H
