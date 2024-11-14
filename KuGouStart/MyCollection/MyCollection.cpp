//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyCollection.h" resolved

#include "MyCollection.h"
#include "ui_MyCollection.h"


MyCollection::MyCollection(QWidget *parent) :
    QWidget(parent), ui(new Ui::MyCollection) {
    ui->setupUi(this);
}

MyCollection::~MyCollection() {
    delete ui;
}
