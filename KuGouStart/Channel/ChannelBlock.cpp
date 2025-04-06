//
// Created by WeiWang on 24-12-3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChannelBlock.h" resolved

#include "ChannelBlock.h"
#include "ui_ChannelBlock.h"
#include "ElaToolTip.h"

#include <QFile>
#include <QRandomGenerator>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ChannelBlock::ChannelBlock(QWidget *parent) :
    QWidget(parent), ui(new Ui::ChannelBlock)
{
    ui->setupUi(this);
    {
        this->setObjectName("block");
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channelblock.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

ChannelBlock::~ChannelBlock() {
    delete ui;
}

void ChannelBlock::setCoverPix(const QString &pixmapPath)const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void ChannelBlock::initUi()const {
    //遮罩设置
    auto& mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#5192FE")));
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));
    mask.setStander(120);
    //mask.setMove(true);
    //扩展MyBlockWidget响应范围
    ui->cover_widget->setExpandRespond(true);
    ui->cover_widget->setPopularDirection(1);
    ui->cover_widget->setHaveNumberUnit(false);
    ui->cover_widget->setPopularBtnText(QString::number(QRandomGenerator::global()->bounded(1,1000)));
}

void ChannelBlock::setTitleText(const QString &title) {
    this->m_titleText = title;
    auto title_label_toolTip = new ElaToolTip(ui->title_label);
    title_label_toolTip->setToolTip(this->m_titleText);
    updateTitleText();
}
void ChannelBlock::updateTitleText()const {
    //设置字体测量工具
    auto font = ui->title_label->font();
    QFontMetrics fm(font);
    auto elidedText = fm.elidedText(this->m_titleText,Qt::ElideRight,this->width()-10);
    ui->title_label->setText(elidedText);
}

void ChannelBlock::setSingerSongText(const QString &singerSong) {
    this->m_singerSongText = singerSong;
    auto singe_song_label_toolTip = new ElaToolTip(ui->singe_song_label);
    singe_song_label_toolTip->setToolTip(this->m_singerSongText);
    updateSingerSongText();
}

void ChannelBlock::updateSingerSongText()const {
    //设置字体测量工具
    auto font = ui->singe_song_label->font();
    QFontMetrics fm(font);
    auto elidedText = fm.elidedText(this->m_singerSongText,Qt::ElideRight,this->width()-10);
    ui->singe_song_label->setText(elidedText);
}

void ChannelBlock::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    ui->cover_widget->onShowMask();
    //ui->cover_widget->getMask().animationUp();
    ui->title_label->setStyleSheet(QStringLiteral("color:#2FAEFF;"));
}

void ChannelBlock::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    ui->cover_widget->onHideMask();
    //ui->cover_widget->getMask().animationDown();
    ui->title_label->setStyleSheet(QStringLiteral("color:black;"));
}

void ChannelBlock::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void ChannelBlock::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void ChannelBlock::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}