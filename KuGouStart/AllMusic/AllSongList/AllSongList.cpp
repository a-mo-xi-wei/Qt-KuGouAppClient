//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllSongList.h" resolved

#include "AllSongList.h"
#include "ui_AllSongList.h"


AllSongList::AllSongList(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllSongList) {
    ui->setupUi(this);
}

AllSongList::~AllSongList() {
    delete ui;
}
