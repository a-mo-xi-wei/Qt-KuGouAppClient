//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySingleSong.h" resolved

#include "RecentlySingleSong.h"
#include "ui_RecentlySingleSong.h"


RecentlySingleSong::RecentlySingleSong(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlySingleSong) {
    ui->setupUi(this);
}

RecentlySingleSong::~RecentlySingleSong() {
    delete ui;
}
