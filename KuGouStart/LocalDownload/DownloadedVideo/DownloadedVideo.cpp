//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DownloadedVideo.h" resolved

#include "DownloadedVideo.h"
#include "ui_DownloadedVideo.h"


DownloadedVideo::DownloadedVideo(QWidget *parent) :
    QWidget(parent), ui(new Ui::DownloadedVideo) {
    ui->setupUi(this);
}

DownloadedVideo::~DownloadedVideo() {
    delete ui;
}
