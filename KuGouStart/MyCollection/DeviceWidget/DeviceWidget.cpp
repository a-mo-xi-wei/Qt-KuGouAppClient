//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DeviceWidget.h" resolved

#include "DeviceWidget.h"
#include "ui_DeviceWidget.h"


DeviceWidget::DeviceWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DeviceWidget) {
    ui->setupUi(this);
}

DeviceWidget::~DeviceWidget() {
    delete ui;
}
