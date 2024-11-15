//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllLocal.h" resolved

#include "AllLocal.h"
#include "ui_AllLocal.h"


AllLocal::AllLocal(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllLocal) {
    ui->setupUi(this);
}

AllLocal::~AllLocal() {
    delete ui;
}
