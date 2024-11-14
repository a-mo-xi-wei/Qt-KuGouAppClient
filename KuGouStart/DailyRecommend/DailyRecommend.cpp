//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DailyRecommend.h" resolved

#include "DailyRecommend.h"
#include "ui_DailyRecommend.h"


DailyRecommend::DailyRecommend(QWidget *parent) :
    QWidget(parent), ui(new Ui::DailyRecommend) {
    ui->setupUi(this);
}

DailyRecommend::~DailyRecommend() {
    delete ui;
}
