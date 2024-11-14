//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SongListWidget.h" resolved

#include "SongListWidget.h"
#include "ui_SongListWidget.h"


SongListWidget::SongListWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SongListWidget) {
    ui->setupUi(this);
}

SongListWidget::~SongListWidget() {
    delete ui;
}
