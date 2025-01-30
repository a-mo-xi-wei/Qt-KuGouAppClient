//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PaidSingle.h" resolved

#include "PaidSingle.h"
#include "ui_PaidSingle.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PaidSingle::PaidSingle(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaidSingle)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/paid.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

PaidSingle::~PaidSingle() {
    delete ui;
}

void PaidSingle::initUi() {
    ui->single_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->single_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));

}
