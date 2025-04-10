//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DownloadedVideo.h" resolved

#include "DownloadedVideo.h"
#include "ui_DownloadedVideo.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

DownloadedVideo::DownloadedVideo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadedVideo)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloadedvideo.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
}

DownloadedVideo::~DownloadedVideo() {
    delete ui;
}

void DownloadedVideo::on_search_pushButton_clicked() {
    emit find_more_music();
}
