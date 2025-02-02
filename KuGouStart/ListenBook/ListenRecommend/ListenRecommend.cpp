//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecommend.h" resolved

#include "ListenRecommend.h"
#include "ui_ListenRecommend.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenRecommend::ListenRecommend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecommend)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

ListenRecommend::~ListenRecommend() {
    delete ui;
}

void ListenRecommend::initUi() {
    //设置全部分类按钮
    ui->all_classify_toolButton->setHoverFontColor(QColor(QStringLiteral("#26A1FF")));
    ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setIconSize(QSize(10, 10));
    ui->all_classify_toolButton->setApproach(true);

    //设置换一批按钮
    ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
    ui->refresh_toolButton->setText(" 换一批");
    ui->refresh_toolButton->installEventFilter(this);

}

bool ListenRecommend::eventFilter(QObject *watched, QEvent *event) {
    // 检查是否是 refresh_toolButton 的事件
    if (watched == ui->refresh_toolButton) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 refresh_toolButton
            //qDebug() << "Mouse entered refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-blue.svg")));
            return true; // 如果希望阻止默认行为，可以返回 true
        }
        if (event->type() == QEvent::Leave) {
            // 鼠标离开 refresh_toolButton
            //qDebug() << "Mouse left refresh_toolButton";
            ui->refresh_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg")));
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}
