//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Search.h" resolved

#include "Search.h"
#include "ui_Search.h"

#include<QFile>
#include<QButtonGroup>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Search::Search(QWidget *parent) :
    QWidget(parent), ui(new Ui::Search)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/search.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
}

Search::~Search() {
    delete ui;
}

void Search::initUi() {
    {
        //设置图标
        ui->toolButton1->setIcon(QIcon(QStringLiteral(":/Search/Res/search/phonePlay.png")));
        ui->toolButton2->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-live.png")));
        ui->toolButton3->setIcon(QIcon(QStringLiteral(":/Search/Res/search/wallpaper.png")));
        ui->toolButton4->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-pingbao.png")));
        ui->toolButton5->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundEffect.png")));
        ui->toolButton6->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundPlugin.png")));
        ui->toolButton7->setIcon(QIcon(QStringLiteral(":/Search/Res/search/ringMake.png")));
        ui->toolButton8->setIcon(QIcon(QStringLiteral(":/Search/Res/search/remoteControl.png")));
        ui->toolButton9->setIcon(QIcon(QStringLiteral(":/Search/Res/search/musicCircle.png")));
        ui->toolButton10->setIcon(QIcon(QStringLiteral(":/Search/Res/search/cd.png")));
        ui->toolButton11->setIcon(QIcon(QStringLiteral(":/Search/Res/search/equalizer.png")));
        ui->toolButton12->setIcon(QIcon(QStringLiteral(":/Search/Res/search/timing.png")));
        ui->toolButton13->setIcon(QIcon(QStringLiteral(":/Search/Res/search/DLNA.png")));
        ui->toolButton14->setIcon(QIcon(QStringLiteral(":/Search/Res/search/change.png")));
        ui->toolButton15->setIcon(QIcon(QStringLiteral(":/Search/Res/search/netTest.png")));
        ui->toolButton16->setIcon(QIcon(QStringLiteral(":/Search/Res/search/earnCoin.png")));
    }
    {
        //设置互斥
        this->m_buttonGroup->addButton(ui->recommend_pushButton);
        this->m_buttonGroup->addButton(ui->rank_pushButton);
        this->m_buttonGroup->addButton(ui->special_pushButton);
        this->m_buttonGroup->addButton(ui->channel_pushButton);
        this->m_buttonGroup->setExclusive(true);
    }
    {
        //下标图片
        ui->index_label1->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label2->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label3->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label4->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label2->hide();
        ui->index_label3->hide();
        ui->index_label4->hide();
    }
}

void Search::on_recommend_pushButton_clicked() {
    ui->index_label1->show();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();
}

void Search::on_rank_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->show();
    ui->index_label3->hide();
    ui->index_label4->hide();
}

void Search::on_special_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->show();
    ui->index_label4->hide();
}

void Search::on_channel_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->show();
}
