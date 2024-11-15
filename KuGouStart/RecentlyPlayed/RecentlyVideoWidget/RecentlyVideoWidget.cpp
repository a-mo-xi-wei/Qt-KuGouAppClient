//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyVideoWidget.h" resolved

#include "RecentlyVideoWidget.h"
#include "ui_RecentlyVideoWidget.h"


RecentlyVideoWidget::RecentlyVideoWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlyVideoWidget) {
    ui->setupUi(this);
}

RecentlyVideoWidget::~RecentlyVideoWidget() {
    delete ui;
}
