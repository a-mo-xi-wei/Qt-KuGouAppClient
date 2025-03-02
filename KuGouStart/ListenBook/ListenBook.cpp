//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenBook.h" resolved

#include "ListenBook.h"
#include "ui_ListenBook.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenBook::ListenBook(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::ListenBook)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/listen.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

ListenBook::~ListenBook() {
    delete ui;
}

void ListenBook::initUi() {
    //初始化堆栈窗口
    initStackedWidget();
    //设置图标
    {
        ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg")));
        ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg")));
        ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg")));

        ui->listen_recommend_toolButton->setIconSize(QSize(17, 17));
        ui->listen_my_download_toolButton->setIconSize(QSize(21, 21));
        ui->recently_play_toolButton->setIconSize(QSize(19, 19));
    }
    //设置文本
    ui->listen_recommend_toolButton->setText(QStringLiteral("   推荐"));
    ui->listen_my_download_toolButton->setText(QStringLiteral("  下载"));
    ui->recently_play_toolButton->setText(QStringLiteral("   最近"));

    //设置选中图标
    {
        connect(ui->listen_recommend_toolButton, &QToolButton::toggled, ui->listen_recommend_toolButton, [=](bool checked) {
            if (checked) {
                ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg")));
            } else {
                ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-gray.svg")));
            }
        });
        connect(ui->listen_my_download_toolButton, &QToolButton::toggled, ui->listen_my_download_toolButton, [=](bool checked) {
            if (checked) {
                ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-black.svg")));
            } else {
                ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg")));
            }
        });
        connect(ui->recently_play_toolButton, &QToolButton::toggled, ui->recently_play_toolButton, [=](bool checked) {
            if (checked) {
                ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-black.svg")));
            } else {
                ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg")));
            }
        });
    }

    //显示隐藏按钮
    {
        ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                    background-color:rgba(255,255,255,0);
                                                    border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                }
                                                 QToolButton:hover{
                                                    border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                })");

        connect(ui->indicator_toolButton, &QToolButton::toggled, ui->indicator_toolButton, [=](bool checked) {
            if (checked) {
                ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                            background-color:rgba(255,255,255,0);
                                                            border-image:url(:/ListenBook/Res/listenbook/down-black.svg);
                                                        }
                                                        QToolButton:hover{
                                                            border-image:url(:/ListenBook/Res/listenbook/down-blue.svg);
                                                        })");
            } else {
                ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                            background-color:rgba(255,255,255,0);
                                                            border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                        }
                                                        QToolButton:hover{
                                                            border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                        })");
            }
        });
    }

}

void ListenBook::initStackedWidget() {
    initListenRecommend();
    initListenMyDownload();
    initListenRecentlyPlay();
    //设置互斥
    this->m_buttonGroup->addButton(ui->listen_recommend_toolButton);
    this->m_buttonGroup->addButton(ui->listen_my_download_toolButton);
    this->m_buttonGroup->addButton(ui->recently_play_toolButton);
    this->m_buttonGroup->setExclusive(true);
}

void ListenBook::initListenRecommend() {
    this->m_listenRecommend = std::make_unique<ListenRecommend>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_listenRecommend.get());
    ui->stackedWidget->setCurrentWidget(this->m_listenRecommend.get());
}

void ListenBook::initListenMyDownload() {
    this->m_listenMyDownload = std::make_unique<ListenMyDownload>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_listenMyDownload.get());
}

void ListenBook::initListenRecentlyPlay() {
    this->m_listenRecentlyPlay = std::make_unique<ListenRecentlyPlay>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_listenRecentlyPlay.get());
}

void ListenBook::on_listen_recommend_toolButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_listenRecommend.get())
        return;
    ui->stackedWidget->setCurrentWidget(this->m_listenRecommend.get());
}

void ListenBook::on_listen_my_download_toolButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_listenMyDownload.get())
        return;
    ui->stackedWidget->setCurrentWidget(this->m_listenMyDownload.get());
}

void ListenBook::on_recently_play_toolButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_listenRecentlyPlay.get())
        return;
    ui->stackedWidget->setCurrentWidget(this->m_listenRecentlyPlay.get());
}
