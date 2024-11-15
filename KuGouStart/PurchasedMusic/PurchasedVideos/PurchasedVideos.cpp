//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PurchasedVideos.h" resolved

#include "PurchasedVideos.h"
#include "ui_PurchasedVideos.h"


PurchasedVideos::PurchasedVideos(QWidget *parent) :
    QWidget(parent), ui(new Ui::PurchasedVideos) {
    ui->setupUi(this);
}

PurchasedVideos::~PurchasedVideos() {
    delete ui;
}
