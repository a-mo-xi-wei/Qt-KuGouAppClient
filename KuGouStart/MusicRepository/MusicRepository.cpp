//
// Created by WeiWang on 24-11-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepository.h" resolved

#include "MusicRepository.h"
#include "ui_MusicRepository.h"

#include <QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


MusicRepository::MusicRepository(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusicRepository) {
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/musicrepo.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }

    initUi();
}

MusicRepository::~MusicRepository() {
    delete ui;
}

void MusicRepository::initUi() {
    //设置鼠标指向样式
    ui->ranking_list_widget->setCursor(Qt::PointingHandCursor);
    ui->singer_widget->setCursor(Qt::PointingHandCursor);
    ui->classify_widget->setCursor(Qt::PointingHandCursor);
    auto lay = dynamic_cast<QVBoxLayout*>(ui->table_widget->layout());
    auto block = new MyBlockWidget(":/BlockCover/Res/blockcover/music-block-cover1.jpg",this);
    block->setPopularDirection(1);
    //为了确保 mask 变量正确引用返回值，需要显式声明为引用：
    auto& mask = block->getMask();
    mask.setDefaultFillCircleColor(Qt::white);
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF")));
    mask.setDefaultFillTriangleColor(QColor());
    mask.setHoverFillTriangleColor(QColor());
    mask.setEnterWidgetChangeCursor(false);
    lay->insertWidget(lay->count()-1,block);
    lay->insertWidget(lay->count()-1,new MyBlockWidget(":/BlockCover/Res/blockcover/music-block-cover2.jpg",this));
    lay->insertWidget(lay->count()-1,new MyBlockWidget(":/BlockCover/Res/blockcover/music-block-cover3.jpg",this));
    lay->insertWidget(lay->count()-1,new MyBlockWidget(":/BlockCover/Res/blockcover/music-block-cover4.jpg",this));

}
