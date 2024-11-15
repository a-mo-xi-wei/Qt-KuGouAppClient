//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadingSong.h" resolved

#include "UploadingSong.h"
#include "ui_UploadingSong.h"


UploadingSong::UploadingSong(QWidget *parent) :
    QWidget(parent), ui(new Ui::UploadingSong) {
    ui->setupUi(this);
}

UploadingSong::~UploadingSong() {
    delete ui;
}
