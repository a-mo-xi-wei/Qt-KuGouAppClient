//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyMVChannel.h" resolved

#include "RecentlyMVChannel.h"
#include "ui_RecentlyMVChannel.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecentlyMVChannel::RecentlyMVChannel(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecentlyMVChannel) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/mv.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }

}

RecentlyMVChannel::~RecentlyMVChannel() {
    delete ui;
}
