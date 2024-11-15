//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllRecent.h" resolved

#include "AllRecent.h"
#include "ui_AllRecent.h"


AllRecent::AllRecent(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllRecent) {
    ui->setupUi(this);
}

AllRecent::~AllRecent() {
    delete ui;
}
