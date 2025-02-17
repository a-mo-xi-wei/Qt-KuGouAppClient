//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveBlockWidget.h" resolved

#include "LiveBlockWidget.h"
#include "ui_LiveBlockWidget.h"


LiveBlockWidget::LiveBlockWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LiveBlockWidget) {
    ui->setupUi(this);
}

LiveBlockWidget::~LiveBlockWidget() {
    delete ui;
}
