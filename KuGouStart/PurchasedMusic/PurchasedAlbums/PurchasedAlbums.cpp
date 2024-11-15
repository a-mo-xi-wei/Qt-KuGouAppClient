//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PurchasedAlbums.h" resolved

#include "PurchasedAlbums.h"
#include "ui_PurchasedAlbums.h"


PurchasedAlbums::PurchasedAlbums(QWidget *parent) :
    QWidget(parent), ui(new Ui::PurchasedAlbums) {
    ui->setupUi(this);
}

PurchasedAlbums::~PurchasedAlbums() {
    delete ui;
}
