//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Search.h" resolved

#include "Search.h"
#include "ui_Search.h"


Search::Search(QWidget *parent) :
    QWidget(parent), ui(new Ui::Search) {
    ui->setupUi(this);
}

Search::~Search() {
    delete ui;
}
