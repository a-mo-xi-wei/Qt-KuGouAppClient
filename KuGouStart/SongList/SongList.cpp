//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SongList.h" resolved

#include "SongList.h"
#include "ui_SongList.h"


SongList::SongList(QWidget *parent) :
    QWidget(parent), ui(new Ui::SongList) {
    ui->setupUi(this);
}

SongList::~SongList() {
    delete ui;
}
