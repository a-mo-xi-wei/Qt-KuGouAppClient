//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySongChannel.h" resolved

#include "RecentlySongChannel.h"
#include "ui_RecentlySongChannel.h"


RecentlySongChannel::RecentlySongChannel(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlySongChannel) {
    ui->setupUi(this);
}

RecentlySongChannel::~RecentlySongChannel() {
    delete ui;
}
