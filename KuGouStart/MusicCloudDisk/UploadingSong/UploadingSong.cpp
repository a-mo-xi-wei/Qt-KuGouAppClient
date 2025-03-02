//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadingSong.h" resolved

#include "UploadingSong.h"
#include "ui_UploadingSong.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

UploadingSong::UploadingSong(QWidget *parent)
    : QWidget(parent)
, ui(new Ui::UploadingSong)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/uploading.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

UploadingSong::~UploadingSong() {
    delete ui;
}

void UploadingSong::initUi() {
    ui->cloud_upload_toolButton->setIcon(QIcon(QStringLiteral(":Res/menuIcon/upload-white.svg")));
    ui->cloud_start_toolButton->setIcon(QIcon(QStringLiteral(":Res/tabIcon/play3-gray.svg")));
    ui->cloud_pause_toolButton->setIcon(QIcon(QStringLiteral(":Res/tabIcon/stop-gray.svg")));
    ui->cloud_clear_toolButton->setIcon(QIcon(QStringLiteral(":Res/menuIcon/delete-gray.svg")));
}