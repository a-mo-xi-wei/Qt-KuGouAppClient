//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVChannel.h" resolved

#include "MVChannel.h"
#include "ui_MVChannel.h"


MVChannel::MVChannel(QWidget *parent) :
    QWidget(parent), ui(new Ui::MVChannel) {
    ui->setupUi(this);
}

MVChannel::~MVChannel() {
    delete ui;
}
