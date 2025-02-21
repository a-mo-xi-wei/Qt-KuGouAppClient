//
// Created by WeiWang on 25-2-17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LiveCommonPartWidget.h" resolved

#include "LiveCommonPartWidget.h"
#include "ui_LiveCommonPartWidget.h"
#include "LiveBlockWidget/LiveBlockWidget.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>

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

void LiveCommonPartWidget::setLineTwo() {
    //初始化line_widget_2
    const auto lay2 = new QHBoxLayout(ui->line_widget_2);
    //lay2->setSpacing(15);
    lay2->setContentsMargins(0,0,0,0);
    for (int i = 6; i < 12; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_2);
        w->setLeftBottomText(this->m_leftBottomTextVec[i + 20]);
        lay2->addWidget(w);
        w->show();
        if (i == 11)w->hide();
        this->m_blockArr[i] = w;
    }
    ui->line_widget_2->setLayout(lay2);
}

void LiveCommonPartWidget::setTitleName(const QString &name) {
    ui->title_label->setText(name);
}

void LiveCommonPartWidget::initUi() {
    //初始化右上角两个按钮图标
    ui->left_label->setStyleSheet(QString("border-image: url('%1/liveRes/left.svg')").arg(GET_CURRENT_DIR));
    ui->right_label->setStyleSheet(QString("border-image: url('%1/liveRes/right.svg')").arg(GET_CURRENT_DIR));
    //初始化block左下角文字vec
    //解析json文件
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/liveRes/text.json"));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading text.json";
            return;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr) {
            QString title = item.toObject().value("text").toString();
            this->m_leftBottomTextVec.emplace_back(title);
        }
        file.close();
    }
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_leftBottomTextVec.begin(), this->m_leftBottomTextVec.end(), std::default_random_engine(seed));

    //初始化line_widget_1
    const auto lay1 = new QHBoxLayout(ui->line_widget_1);
    //lay1->setSpacing(15);
    lay1->setContentsMargins(0,0,0,0);
    for (int i = 0; i < 6; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_1);
        w->setLeftBottomText(this->m_leftBottomTextVec[i]);
        lay1->addWidget(w);
        w->show();
        if (i == 5)w->hide();
        this->m_blockArr[i] = w;
    }
    ui->line_widget_1->setLayout(lay1);
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
