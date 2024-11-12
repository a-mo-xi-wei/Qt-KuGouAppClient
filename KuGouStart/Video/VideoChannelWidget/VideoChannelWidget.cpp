//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoChannelWidget.h" resolved

#include "VideoChannelWidget.h"
#include "ui_VideoChannelWidget.h"


VideoChannelWidget::VideoChannelWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoChannelWidget) {
    ui->setupUi(this);
}

VideoChannelWidget::~VideoChannelWidget() {
    delete ui;
}
