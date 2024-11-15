//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PurchasedMusic.h" resolved

#include "PurchasedMusic.h"

#include <QButtonGroup>

#include "ui_PurchasedMusic.h"


PurchasedMusic::PurchasedMusic(QWidget *parent) :
    QWidget(parent), ui(new Ui::PurchasedMusic)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    initStackedWidget();
}

PurchasedMusic::~PurchasedMusic() {
    delete ui;
}

void PurchasedMusic::initStackedWidget() {
    initPaidSingle();
    initPurchasedAlbums();
    initPurchasedVideos();
    this->m_buttonGroup->addButton(ui->paid_single_pushButton);
    this->m_buttonGroup->addButton(ui->purchased_albums_pushButton);
    this->m_buttonGroup->addButton(ui->purchased_videos_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void PurchasedMusic::initPaidSingle() {
    this->m_paidSingle = std::make_unique<PaidSingle>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_paidSingle.get());
    ui->stackedWidget->setCurrentWidget(this->m_paidSingle.get());
}

void PurchasedMusic::initPurchasedAlbums() {
    this->m_purchasedAlbums = std::make_unique<PurchasedAlbums>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_purchasedAlbums.get());
}

void PurchasedMusic::initPurchasedVideos() {
    this->m_purchasedVideos = std::make_unique<PurchasedVideos>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_purchasedVideos.get());
}

void PurchasedMusic::on_paid_single_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_paidSingle.get());
    qDebug()<<"点击paidSingleWidget";
}

void PurchasedMusic::on_purchased_albums_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_purchasedAlbums.get());
    qDebug()<<"点击purchasedAlbumsWidget";
}

void PurchasedMusic::on_purchased_videos_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_purchasedVideos.get());
    qDebug()<<"点击purchasedVideosWidget";
}
