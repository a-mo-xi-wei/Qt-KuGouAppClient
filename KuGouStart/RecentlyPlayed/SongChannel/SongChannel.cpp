//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SongChannel.h" resolved

#include "SongChannel.h"
#include "ui_SongChannel.h"


SongChannel::SongChannel(QWidget *parent) :
    QWidget(parent), ui(new Ui::SongChannel) {
    ui->setupUi(this);
}

SongChannel::~SongChannel() {
    delete ui;
}
