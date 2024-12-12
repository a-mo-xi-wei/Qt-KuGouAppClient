//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoChannelWidget.h" resolved

#include "VideoChannelWidget.h"
#include "ui_VideoChannelWidget.h"

#include <QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

VideoChannelWidget::VideoChannelWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoChannelWidget) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channelwidget.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

VideoChannelWidget::~VideoChannelWidget() {
    delete ui;
}
