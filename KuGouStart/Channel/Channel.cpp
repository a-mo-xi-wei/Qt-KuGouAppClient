//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Channel.h" resolved

#include "Channel.h"
#include "ui_Channel.h"

#include<QFile>
#include<QButtonGroup>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Channel::Channel(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Channel)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channel.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
        ui->title_label->setStyleSheet(QString("border:none;border-image: url(%1/music-channel.png);").arg(GET_CURRENT_DIR));
    }
    initButtonGroup();
}

Channel::~Channel() {
    delete ui;
}

void Channel::initButtonGroup() {
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->DJ_pushButton);
    this->m_buttonGroup->addButton(ui->language_pushButton);
    this->m_buttonGroup->addButton(ui->theme_pushButton);
    this->m_buttonGroup->addButton(ui->scene_pushButton);
    this->m_buttonGroup->addButton(ui->mood_pushButton);
    this->m_buttonGroup->addButton(ui->style_pushButton);
    this->m_buttonGroup->addButton(ui->crowd_pushButton);
    this->m_buttonGroup->addButton(ui->children_pushButton);
    this->m_buttonGroup->addButton(ui->musical_instrument_pushButton);
    this->m_buttonGroup->addButton(ui->label_pushButton);
    this->m_buttonGroup->addButton(ui->variety_pushButton);
    this->m_buttonGroup->addButton(ui->national_customs_pushButton);
    this->m_buttonGroup->addButton(ui->sports_pushButton);
    this->m_buttonGroup->setExclusive(true);
}
