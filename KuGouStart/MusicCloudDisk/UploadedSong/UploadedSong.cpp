//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadedSong.h" resolved

#include "UploadedSong.h"
#include "ui_UploadedSong.h"


UploadedSong::UploadedSong(QWidget *parent) :
    QWidget(parent), ui(new Ui::UploadedSong) {
    ui->setupUi(this);
}

UploadedSong::~UploadedSong() {
    delete ui;
}
