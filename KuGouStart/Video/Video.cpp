//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Video.h" resolved

#include "Video.h"
#include "ui_Video.h"

#include<QButtonGroup>

Video::Video(QWidget *parent) :
    QWidget(parent), ui(new Ui::Video)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    initStackedWidget();
}

void Video::initStackedWidget() {
    initVideoChannelWidget();
    initMVWidget();
    initVideoWidget();
    //设置互斥
    m_buttonGroup->addButton(ui->video_channel_pushButton);
    m_buttonGroup->addButton(ui->MV_pushButton);
    m_buttonGroup->addButton(ui->video_pushButton);
    m_buttonGroup->setExclusive(true);

}

void Video::initVideoChannelWidget() {
    this->m_videoChannelWidget = std::make_unique<VideoChannelWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_videoChannelWidget.get());
}

void Video::initMVWidget() {
    this->m_MVWidget = std::make_unique<MVWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_MVWidget.get());
}

void Video::initVideoWidget() {
    this->m_videoWidget = std::make_unique<VideoWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_videoWidget.get());
}

Video::~Video() {
    delete ui;
}

void Video::on_video_channel_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_videoChannelWidget.get());
    qDebug()<<"点击videoChannelWidget";
}

void Video::on_MV_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_MVWidget.get());
    qDebug()<<"点击MVWidget";
}

void Video::on_video_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_videoWidget.get());
    qDebug()<<"点击videoWidget";
}
