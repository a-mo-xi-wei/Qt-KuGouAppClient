//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingerWidget.h" resolved

#include "SingerWidget.h"
#include "ui_SingerWidget.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

SingerWidget::SingerWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SingerWidget) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/singer.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
}

SingerWidget::~SingerWidget() {
    delete ui;
}

void SingerWidget::on_search_pushButton_clicked() {
    emit find_more_music();
}
