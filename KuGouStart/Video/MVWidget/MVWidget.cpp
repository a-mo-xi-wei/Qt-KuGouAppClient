//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVWidget.h" resolved

#include "MVWidget.h"
#include "ui_MVWidget.h"

#include <QFile>


// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MVWidget::MVWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MVWidget) {
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
    initUi();
}

MVWidget::~MVWidget() {
    delete ui;
}

void MVWidget::initUi() {
    //滚动条
    ui->scrollArea->setScrollAreaKind(MyScrollArea::ScrollAreaKind::Inside);
    //广告
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/1.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/2.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/3.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/4.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/5.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/6.png")));
    ui->advertise_widget->setCurrentIndex(0);
    ui->advertise_widget->adjustSize();
    ui->advertise_widget->setAutoSlide(4000);
    //搜索框
    auto action = new QAction(this);
    action->setIcon(QIcon(QStringLiteral(":/Res/titlebar/search-black.svg")));
    action->setIconVisibleInMenu(false);
    ui->search_lineEdit->addAction(action, QLineEdit::TrailingPosition);
    //隐藏后四个按钮
    ui->pushButton5->hide();
    ui->pushButton6->hide();
    ui->pushButton7->hide();
    ui->pushButton8->hide();
    ui->pushButton5->setFixedSize(105,30);
    ui->pushButton6->setFixedSize(105,30);
    ui->pushButton7->setFixedSize(105,30);
    ui->pushButton8->setFixedSize(105,30);
}

void MVWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"width = "<<this->width();
    auto w = this->width();
    static int currentState = -1;  // 记录当前的按钮状态

    int newState = 0;  // 状态变量来表示当前应该显示哪些按钮

    if (w <= 890) {
        newState = 0;  // 只有前 1 个按钮可见
    } else if (w <= 950) {
        newState = 1;  // 前 2 个按钮可见
    } else if (w <= 1060) {
        newState = 2;  // 前 3 个按钮可见
    } else if (w <= 1120) {
        newState = 3;  // 前 4 个按钮可见
    } else {
        newState = 4;  // 所有按钮可见
    }

    // 仅当按钮状态变化时才更新显示
    if (currentState != newState) {
        currentState = newState;
        ui->pushButton5->setVisible(newState >= 1);
        ui->pushButton6->setVisible(newState >= 2);
        ui->pushButton7->setVisible(newState >= 3);
        ui->pushButton8->setVisible(newState >= 4);
    }
}
