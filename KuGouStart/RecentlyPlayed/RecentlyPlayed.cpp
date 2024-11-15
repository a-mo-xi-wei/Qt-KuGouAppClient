//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyPlayed.h" resolved

#include "RecentlyPlayed.h"

#include <QButtonGroup>

#include "ui_RecentlyPlayed.h"

RecentlyPlayed::RecentlyPlayed(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlyPlayed)
    ,m_buttonGroup((std::make_unique<QButtonGroup>(this)))
{
    ui->setupUi(this);
    initStackedWidet();
}

RecentlyPlayed::~RecentlyPlayed() {
    delete ui;
}

void RecentlyPlayed::initStackedWidet() {
    initSingleSong();
    initSongList();
    initVideoWidget();
    initSongChannel();
    initMVChannel();
    this->m_buttonGroup->addButton(ui->singleSong_pushButton);
    this->m_buttonGroup->addButton(ui->songList_pushButton);
    this->m_buttonGroup->addButton(ui->video_pushButton);
    this->m_buttonGroup->addButton(ui->song_channel_pushButton);
    this->m_buttonGroup->addButton(ui->MV_channel_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void RecentlyPlayed::initSingleSong() {
    this->m_singleSong = std::make_unique<SingleSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_singleSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
}

void RecentlyPlayed::initSongList() {
    this->m_songList = std::make_unique<RecentlySongList>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songList.get());
}

void RecentlyPlayed::initVideoWidget() {
    this->m_videoWidget = std::make_unique<RecentlyVideoWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_videoWidget.get());
}

void RecentlyPlayed::initSongChannel() {
    this->m_songChannel = std::make_unique<SongChannel>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songChannel.get());
}

void RecentlyPlayed::initMVChannel() {
    this->m_mvChannel = std::make_unique<MVChannel>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_mvChannel.get());
}

void RecentlyPlayed::on_singleSong_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
    qDebug()<<"点击singleSong";
}

void RecentlyPlayed::on_songList_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_songList.get());
    qDebug()<<"点击songList";
}

void RecentlyPlayed::on_video_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_videoWidget.get());
    qDebug()<<"点击video";
}

void RecentlyPlayed::on_song_channel_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_songChannel.get());
    qDebug()<<"点击songChannel";
}

void RecentlyPlayed::on_MV_channel_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_mvChannel.get());
    qDebug()<<"点击MVChannel";
}
