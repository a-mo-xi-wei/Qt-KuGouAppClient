//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecentlyPlay.h" resolved

#include "ListenRecentlyPlay.h"
#include "ui_ListenRecentlyPlay.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenRecentlyPlay::ListenRecentlyPlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecentlyPlay)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/play.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
}

ListenRecentlyPlay::~ListenRecentlyPlay() {
    delete ui;
}
