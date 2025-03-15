//
// Created by WeiWang on 25-2-4.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DownloadingWidget.h" resolved

#include "DownloadingWidget.h"
#include "ui_DownloadingWidget.h"
#include "logger.hpp"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

DownloadingWidget::DownloadingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadingWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloading.css"));
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

DownloadingWidget::~DownloadingWidget() {
    delete ui;
}

void DownloadingWidget::initUi() {
    ui->downloading_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->downloading_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/stop-gray.svg")));
    ui->downloading_clear_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg")));

}
