//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenMyDownload.h" resolved

#include "ListenMyDownload.h"
#include "ui_ListenMyDownload.h"


ListenMyDownload::ListenMyDownload(QWidget *parent) :
    QWidget(parent), ui(new Ui::ListenMyDownload) {
    ui->setupUi(this);
}

ListenMyDownload::~ListenMyDownload() {
    delete ui;
}
