//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllMusic.h" resolved

#include "AllMusic.h"

#include <QButtonGroup>

#include "ui_AllMusic.h"


AllMusic::AllMusic(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllMusic)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    initStackedWidget();
}

AllMusic::~AllMusic() {
    delete ui;
}

void AllMusic::initStackedWidget() {
    initAllWidget();
    initAllLove();
    initAllSongList();
    initAllLocal();
    initAllRecent();
    initAllPaid();
    initAllCloudDisk();
    this->m_buttonGroup->addButton(ui->all_pushButton);
    this->m_buttonGroup->addButton(ui->love_pushButton);
    this->m_buttonGroup->addButton(ui->song_list_pushButton);
    this->m_buttonGroup->addButton(ui->recent_pushButton);
    this->m_buttonGroup->addButton(ui->local_pushButton);
    this->m_buttonGroup->addButton(ui->paid_pushButton);
    this->m_buttonGroup->addButton(ui->cloud_disk_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void AllMusic::initAllWidget() {
    this->m_allWidget = std::make_unique<AllWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allWidget.get());
    ui->stackedWidget->setCurrentWidget(this->m_allWidget.get());
}

void AllMusic::initAllLove() {
    this->m_allLove = std::make_unique<AllLove>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allLove.get());
}

void AllMusic::initAllSongList() {
    this->m_allSongList = std::make_unique<AllSongList>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allSongList.get());
}

void AllMusic::initAllRecent() {
    this->m_allRecent = std::make_unique<AllRecent>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allRecent.get());
}

void AllMusic::initAllLocal() {
    this->m_allLocal = std::make_unique<AllLocal>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allLocal.get());
}

void AllMusic::initAllPaid() {
    this->m_allPaid = std::make_unique<AllPaid>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allPaid.get());
}

void AllMusic::initAllCloudDisk() {
    this->m_allCloudDisk = std::make_unique<AllCloudDisk>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allCloudDisk.get());
}

void AllMusic::on_all_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allWidget.get());
    qDebug()<<"点击allWidget";
}

void AllMusic::on_love_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allLove.get());
    qDebug()<<"点击allLove";
}

void AllMusic::on_song_list_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allSongList.get());
    qDebug()<<"点击allSongList";
}

void AllMusic::on_recent_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allRecent.get());
    qDebug()<<"点击allRecent";
}

void AllMusic::on_local_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allLocal.get());
    qDebug()<<"点击allLocal";
}

void AllMusic::on_paid_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allPaid.get());
    qDebug()<<"点击allPaid";
}

void AllMusic::on_cloud_disk_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_allCloudDisk.get());
}
