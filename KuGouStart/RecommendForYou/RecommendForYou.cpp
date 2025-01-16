//
// Created by WeiWang on 24-10-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecommendForYou.h" resolved

#include "RecommendForYou.h"
#include "ui_RecommendForYou.h"
#include "TableWidget.h"

#include <QDir>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScrollBar>
#include <QTimer>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecommendForYou::RecommendForYou(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecommendForYou)
    , m_parent(this->window())
{
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

void RecommendForYou::initAdvertiseBoard() const {
    QDir dir(__FILE__);
    dir.cdUp();
    dir.cdUp();
    //qDebug()<<"当前目录："<<dir.dirName();
    dir.cd("Res/recommend/poster");
    //qDebug()<<"当前目录："<<dir.dirName();

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    //qDebug()<<"共有: "<<s<<" 条数据";
    for (auto i = 1; i <= s; ++i) {
        //QString path = QString(":/RecommendForYou/Res/recommend/poster/%1.jpg").arg(i);
        //qDebug()<<"图片路径为："<<path;
        //const QPixmap pix(path);
        //if(pix.isNull())qDebug()<<"图像错误";
        ui->advertise_board_widget->addPoster(QPixmap(QString(":/RecommendForYou/Res/recommend/poster/%1.jpg").arg(i)));
    }
}

void RecommendForYou::initClassifyWidget() const {
    //QIcon ico = QIcon(":/RecommendForYou/recommend/tabIcon/rili.svg");
    //if(ico.isNull())qDebug()<<"++++++++++ico 为空++++++++++++++";
    //QFile file(":/RecommendForYou/recommend/tabIcon/rili.svg");
    //qDebug() << "File Exists:" << file.exists();

    //设置可变大小
    ui->recommend_toolButton->setChangeSize(true);
    ui->ranking_list_toolButton->setChangeSize(true);
    ui->classify_toolButton->setChangeSize(true);
    ui->scene_music_toolButton->setChangeSize(true);
    ui->music_quality_toolButton->setChangeSize(true);

    ui->recommend_toolButton->setIcon(QIcon(":/RecommendForYou/Res/recommend/tabIcon/rili.svg"));
    ui->recommend_toolButton->setEnterIconSize(QSize(35, 35));
    ui->recommend_toolButton->setLeaveIconSize(QSize(30, 30));

    ui->ranking_list_toolButton->setIcon(QIcon(":/RecommendForYou/Res/recommend/tabIcon/rank.svg"));
    ui->ranking_list_toolButton->setEnterIconSize(QSize(40, 40));
    ui->ranking_list_toolButton->setLeaveIconSize(QSize(35, 35));

    ui->classify_toolButton->setIcon(QIcon(":/RecommendForYou/Res/recommend/tabIcon/classification.svg"));
    ui->classify_toolButton->setEnterIconSize(QSize(40, 40));
    ui->classify_toolButton->setLeaveIconSize(QSize(35, 35));

    ui->scene_music_toolButton->setIcon(QIcon(":/RecommendForYou/Res/recommend/tabIcon/shafa.svg"));
    ui->scene_music_toolButton->setEnterIconSize(QSize(45, 45));
    ui->scene_music_toolButton->setLeaveIconSize(QSize(40, 40));
    ui->scene_music_toolButton->setEnterFontSize(13);

    ui->music_quality_toolButton->setIcon(QIcon(":/RecommendForYou/Res/recommend/tabIcon/dish.svg"));
    ui->music_quality_toolButton->setEnterIconSize(QSize(40, 40));
    ui->music_quality_toolButton->setLeaveIconSize(QSize(35, 35));
}

void RecommendForYou::initTabWidget() {
    const auto layout = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
    if (!layout)return;
    layout->insertWidget(layout->count() - 1,
                         new TableWidget(QStringLiteral("今日专属推荐"), TableWidget::KIND::BlockList, this));
    layout->insertWidget(layout->count() - 1,
                         new TableWidget(QStringLiteral("潮流音乐站 "), TableWidget::KIND::ItemList, this));
    layout->insertWidget(layout->count() - 1,
                         new TableWidget(QStringLiteral("热门好歌精选 "), TableWidget::KIND::ItemList, this));
    layout->insertWidget(layout->count() - 1,
                         new TableWidget(QStringLiteral("私人专属好歌 "), TableWidget::KIND::ItemList, this));
}

void RecommendForYou::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->m_parent->height() - 180);
}