//
// Created by WeiWang on 24-11-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepository.h" resolved

#include "MusicRepository.h"
#include "ui_MusicRepository.h"


MusicRepository::MusicRepository(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicRepository) {
    ui->setupUi(this);
}

MusicRepository::~MusicRepository() {
    delete ui;
}
