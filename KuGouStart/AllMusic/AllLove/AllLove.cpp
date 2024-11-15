//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllLove.h" resolved

#include "AllLove.h"
#include "ui_AllLove.h"


AllLove::AllLove(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllLove) {
    ui->setupUi(this);
}

AllLove::~AllLove() {
    delete ui;
}
