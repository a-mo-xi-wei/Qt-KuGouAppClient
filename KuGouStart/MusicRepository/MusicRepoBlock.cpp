//
// Created by WeiWang on 24-11-30.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepoBlock.h" resolved

#include "MusicRepoBlock.h"
#include "ui_MusicRepoBlock.h"

#include <QFile>
#include <QMouseEvent>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


MusicRepoBlock::MusicRepoBlock(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicRepoBlock) {
    ui->setupUi(this);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/repoblock.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
}

MusicRepoBlock::~MusicRepoBlock() {
    delete ui;
}

void MusicRepoBlock::setCoverPix(const QString &pixmapPath) {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void MusicRepoBlock::setSongName(const QString &song) {
    this->m_songName = song;
    updateSongText();
}

void MusicRepoBlock::setSinger(const QString &singer) {
    this->m_singer = singer;
    updateSingerText();
}

void MusicRepoBlock::updateSongText() {
    //设置字体测量工具
    auto font = ui->song_label->font();
    QFontMetrics fm(font);
    ui->song_label->setToolTip(this->m_songName);
    //auto w = fm.horizontalAdvance(this->m_songName);
    //if(w >= ui->info_widget->width()-50) {
    auto elidedText = fm.elidedText(this->m_songName,Qt::ElideRight,ui->info_widget->width()-10);
    ui->song_label->setText(elidedText);
}

void MusicRepoBlock::updateSingerText() {
    //设置字体测量工具
    auto font = ui->singer_label->font();
    QFontMetrics fm(font);
    ui->singer_label->setToolTip(this->m_singer);
    auto elidedText = fm.elidedText(this->m_singer,Qt::ElideRight,ui->info_widget->width()-10);
    ui->singer_label->setText(elidedText);
}

void MusicRepoBlock::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoBlock::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoBlock::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoBlock::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width());

    //改变字数
    updateSongText();
    updateSingerText();
}
