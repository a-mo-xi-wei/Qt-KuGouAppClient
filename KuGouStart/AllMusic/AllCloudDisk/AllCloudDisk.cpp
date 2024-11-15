//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllCloudDisk.h" resolved

#include "AllCloudDisk.h"
#include "ui_AllCloudDisk.h"


AllCloudDisk::AllCloudDisk(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllCloudDisk) {
    ui->setupUi(this);
}

AllCloudDisk::~AllCloudDisk() {
    delete ui;
}
