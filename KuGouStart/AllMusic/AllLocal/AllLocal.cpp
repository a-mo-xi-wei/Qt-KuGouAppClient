//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllLocal.h" resolved

#include "AllLocal.h"
#include "ui_AllLocal.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AllLocal::AllLocal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllLocal)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/local.css"));
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

AllLocal::~AllLocal() {
    delete ui;
}

void AllLocal::initUi() {
    //此处需要获取本地音乐列表，待到后面再说。。。先把界面写完
    auto lay = ui->all_local_song_list_widget->layout();
    if (lay->count() == 0) ui->scrollArea->hide();
}

void AllLocal::on_search_pushButton_clicked() {
    emit find_more_music();
}
