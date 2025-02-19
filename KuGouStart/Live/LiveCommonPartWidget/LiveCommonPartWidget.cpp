//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveCommonPartWidget.h" resolved

#include "LiveCommonPartWidget.h"
#include "ui_LiveCommonPartWidget.h"


LiveCommonPartWidget::LiveCommonPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveCommonPartWidget)
{
    ui->setupUi(this);
}

LiveCommonPartWidget::~LiveCommonPartWidget() {
    delete ui;
}

void LiveCommonPartWidget::setTitleName(const QString &name) {
    ui->title_label->setText(name);
}
