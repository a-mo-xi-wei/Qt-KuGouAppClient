//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyCollection.h" resolved

#include "MyCollection.h"
#include "ui_MyCollection.h"

#include<QButtonGroup>

MyCollection::MyCollection(QWidget *parent) :
    QWidget(parent), ui(new Ui::MyCollection)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    initStackedWidget();
}

MyCollection::~MyCollection() {
    delete ui;
}

void MyCollection::initStackedWidget() {
    initSingleSong();
    initSongList();
    initSpecialAlbum();
    initCollectVideo();
    initSinger();
    initDevice();
    //设置互斥
    this->m_buttonGroup->addButton(ui->singleSong_pushButton);
    this->m_buttonGroup->addButton(ui->songList_pushButton);
    this->m_buttonGroup->addButton(ui->specialAlbum_pushButton);
    this->m_buttonGroup->addButton(ui->collectVideo_pushButton);
    this->m_buttonGroup->addButton(ui->singer_pushButton);
    this->m_buttonGroup->addButton(ui->device_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MyCollection::initSingleSong() {
    this->m_singleSong = std::make_unique<SingleSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_singleSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
}

void MyCollection::initSongList() {
    this->m_songList = std::make_unique<SongListWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songList.get());
}

void MyCollection::initSpecialAlbum() {
    this->m_specialAlbum = std::make_unique<SpecialAlbum>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_specialAlbum.get());
}

void MyCollection::initCollectVideo() {
    this->m_collectVideo = std::make_unique<CollectVideo>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_collectVideo.get());
}

void MyCollection::initSinger() {
    this->m_singerWidget = std::make_unique<SingerWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_singerWidget.get());
}

void MyCollection::initDevice() {
    this->m_deviceWidget = std::make_unique<DeviceWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_deviceWidget.get());
}

void MyCollection::on_singleSong_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
    qDebug()<<"点击singleSong";
}

void MyCollection::on_songList_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_songList.get());
    qDebug()<<"点击songList";
}

void MyCollection::on_specialAlbum_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_specialAlbum.get());
    qDebug()<<"点击specialAlbum";
}

void MyCollection::on_collectVideo_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_collectVideo.get());
    qDebug()<<"点击collectVideo";
}

void MyCollection::on_singer_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_singerWidget.get());
    qDebug()<<"点击singerWidget";
}

void MyCollection::on_device_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_deviceWidget.get());
    qDebug()<<"点击deviceWidget";
}
