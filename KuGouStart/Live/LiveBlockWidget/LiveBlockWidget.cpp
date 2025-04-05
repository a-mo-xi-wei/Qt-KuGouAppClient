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

void LiveBlockWidget::setCoverHeightExpanding() {
    this->setContentsMargins(0,0,0,0);
    ui->cover_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->cover_widget->setMinimumHeight(0);  // 清除最小高度限制
    ui->cover_widget->setMaximumHeight(QWIDGETSIZE_MAX);  // 清除最大高度限制
    update();
}

LiveBlockWidget::~LiveBlockWidget() {
    delete ui;
}

void LiveBlockWidget::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void LiveBlockWidget::setLeftBottomText(const QString &text) {
    ui->cover_widget->setPopularBtnText(text);
    ui->cover_widget->setToolTip(text);
    update();
}

void LiveBlockWidget::setLeftPopularBtnFontSize(const int &size, const bool &isBold) const {
    ui->cover_widget->setLeftPopularBtnFontSize(size,isBold);
}

void LiveBlockWidget::setTipLabText(const QString& text) const {
    ui->cover_widget->setTipLabText(" " + text + " ");
}

void LiveBlockWidget::setNoTipLab() const {
    ui->cover_widget->setShowTip(false);
}

void LiveBlockWidget::setAspectRatio(const float &aspectRatio) {
    this->m_aspectRatio = aspectRatio;
    ui->cover_widget->setAspectRatio(aspectRatio);
}

void LiveBlockWidget::setCircleStander(const int &stander) const {
    auto &mask = ui->cover_widget->getMask();
    mask.setStander(stander);
}

void LiveBlockWidget::setTipStyleSheet(const QString &styleSheet) const {
    ui->cover_widget->setTipStyleSheet(styleSheet);
}

void LiveBlockWidget::initUi() const {
    //设置tip数组
    std::vector<QString> tipArr {
        "第一开播","第二天开播","第三天开播","第四天开播","第五天开播","第六天开播",
        "德州","葫芦岛","淄博","深圳","洛阳","哈尔滨","临沂",
        "福州","东莞","常德","宝山","昆明","长沙","厦门",
        "合肥","苏州","龙岩","西安","嘉兴","上海","广州",
        "重庆","三明","龙岩","西安","商丘","常州","宁波",
        "威海","徐州","十佳工会推荐主播","宜昌","廊坊","菏泽",
    };
    ui->cover_widget->setPopularDirection(1);
    ui->cover_widget->setLeftPopularBtnIcon("");
    ui->cover_widget->setPopularBtnLeftPadding(0);
    ui->cover_widget->setLeftPopularBtnFontSize(13,true);
    ui->cover_widget->setAspectRatio(this->m_aspectRatio);
    ui->cover_widget->setShowTip();
    ui->cover_widget->setTipArr(tipArr);
    ui->cover_widget->setTipStyleSheet(QStringLiteral("border:1px solid rgba(255,255,255,50%);border-radius:10px;background-color:rgba(0,0,0,50%);color:white;"));
    ui->cover_widget->setHaveNumberUnit(false);

    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setDefaultFillTriangleColor(QColor(0,0,0,200));
    mask.setHoverFillCircleColor(Qt::white);
    mask.setHoverFillTriangleColor(QColor());
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
    //ui->cover_widget->setFixedHeight(ui->cover_widget->width()*5/4);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()/m_aspectRatio);
    //qDebug()<<"ui->cover_widget->width(): "<<ui->cover_widget->width()
    //<<"ui->cover_widget->height(): "<<ui->cover_widget->height();
}
