//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllCloudDisk.h" resolved

#include "AllCloudDisk.h"
#include "ui_AllCloudDisk.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AllCloudDisk::AllCloudDisk(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllCloudDisk) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/disk.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

AllCloudDisk::~AllCloudDisk() {
    delete ui;
}
