//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PaidSingle.h" resolved

#include "PaidSingle.h"
#include "ui_PaidSingle.h"


PaidSingle::PaidSingle(QWidget *parent) :
    QWidget(parent), ui(new Ui::PaidSingle) {
    ui->setupUi(this);
}

PaidSingle::~PaidSingle() {
    delete ui;
}
