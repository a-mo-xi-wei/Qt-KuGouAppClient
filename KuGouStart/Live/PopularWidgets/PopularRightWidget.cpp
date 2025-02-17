//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PopularRightWidget.h" resolved

#include "PopularRightWidget.h"
#include "ui_PopularRightWidget.h"


PopularRightWidget::PopularRightWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PopularRightWidget) {
    ui->setupUi(this);
}

PopularRightWidget::~PopularRightWidget() {
    delete ui;
}
