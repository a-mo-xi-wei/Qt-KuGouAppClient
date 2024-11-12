//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVWidget.h" resolved

#include "MVWidget.h"
#include "ui_MVWidget.h"


MVWidget::MVWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MVWidget) {
    ui->setupUi(this);
}

MVWidget::~MVWidget() {
    delete ui;
}
