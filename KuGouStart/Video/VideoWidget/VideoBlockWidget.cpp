//
// Created by WeiWang on 24-12-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoBlockWidget.h" resolved

#include "VideoBlockWidget.h"
#include "ui_VideoBlockWidget.h"

#include <QFile>
#include <QPainterPath>
#include <QRandomGenerator>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

constexpr float AspectRation = 1.6;

VideoBlockWidget::VideoBlockWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoBlockWidget) {
    ui->setupUi(this);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/block.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

VideoBlockWidget::~VideoBlockWidget() {
    delete ui;
}

void VideoBlockWidget::setCoverPix(const QString &pixmapPath) {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void VideoBlockWidget::setVideoName(const QString &name) {
    this->m_videoName = name;
    updateVideoNameText();
}

void VideoBlockWidget::setIconPix(const QString &pix) {
    auto src = QPixmap(pix);
    auto size = ui->ico_label->size();
    auto len = size.width();
    QPixmap scaled = src.scaled(size , Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);
    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, len,len, len/2, len/2);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);
    ui->ico_label->setPixmap(dest);
}

void VideoBlockWidget::setAuthor(const QString &author) {
    this->m_videoAuthor = author;
    updateVideoAuthorText();
}

void VideoBlockWidget::setShowTip()const {
    ui->cover_widget->setShowTip();
    ui->cover_widget->setTipStyleSheet(QStringLiteral("border-radius:10px;background-color:#797978;color:white;"));
}

void VideoBlockWidget::setTipText(const QString &text) {
    ui->cover_widget->setTipLabText(text);
}

void VideoBlockWidget::initUi() {
    //遮罩设置
    auto &mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setMaskColor(QColor(0,0,0,20));
    ui->cover_widget->setLeftPopularBtnIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"));
    ui->cover_widget->setPopularDirection(1);
    ui->cover_widget->setAspectRatio(AspectRation);
    ui->cover_widget->setHaveNumberUnit(false);
    ui->cover_widget->setDurationBtnShow();
    QString durationText = "0";
    auto number = QRandomGenerator::global()->bounded(1,10);
    durationText += QString::number(number) + ":";
    number = QRandomGenerator::global()->bounded(1,60);
    if (number < 10) {
        durationText += "0";
    }
    durationText += QString::number(number);
    ui->cover_widget->setDurationBtnText(durationText);
    number = QRandomGenerator::global()->bounded(1,5000);
    if (number <= 500) {
        ui->cover_widget->setHaveNumberUnit(true);
        auto n = QRandomGenerator::global()->generateDouble() * 100;
        ui->cover_widget->setPopularBtnText(QString::number(n, 'f', 2));
    }
    else {
        ui->cover_widget->setPopularBtnText(QString::number(number));
    }

}

void VideoBlockWidget::updateVideoNameText() {
    //设置字体测量工具
    auto font = ui->video_name_label->font();
    QFontMetrics fm(font);
    ui->video_name_label->setToolTip(this->m_videoName);
    auto elidedText = fm.elidedText(this->m_videoName,Qt::ElideRight,ui->info_widget->width()-20);
    ui->video_name_label->setText(elidedText);
}

void VideoBlockWidget::updateVideoAuthorText() {
    //设置字体测量工具
    auto font = ui->video_author_label->font();
    QFontMetrics fm(font);
    ui->video_author_label->setToolTip(this->m_videoAuthor);
    auto elidedText = fm.elidedText(this->m_videoAuthor,Qt::ElideRight,ui->info_widget->width()-20);
    ui->video_author_label->setText(elidedText);
}

void VideoBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void VideoBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

bool VideoBlockWidget::eventFilter(QObject *watched, QEvent *event) {
    return QWidget::eventFilter(watched, event);
}

void VideoBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //this->setFixedHeight(event->size().width()/1.1);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()/AspectRation);
    updateVideoNameText();
    updateVideoAuthorText();
}
