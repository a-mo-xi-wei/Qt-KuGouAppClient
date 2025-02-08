//
// Created by WeiWang on 25-2-8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RankPartWidget.h" resolved

#include "RankPartWidget.h"
#include "ui_RankPartWidget.h"


RankPartWidget::RankPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RankPartWidget)
{
    ui->setupUi(this);
    ui->title_label->setStyleSheet(QStringLiteral("font-style: italic;"));
    initRankListArr();
}

RankPartWidget::~RankPartWidget() {
    delete ui;
}

void RankPartWidget::setTitle(const QString &title) const {
    ui->title_label->setText(title);
}

RankListWidget * RankPartWidget::getRankListWidget(const int &index) const {
    return this->m_rankListArr[index];
}

void RankPartWidget::initRankListArr() {
    this->m_rankListArr[0] = ui->widget_1;
    this->m_rankListArr[1] = ui->widget_2;
    this->m_rankListArr[2] = ui->widget_3;
    this->m_rankListArr[3] = ui->widget_4;
    this->m_rankListArr[4] = ui->widget_5;
}
