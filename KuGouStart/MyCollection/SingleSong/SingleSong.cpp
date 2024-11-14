//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleSong.h" resolved

#include "SingleSong.h"
#include "ui_SingleSong.h"


SingleSong::SingleSong(QWidget *parent) :
    QWidget(parent), ui(new Ui::SingleSong) {
    ui->setupUi(this);
}

SingleSong::~SingleSong() {
    delete ui;
}
