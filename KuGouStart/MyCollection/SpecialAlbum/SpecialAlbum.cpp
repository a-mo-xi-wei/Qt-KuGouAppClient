//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SpecialAlbum.h" resolved

#include "SpecialAlbum.h"
#include "ui_SpecialAlbum.h"


SpecialAlbum::SpecialAlbum(QWidget *parent) :
    QWidget(parent), ui(new Ui::SpecialAlbum) {
    ui->setupUi(this);
}

SpecialAlbum::~SpecialAlbum() {
    delete ui;
}
