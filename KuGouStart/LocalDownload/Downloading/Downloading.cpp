//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Downloading.h" resolved

#include "Downloading.h"
#include "ui_Downloading.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

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
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

Downloading::~Downloading() {
    delete ui;
}

void Downloading::on_stop_toolButton_clicked() {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Info",
                            "暂无正在下载音乐",
                            1000,this->window());
}

void Downloading::on_clear_toolButton_clicked() {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Info",
                            "暂无正在下载音乐",
                            1000,this->window());
}

void Downloading::on_setting_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "下载设置 功能暂未实现 敬请期待", 1000,this->window());
}

void Downloading::initUi() {
    //设置toolTip
    // 设置 setting_toolButton 的 tooltip
    auto setting_toolButton_toolTip = new ElaToolTip(ui->setting_toolButton);
    setting_toolButton_toolTip->setToolTip(QStringLiteral("下载设置"));

    ui->start_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->stop_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/stop-gray.svg")));
    ui->clear_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg")));
}
