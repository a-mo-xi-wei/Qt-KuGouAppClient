//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingerWidget.h" resolved

#include "SingerWidget.h"
#include "ui_SingerWidget.h"


SingerWidget::SingerWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SingerWidget) {
    ui->setupUi(this);
}

SingerWidget::~SingerWidget() {
    delete ui;
}
