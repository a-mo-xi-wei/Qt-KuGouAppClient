//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Live.h" resolved

#include "Live.h"
#include "ui_Live.h"


Live::Live(QWidget *parent) :
    QWidget(parent), ui(new Ui::Live) {
    ui->setupUi(this);
}

Live::~Live() {
    delete ui;
}
