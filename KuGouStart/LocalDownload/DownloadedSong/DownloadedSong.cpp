//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DownloadedSong.h" resolved

#include "DownloadedSong.h"
#include "ui_DownloadedSong.h"


DownloadedSong::DownloadedSong(QWidget *parent) :
    QWidget(parent), ui(new Ui::DownloadedSong) {
    ui->setupUi(this);
}

DownloadedSong::~DownloadedSong() {
    delete ui;
}
