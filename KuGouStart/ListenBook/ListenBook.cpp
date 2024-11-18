//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenBook.h" resolved

#include "ListenBook.h"
#include "ui_ListenBook.h"


ListenBook::ListenBook(QWidget *parent) :
    QWidget(parent), ui(new Ui::ListenBook) {
    ui->setupUi(this);
}

ListenBook::~ListenBook() {
    delete ui;
}
