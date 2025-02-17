//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveBigLeftWidget.h" resolved

#include "LiveBigLeftWidget.h"
#include "ui_LiveBigLeftWidget.h"


LiveBigLeftWidget::LiveBigLeftWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LiveBigLeftWidget) {
    ui->setupUi(this);
}

LiveBigLeftWidget::~LiveBigLeftWidget() {
    delete ui;
}
