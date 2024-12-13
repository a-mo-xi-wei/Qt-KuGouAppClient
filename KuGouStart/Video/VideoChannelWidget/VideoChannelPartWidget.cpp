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

void VideoChannelPartWidget::setTitleName(const QString &title) {
    this->m_titleLab->setText(" " + title);
}

void VideoChannelPartWidget::addBlockWidget(VideoChannelBlock *block) {
    auto lay = static_cast<MyFlowLayout*>(this->m_tabWidget->layout());
    lay->addWidget(block);
}

void VideoChannelPartWidget::initUi() {
    this->setContentsMargins(0,0,0,0);
    this->m_titleLab->setStyleSheet("font-weight: bold;font-size: 16px;color: black;");
    auto hlay = new QHBoxLayout;
    hlay->addWidget(this->m_titleLab);
    hlay->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Expanding,QSizePolicy::Preferred));
    auto vlay = new QVBoxLayout(this);
    vlay->addLayout(hlay);
    vlay->addWidget(this->m_tabWidget);
    auto lay = new MyFlowLayout(this->m_tabWidget,8, -1, 10);
    this->m_tabWidget->setLayout(lay);
}

