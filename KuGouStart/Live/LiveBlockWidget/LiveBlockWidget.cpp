//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveBlockWidget.h" resolved

#include "LiveBlockWidget.h"

#include <QMouseEvent>

#include "ui_LiveBlockWidget.h"


LiveBlockWidget::LiveBlockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveBlockWidget)
{
    ui->setupUi(this);
    initUi();
}

LiveBlockWidget::~LiveBlockWidget() {
    delete ui;
}

void LiveBlockWidget::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void LiveBlockWidget::initUi() {
    ui->cover_widget->setPopularDirection(1);
    ui->cover_widget->setLeftPopularBtnIcon("");
    ui->cover_widget->setAspectRatio(0.8);
    ui->cover_widget->setShowTip();
    ui->cover_widget->setHaveNumberUnit(false);
    ui->cover_widget->setPopularBtnText("123");

    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setDefaultFillTriangleColor(QColor(0,0,0, 0));
    mask.setHoverFillCircleColor(Qt::white);
    mask.setHoverFillTriangleColor(QColor(255, 255, 255, 0));
    mask.setStander(100);
    mask.setMove(true);

}

void LiveBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void LiveBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void LiveBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void LiveBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()*1.25);
}
