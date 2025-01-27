//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Downloading.h" resolved

#include "Downloading.h"
#include "ui_Downloading.h"


Downloading::Downloading(QWidget *parent) :
    QWidget(parent), ui(new Ui::Downloading) {
    ui->setupUi(this);
}

Downloading::~Downloading() {
    delete ui;
}
