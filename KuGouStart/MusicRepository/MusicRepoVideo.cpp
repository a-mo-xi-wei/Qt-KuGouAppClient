//
// Created by WeiWang on 24-11-30.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepoVideo.h" resolved

#include "MusicRepoVideo.h"
#include "ui_MusicRepoVideo.h"

#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>


// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MusicRepoVideo::MusicRepoVideo(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicRepoVideo) {
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/repovideo.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

MusicRepoVideo::~MusicRepoVideo() {
    delete ui;
}

void MusicRepoVideo::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath,10);
}

void MusicRepoVideo::setVideoName(const QString &name) {
    this->m_videoName = name;
    updateVideoNameText();
}

void MusicRepoVideo::setIconPix(const QString &pix) const {
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

void MusicRepoVideo::setAuthor(const QString &author) {
    this->m_videoAuthor = author;
    updateVideoAuthorText();
}

void MusicRepoVideo::initUi() {
    ui->cover_widget->setRightPopularBtnIcon(QStringLiteral(":/Res/tabIcon/video-white.svg"));
    ui->cover_widget->setPopularDirection(2);
    ui->cover_widget->setAspectRatio(2);
    ui->cover_widget->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 10, 'f', 1));
    ui->cover_widget->setPopularBtnLeftPadding(8);
    ui->cover_widget->installEventFilter(this);
}

void MusicRepoVideo::updateVideoNameText() const {
    //设置字体测量工具
    auto font = ui->video_name_label->font();
    QFontMetrics fm(font);
    ui->video_name_label->setToolTip(this->m_videoName);
    auto elidedText = fm.elidedText(this->m_videoName,Qt::ElideRight,ui->info_widget->width()-20);
    ui->video_name_label->setText(elidedText);
}

void MusicRepoVideo::updateVideoAuthorText() const {
    //设置字体测量工具
    auto font = ui->video_author_label->font();
    QFontMetrics fm(font);
    ui->video_author_label->setToolTip(this->m_videoAuthor);
    auto elidedText = fm.elidedText(this->m_videoAuthor,Qt::ElideRight,ui->info_widget->width()-20);
    ui->video_author_label->setText(elidedText);
}

void MusicRepoVideo::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoVideo::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoVideo::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

bool MusicRepoVideo::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->cover_widget) {
        if (event->type() == QEvent::Enter) {
            if (!this->m_isEnter) {
                this->m_isEnter = true;
                ui->cover_widget->setPopularDirection(0);
            }
        } else if (event->type() == QEvent::Leave) {
            if (this->m_isEnter) {
                this->m_isEnter = false;
                ui->cover_widget->setPopularDirection(2);
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MusicRepoVideo::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()/2);
    updateVideoNameText();
    updateVideoAuthorText();
}
