//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveCommonPartWidget.h" resolved

#include "LiveCommonPartWidget.h"
#include "ui_LiveCommonPartWidget.h"
#include "LiveBlockWidget/LiveBlockWidget.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

LiveCommonPartWidget::LiveCommonPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveCommonPartWidget)
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/common.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

LiveCommonPartWidget::~LiveCommonPartWidget() {
    delete ui;
}

void LiveCommonPartWidget::setLineTow() {
    //初始化line_widget_2
    const auto lay2 = new QHBoxLayout(ui->line_widget_2);
    lay2->setContentsMargins(0, 0, 0, 0);
    lay2->setSpacing(15);
    for (int i = 6; i < 12; ++i) {
        const auto w = new LiveBlockWidget(this);
        lay2->addWidget(w);
        w->show();
        if (i == 11)w->hide();
        this->m_blockArr[i] = w;
    }
}

void LiveCommonPartWidget::setTitleName(const QString &name) {
    ui->title_label->setText(name);
}

void LiveCommonPartWidget::initUi() {
    ui->left_label->setStyleSheet(QString("border-image: url('%1/liveRes/left.svg')").arg(GET_CURRENT_DIR));
    ui->right_label->setStyleSheet(QString("border-image: url('%1/liveRes/right.svg')").arg(GET_CURRENT_DIR));

    //初始化line_widget_1
    const auto lay1 = new QHBoxLayout(ui->line_widget_1);
    lay1->setSpacing(15);
    for (int i = 0; i < 6; ++i) {
        const auto w = new LiveBlockWidget(this);
        lay1->addWidget(w);
        w->show();
        if (i == 5)w->hide();
        this->m_blockArr[i] = w;
    }

}

void LiveCommonPartWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"当前宽度为："<<this->width();
    if (this->width() < 1180) {
        if (this->m_blockArr[5]) {
            this->m_blockArr[5]->hide();
        }
        if (this->m_blockArr[11]) {
            this->m_blockArr[11]->hide();
        }
    }
    else {
        if (this->m_blockArr[5]) {
            this->m_blockArr[5]->show();
        }
        if (this->m_blockArr[11]) {
            this->m_blockArr[11]->show();
        }
    }
}
