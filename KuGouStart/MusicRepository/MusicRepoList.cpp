//
// Created by WeiWang on 24-11-24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepoList.h" resolved

#include "MusicRepoList.h"
#include "ui_MusicRepoList.h"

#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>


// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


MusicRepoList::MusicRepoList(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::MusicRepoList)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/repolist.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

MusicRepoList::~MusicRepoList() {
    delete ui;
}

void MusicRepoList::setCoverPix(const QString &pixmapPath) const {
    ui->cover_widget->setBorderImage(pixmapPath);
}

void MusicRepoList::setSongName(const QString &song) {
    this->m_songName = song;
    updateSongText();
}

void MusicRepoList::setSinger(const QString &singer) {
    this->m_singer = singer;
    updateSingerText();
}

void MusicRepoList::initUi() {
    //隐藏toolButton
    ui->play_add_toolButton->hide();
    ui->like_toolButton->hide();
    ui->comment_toolButton->hide();
    //遮罩设置
    auto& mask = ui->cover_widget->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor());
    mask.setHoverFillTriangleColor(QColor());
    mask.setEnterWidgetChangeCursor(false);
    //扩展MyBlockWidget响应范围
    ui->cover_widget->setExpandRespond(true);
    connect(this,&MusicRepoList::enterList,ui->cover_widget,&MyBlockWidget::onShowMask);
    connect(this,&MusicRepoList::leaveList,ui->cover_widget,&MyBlockWidget::onHideMask);

}

void MusicRepoList::updateSongText() const {
    //设置字体测量工具
    auto font = ui->song_label->font();
    QFontMetrics fm(font);
    ui->song_label->setToolTip(this->m_songName);
    //auto w = fm.horizontalAdvance(this->m_songName);
    //if(w >= ui->info_widget->width()-50) {
        auto elidedText = fm.elidedText(this->m_songName,Qt::ElideRight,ui->info_widget->width()-20);
        ui->song_label->setText(elidedText);
    //}
    //else {
    //    ui->song_label->setText(this->m_songName);
    //}
}

void MusicRepoList::updateSingerText() const {
    //设置字体测量工具
    auto font = ui->singer_label->font();
    QFontMetrics fm(font);
    ui->singer_label->setToolTip(this->m_singer);
    auto elidedText = fm.elidedText(this->m_singer,Qt::ElideRight,ui->info_widget->width()-20);
    ui->singer_label->setText(elidedText);

}

void MusicRepoList::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    if(!this->m_isEnter) {
        this->m_isEnter = true;//给MyBlockWidget发送进入事件
        emit enterList();
        //显示按钮
        ui->play_add_toolButton->show();
        ui->like_toolButton->show();
        ui->comment_toolButton->show();
        update();
    }
}

void MusicRepoList::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    if(this->m_isEnter) {
        this->m_isEnter = false;
        emit leaveList();//给MyBlockWidget发送离开事件
        //显示按钮
        ui->play_add_toolButton->hide();
        ui->like_toolButton->hide();
        ui->comment_toolButton->hide();
        update();
    }
}

void MusicRepoList::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    if(this->m_isEnter) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(QStringLiteral("#E8EAFB")));
        p.drawRoundedRect(rect(),10,10);
    }
    //改变字数
    updateSongText();
    updateSingerText();
}

void MusicRepoList::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoList::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepoList::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //改变字数
    updateSongText();
    updateSingerText();
}