//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveMusicPartWidget.h" resolved

#include "LiveMusicPartWidget.h"
#include "ui_LiveMusicPartWidget.h"


LiveMusicPartWidget::LiveMusicPartWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LiveMusicPartWidget) {
    ui->setupUi(this);
}

LiveMusicPartWidget::~LiveMusicPartWidget() {
    delete ui;
}

void LiveMusicPartWidget::setTitleName(const QString &name) {
}
