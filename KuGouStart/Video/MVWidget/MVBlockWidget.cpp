//
// Created by WeiWang on 24-12-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVBlockWidget.h" resolved

#include "MVBlockWidget.h"
#include "ui_MVBlockWidget.h"
#include "logger.hpp"

#include <QFile>
#include <QMouseEvent>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MVBlockWidget::MVBlockWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MVBlockWidget) {
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/block.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

MVBlockWidget::~MVBlockWidget() {
    delete ui;
}

void MVBlockWidget::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void MVBlockWidget::setTitle(const QString &title) {
    this->m_title = title;
    updateTitleText();
}

void MVBlockWidget::setDescription(const QString &description) {
    this->m_description = description;
    updateDescText();
}

void MVBlockWidget::hideDesc() {
    ui->desc_label->hide();
}

void MVBlockWidget::initUi() {
    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setMaskColor(QColor(0,0,0,100));
    mask.setStander(120);
    ui->cover_widget->setAspectRatio(2);

}

void MVBlockWidget::updateTitleText() const {
    //设置字体测量工具
    auto font = ui->title_label->font();
    QFontMetrics fm(font);
    ui->title_label->setToolTip(this->m_title);
    //auto w = fm.horizontalAdvance(this->m_songName);
    //if(w >= ui->info_widget->width()-50) {
    auto elidedText = fm.elidedText(this->m_title,Qt::ElideRight,this->width()-10);
    ui->title_label->setText(elidedText);
}

void MVBlockWidget::updateDescText() const {
    //设置字体测量工具
    auto font = ui->desc_label->font();
    QFontMetrics fm(font);
    ui->desc_label->setToolTip(this->m_description);
    auto elidedText = fm.elidedText(this->m_description,Qt::ElideRight,this->width()-10);
    ui->desc_label->setText(elidedText);
}

void MVBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MVBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MVBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void MVBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()/2);

    //改变字数
    updateTitleText();
    updateDescText();
}
