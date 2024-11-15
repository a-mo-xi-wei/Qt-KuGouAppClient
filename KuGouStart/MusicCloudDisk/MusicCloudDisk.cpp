//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicCloudDisk.h" resolved

#include "MusicCloudDisk.h"

#include "ui_MusicCloudDisk.h"

#include <QButtonGroup>


MusicCloudDisk::MusicCloudDisk(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicCloudDisk)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    initStackedWidget();
}

MusicCloudDisk::~MusicCloudDisk() {
    delete ui;
}

void MusicCloudDisk::initStackedWidget() {
    initUploadedSong();
    initUploadingSong();
    this->m_buttonGroup->addButton(ui->uploaded_song_pushButton);
    this->m_buttonGroup->addButton(ui->uploading_song_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MusicCloudDisk::initUploadedSong() {
    this->m_uploadedSong = std::make_unique<UploadedSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_uploadedSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_uploadedSong.get());
}

void MusicCloudDisk::initUploadingSong() {
    this->m_uploadingSong = std::make_unique<UploadingSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_uploadingSong.get());
}

void MusicCloudDisk::on_uploaded_song_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_uploadedSong.get());
    qDebug()<<"点击uploadedSong";
}

void MusicCloudDisk::on_uploading_song_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_uploadingSong.get());
    qDebug()<<"点击uploadingSong";
}

