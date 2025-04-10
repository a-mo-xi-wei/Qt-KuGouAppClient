//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllWidget.h" resolved

#include "AllWidget.h"
#include "logger.hpp"
#include "ui_AllWidget.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AllWidget::AllWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/widget.css"));
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

AllWidget::~AllWidget() {
    delete ui;
}

void AllWidget::on_search_pushButton_clicked() {
    emit find_more_music();
}

void AllWidget::initUi() {
    //此处需要获取本地音乐列表，待到后面再说。。。先把界面写完
    auto lay = ui->all_widget_song_list_widget->layout();
    if (lay->count() == 0) ui->scrollArea->hide();
}
