//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DefaultList.h" resolved

#include "DefaultList.h"
#include "ui_DefaultList.h"


DefaultList::DefaultList(QWidget *parent) :
    QWidget(parent), ui(new Ui::DefaultList) {
    ui->setupUi(this);
}

DefaultList::~DefaultList() {
    delete ui;
}
