//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicCloudDisk.h" resolved

#include "MusicCloudDisk.h"
#include "ui_MusicCloudDisk.h"


MusicCloudDisk::MusicCloudDisk(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicCloudDisk) {
    ui->setupUi(this);
}

MusicCloudDisk::~MusicCloudDisk() {
    delete ui;
}
