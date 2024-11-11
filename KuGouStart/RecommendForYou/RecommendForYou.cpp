//
// Created by WeiWang on 24-10-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecommendForYou.h" resolved

#include "RecommendForYou.h"
#include "ui_RecommendForYou.h"
#include "TableWidget.h"

#include<QDir>
#include <QResizeEvent>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecommendForYou::RecommendForYou(QWidget *parent) :
    QWidget(parent), ui(new Ui::RecommendForYou) {
    ui->setupUi(this);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css"));
    //qDebug()<<(GET_CURRENT_DIR + "/recommend.css").data();
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }

    initAdvertiseBoard();
    initClassifyWidget();
    initTabWidget();
}

RecommendForYou::~RecommendForYou() {
    delete ui;
}

void RecommendForYou::initAdvertiseBoard()
{
    QDir dir(__FILE__);
    if (dir.cdUp())dir.cd("Res/poster");

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    for (auto i = 1; i <= s; ++i)
        ui->advertise_board_widget->addPoster(QPixmap(QString(":/Res/poster/%1.jpg").arg(i)));
}

void RecommendForYou::initClassifyWidget() {
    ui->recommend_toolButton->setIcon(QIcon(":/Res/tabIcon/rili.svg"));
    ui->recommend_toolButton->setEnterIconSize(QSize(35,35));
    ui->recommend_toolButton->setLeaveIconSize(QSize(30,30));
    ui->ranking_list_toolButton->setIcon(QIcon(":/Res/tabIcon/rank.svg"));
    ui->ranking_list_toolButton->setEnterIconSize(QSize(40,40));
    ui->ranking_list_toolButton->setLeaveIconSize(QSize(35,35));
    ui->classify_toolButton->setIcon(QIcon(":/Res/tabIcon/classification.svg"));
    ui->classify_toolButton->setEnterIconSize(QSize(40,40));
    ui->classify_toolButton->setLeaveIconSize(QSize(35,35));
    ui->scene_music_toolButton->setIcon(QIcon(":/Res/tabIcon/shafa.svg"));
    ui->scene_music_toolButton->setEnterIconSize(QSize(45,45));
    ui->scene_music_toolButton->setLeaveIconSize(QSize(40,40));
    ui->scene_music_toolButton->setEnterFontSize(13);
    ui->music_quality_toolButton->setIcon(QIcon(":/Res/tabIcon/dish.svg"));
    ui->music_quality_toolButton->setEnterIconSize(QSize(40,40));
    ui->music_quality_toolButton->setLeaveIconSize(QSize(35,35));
    //ui->recommend_toolButton->setIcon(QIcon(":/Res/tabIcon/rili.svg"));
}

void RecommendForYou::initTabWidget() {
    const auto layout = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
    if (!layout)return;
    layout->insertWidget(layout->count() - 1, new TableWidget(QStringLiteral("今日专属推荐"), TableWidget::KIND::BlockList,this));
    layout->insertWidget(layout->count() - 1, new TableWidget(QStringLiteral("潮流音乐站 "), TableWidget::KIND::ItemList,this));
    layout->insertWidget(layout->count() - 1, new TableWidget(QStringLiteral("热门好歌精选 "),TableWidget::KIND::ItemList, this));
    layout->insertWidget(layout->count() - 1, new TableWidget(QStringLiteral("私人专属好歌 "),TableWidget::KIND::ItemList, this));
}
