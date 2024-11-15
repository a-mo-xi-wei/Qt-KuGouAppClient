//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySongList.h" resolved

#include "RecentlySongList.h"
#include "ui_RecentlySongList.h"


RecentlySongList::RecentlySongList(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlySongList) {
    ui->setupUi(this);
}

RecentlySongList::~RecentlySongList() {
    delete ui;
}
