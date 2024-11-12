//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Channel.h" resolved

#include "Channel.h"
#include "ui_Channel.h"


Channel::Channel(QWidget *parent) :
    QWidget(parent), ui(new Ui::Channel) {
    ui->setupUi(this);
}

Channel::~Channel() {
    delete ui;
}
