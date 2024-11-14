//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyPlayed.h" resolved

#include "RecentlyPlayed.h"
#include "ui_RecentlyPlayed.h"


RecentlyPlayed::RecentlyPlayed(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlyPlayed) {
    ui->setupUi(this);
}

RecentlyPlayed::~RecentlyPlayed() {
    delete ui;
}
