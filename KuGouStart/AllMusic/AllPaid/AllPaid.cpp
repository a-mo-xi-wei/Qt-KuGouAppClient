//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllPaid.h" resolved

#include "AllPaid.h"
#include "ui_AllPaid.h"


AllPaid::AllPaid(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllPaid) {
    ui->setupUi(this);
}

AllPaid::~AllPaid() {
    delete ui;
}
