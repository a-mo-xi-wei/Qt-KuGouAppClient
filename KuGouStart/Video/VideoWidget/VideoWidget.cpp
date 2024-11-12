//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoWidget.h" resolved

#include "VideoWidget.h"
#include "ui_VideoWidget.h"


VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoWidget) {
    ui->setupUi(this);
}

VideoWidget::~VideoWidget() {
    delete ui;
}
