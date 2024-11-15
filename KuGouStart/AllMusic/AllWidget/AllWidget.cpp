//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllWidget.h" resolved

#include "AllWidget.h"
#include "ui_AllWidget.h"


AllWidget::AllWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllWidget) {
    ui->setupUi(this);
}

AllWidget::~AllWidget() {
    delete ui;
}
