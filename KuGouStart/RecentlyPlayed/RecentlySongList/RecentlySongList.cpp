//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySongList.h" resolved

#include "RecentlySongList.h"
#include "ui_RecentlySongList.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecentlySongList::RecentlySongList(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlySongList) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/list.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }

}

RecentlySongList::~RecentlySongList() {
    delete ui;
}

void RecentlySongList::on_search_pushButton_clicked() {
    emit find_more_music();
}
