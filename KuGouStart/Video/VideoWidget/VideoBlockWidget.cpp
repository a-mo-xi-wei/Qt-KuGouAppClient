//
// Created by WeiWang on 24-12-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoBlockWidget.h" resolved

#include "VideoBlockWidget.h"
#include "ui_VideoBlockWidget.h"


VideoBlockWidget::VideoBlockWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoBlockWidget) {
    ui->setupUi(this);
}

VideoBlockWidget::~VideoBlockWidget() {
    delete ui;
}
