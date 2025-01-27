//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Downloading.h" resolved

#include "Downloading.h"
#include "ui_Downloading.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Downloading::Downloading(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Downloading)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloading.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

Downloading::~Downloading() {
    delete ui;
}

void Downloading::initUi() {
    ui->start_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->stop_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/stop-gray.svg")));
    ui->clear_toolButton->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/delete-black.svg")));
}
