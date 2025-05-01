//
// Created by WeiWang on 25-2-4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DownloadedWidget.h" resolved

#include "DownloadedWidget.h"
#include "ui_DownloadedWidget.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

DownloadedWidget::DownloadedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadedWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloaded.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
}

DownloadedWidget::~DownloadedWidget() {
    delete ui;
}

void DownloadedWidget::on_search_pushButton_clicked() {
    emit find_more_audio_book();
}
