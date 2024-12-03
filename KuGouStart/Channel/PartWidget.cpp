//
// Created by WeiWang on 24-12-3.
//

#include "PartWidget.h"
#include "MyFlowLayout.h"

#include <QFile>
#include <QBoxLayout>
#include <QLabel>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PartWidget::PartWidget(QWidget *parent)
    :QWidget(parent)
    ,m_titleLab(new QLabel(this))
    , m_titleDeco(new QLabel(this))
    , m_tabWidget(new QWidget(this))
{
    initUi();
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/part.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

void PartWidget::setTitleName(const QString &title) {
    this->m_titleLab->setText(title);
}

void PartWidget::addBlockWidget(ChannelBlock* block) {
    auto lay = static_cast<MyFlowLayout*>(this->m_tabWidget->layout());
    lay->addWidget(block);
}

void PartWidget::initUi() {
    this->m_titleLab->setObjectName("titleLab");
    this->m_titleDeco->setObjectName("titleDeco");
    this->m_titleDeco->setText(" .MHz");
    auto hlay = new QHBoxLayout;
    hlay->addWidget(this->m_titleLab);
    hlay->addWidget(this->m_titleDeco);
    hlay->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Expanding,QSizePolicy::Preferred));
    auto vlay = new QVBoxLayout(this);
    vlay->addLayout(hlay);
    vlay->addWidget(this->m_tabWidget);
    auto lay = new MyFlowLayout;
    this->m_tabWidget->setLayout(lay);
}

