//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CollectVideo.h" resolved

#include "CollectVideo.h"
#include "ui_CollectVideo.h"


CollectVideo::CollectVideo(QWidget *parent) :
    QWidget(parent), ui(new Ui::CollectVideo) {
    ui->setupUi(this);
}

CollectVideo::~CollectVideo() {
    delete ui;
}
