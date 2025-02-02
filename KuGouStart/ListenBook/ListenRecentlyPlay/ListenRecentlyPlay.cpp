//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecentlyPlay.h" resolved

#include "ListenRecentlyPlay.h"
#include "ui_ListenRecentlyPlay.h"


ListenRecentlyPlay::ListenRecentlyPlay(QWidget *parent) :
    QWidget(parent), ui(new Ui::ListenRecentlyPlay) {
    ui->setupUi(this);
}

ListenRecentlyPlay::~ListenRecentlyPlay() {
    delete ui;
}
