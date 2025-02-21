//
// Created by WeiWang on 25-1-25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SongListBlockWidget.h" resolved

#include "SongListBlockWidget.h"

#include <QMouseEvent>

#include "ui_SongListBlockWidget.h"


SongListBlockWidget::SongListBlockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SongListBlockWidget)
{
    ui->setupUi(this);
    initUi();
}

SongListBlockWidget::~SongListBlockWidget() {
    delete ui;
}

void SongListBlockWidget::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void SongListBlockWidget::setTitleText(const QString &title) const {
    ui->title_label->setText(title);
}

void SongListBlockWidget::initUi() const {
    //blockWidget设置
    ui->cover_widget->setExpandRespond(false);
    ui->cover_widget->setPopularDirection(1);
    ui->cover_widget->setHaveNumberUnit(false);
    ui->cover_widget->setLeftPopularBtnIcon("");
    ui->cover_widget->setLeftPopularBtnWidth(30);
    ui->cover_widget->setPopularBtnText("0首");
    ui->cover_widget->setPopularBtnLeftPadding(0);
    //遮罩设置
    auto& mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(Qt::white);
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setMaskColor(QColor(0, 0, 0, 100));
    mask.setEnterWidgetChangeCursor(false);
    mask.setCursor(Qt::ArrowCursor);
}

void SongListBlockWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    //ui->cover_widget->onShowMask();
}

void SongListBlockWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    //ui->cover_widget->onHideMask();
}

void SongListBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void SongListBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void SongListBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}
