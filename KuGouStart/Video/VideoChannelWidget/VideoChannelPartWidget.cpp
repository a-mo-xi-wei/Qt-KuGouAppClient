//
// Created by WeiWang on 24-12-12.
//

#include "VideoChannelPartWidget.h"
#include "MyFlowLayout.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>

VideoChannelPartWidget::VideoChannelPartWidget(QWidget *parent)
    :QWidget(parent)
    , m_titleLab(new QLabel(this))
    , m_tabWidget(new QWidget(this))
{
    initUi();
}

void VideoChannelPartWidget::setTitleName(const QString &title)const {
    this->m_titleLab->setText(" " + title);
}

void VideoChannelPartWidget::addBlockWidget(VideoChannelBlock *block)const {
    auto lay = static_cast<MyFlowLayout*>(this->m_tabWidget->layout());
    lay->addWidget(block);
}

void VideoChannelPartWidget::initUi() {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setContentsMargins(0,0,0,0);
    this->m_titleLab->setStyleSheet("font-weight: bold;font-size: 16px;color: black;");
    auto hlay = new QHBoxLayout;
    hlay->addWidget(this->m_titleLab);
    hlay->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Expanding,QSizePolicy::Preferred));
    auto vlay = new QVBoxLayout(this);
    vlay->addLayout(hlay);
    vlay->addWidget(this->m_tabWidget);
    //auto lay = new MyFlowLayout(this->m_tabWidget,15,-1,-1);
    auto lay = new MyFlowLayout(this->m_tabWidget,true,5);//这个有间隙，符合要求
    lay->setContentsMargins(0,0,0,0);
    this->m_tabWidget->setLayout(lay);
    this->m_tabWidget->setContentsMargins(0,0,0,0);
    //this->m_tabWidget->setStyleSheet("background-color:pink;");
}

void VideoChannelPartWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

}

