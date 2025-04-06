//
// Created by WeiWang on 25-2-8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RankListWidget.h" resolved

#include "RankListWidget.h"
#include "ui_RankListWidget.h"
#include "ElaToolTip.h"

RankListWidget::RankListWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RankListWidget) {
    ui->setupUi(this);
    ui->desc_label->setStyleSheet(QStringLiteral("color: black;"));
    ui->fire_label->setStyleSheet(QStringLiteral("border-image: url(\":/ListenBook/Res/listenbook/fire.svg\");"));
    ui->info_label->setStyleSheet(QStringLiteral("color: gray;"));
}

RankListWidget::~RankListWidget() {
    delete ui;
}

void RankListWidget::setDescText(const QString &text) const {
    //设置字体测量工具
    const auto font = ui->desc_label->font();
    const QFontMetrics fm(font);
    const QString elidedText = fm.elidedText(text, Qt::ElideRight, this->width()-110);
    ui->desc_label->setText(elidedText);

    auto desc_label_toolTip = new ElaToolTip(ui->desc_label);
    desc_label_toolTip->setToolTip(text);
}

void RankListWidget::setInfoText(const QString &text) const {
    //设置字体测量工具
    const auto font = ui->info_label->font();
    const QFontMetrics fm(font);
    const QString elidedText = fm.elidedText(text, Qt::ElideRight, ui->info_label->width());
    ui->info_label->setText(elidedText);
}

void RankListWidget::setCoverImg(const QString &path) const {
    ui->rank_cover_label->setStyleSheet(QString("border-radius: 5px;border-image: url(%1);").arg(path));
}

void RankListWidget::setRankMedal(const QString &path) const {
    ui->rank_number_label->setStyleSheet(QString("border-image: url(%1);").arg(path));
}

void RankListWidget::setRankNumber(const QString &number) const {
    ui->rank_number_label->setText(number);
    ui->rank_number_label->setStyleSheet(QString("font-size: 15px;color: gray;"));
}

void RankListWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    ui->desc_label->setStyleSheet(QStringLiteral("color: #26A1FF;"));
}

void RankListWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    ui->desc_label->setStyleSheet(QStringLiteral("color: black;"));
}
