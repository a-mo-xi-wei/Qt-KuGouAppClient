//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyMVChannel.h" resolved

#include "RecentlyMVChannel.h"
#include "ui_RecentlyMVChannel.h"


RecentlyMVChannel::RecentlyMVChannel(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlyMVChannel) {
    ui->setupUi(this);
}

RecentlyMVChannel::~RecentlyMVChannel() {
    delete ui;
}
