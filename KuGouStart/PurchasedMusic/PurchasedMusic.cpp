//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PurchasedMusic.h" resolved

#include "PurchasedMusic.h"
#include "ui_PurchasedMusic.h"


PurchasedMusic::PurchasedMusic(QWidget *parent) :
    QWidget(parent), ui(new Ui::PurchasedMusic) {
    ui->setupUi(this);
}

PurchasedMusic::~PurchasedMusic() {
    delete ui;
}
