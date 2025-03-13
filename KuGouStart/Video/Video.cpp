//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Video.h" resolved

#include "Video.h"
#include "ui_Video.h"
#include "logger.hpp"

#include<QButtonGroup>
#include<QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Video::Video(QWidget *parent) :
    QWidget(parent), ui(new Ui::Video)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    qDebug() << "Logger instance address:" << mylog::Logger::get();
    mylog::Logger::get()->set_level(spdlog::level::info);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initStackedWidget();
    initUi();
}

Video::~Video() {
    delete ui;
}

void Video::initUi()const {
    ui->index_label1->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->index_label2->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->index_label3->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->index_label2->hide();
    ui->index_label3->hide();
}

void Video::initStackedWidget() {
    // 设置堆栈窗口的布局的边距
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
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
void Video::on_video_channel_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_videoChannelWidget.get());
    // 输出测试日志并立即刷新
    STREAM_INFO() << "切换 videoChannelWidget 界面";
    ui->index_label1->show();
    ui->index_label2->hide();
    ui->index_label3->hide();
}

void Video::on_MV_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_MVWidget.get());
    STREAM_INFO() << "切换 MVWidget 界面";
    ui->index_label1->hide();
    ui->index_label2->show();
    ui->index_label3->hide();
}

void Video::on_video_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_videoWidget.get());
    STREAM_INFO() << "切换 videoWidget 界面";
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->show();
}
