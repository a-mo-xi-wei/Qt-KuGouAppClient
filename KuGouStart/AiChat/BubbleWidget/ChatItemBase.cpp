﻿#include "ChatItemBase.h"
#include "BubbleFrame.h"

#include <QFont>
#include <QMovie>

ChatItemBase::ChatItemBase(const ChatRole role, QWidget *parent)
    : QWidget(parent)
    , m_role(role)
    , m_pBubble(std::make_unique<QWidget>(this))
{
    auto pGLayout = new QGridLayout(this);
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setContentsMargins(3,3,3,3);
    auto pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if (m_role == ChatRole::Time) {
        auto pSpacerLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        auto pSpacerRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        pGLayout->addItem(pSpacerLeft, 0, 0, 1, 1);
        pGLayout->addWidget(m_pBubble.get(), 0, 1, 1, 1, Qt::AlignCenter);
        pGLayout->addItem(pSpacerRight, 0, 2, 1, 1);
        return;// 提前返回，避免创建无用控件
    }
    m_pNameLabel    = new QLabel(this);
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);
    m_pIconLabel    = new QLabel(this);
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42, 42);
    if(m_role == ChatRole::Self)
    {
        m_pNameLabel->setContentsMargins(0,0,8,0);
        m_pNameLabel->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2,1, Qt::AlignTop);
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);
        pGLayout->addWidget(m_pBubble.get(), 1,1, 1,1);
        pGLayout->setColumnStretch(0, 2);
        pGLayout->setColumnStretch(1, 3);
    }
    else if (m_role == ChatRole::Other){
        initMovie();
        m_pNameLabel->setContentsMargins(8,0,0,0);
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(m_pIconLabel, 0, 0, 2,1, Qt::AlignTop);
        // pGLayout->addWidget(m_pNameLabel, 0,1, 1,1);
        // pGLayout->addWidget(m_loading, 0,2, 1,1);
        // 新建一个水平布局，把 m_pNameLabel 和 m_loading 放在一起
        QHBoxLayout* nameLayout = new QHBoxLayout;
        nameLayout->setContentsMargins(0, 0, 0, 0); // 去除边距
        nameLayout->setSpacing(5); // 控件之间的间距，可以调小，比如 2
        nameLayout->addWidget(m_pNameLabel);
        nameLayout->addWidget(m_loading);
        nameLayout->addStretch();

        // 用 QWidget 包裹这个布局
        QWidget* nameWidget = new QWidget;
        nameWidget->setLayout(nameLayout);

        // 加入到网格布局中
        pGLayout->addWidget(nameWidget, 0, 1, 1, 2); // 横跨原来的 1,1 和 0,2

        pGLayout->addWidget(m_pBubble.get(), 1,1, 1,1);
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);
        pGLayout->setColumnStretch(1, 3);
        pGLayout->setColumnStretch(2, 2);
    }
}

void ChatItemBase::setUserName(const QString &name) const {
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon) const {
    m_pIconLabel->setPixmap(icon);
}

void ChatItemBase::setWidget(QWidget *w) {
    if (!w || !layout()) return;

    auto pGLayout = qobject_cast<QGridLayout*>(layout());
    if (!pGLayout) return;

    // 1. 移除旧控件
    if (m_pBubble) {
        pGLayout->removeWidget(m_pBubble.get()); // 从布局移除
        m_pBubble.reset(); // 释放旧控件所有权（会自动调用 delete）
    }

    // 2. 接管新控件所有权
    m_pBubble.reset(w); // 转移所有权给 unique_ptr
    m_pBubble->setParent(this); // 设置父控件

    // 3. 根据角色添加
    if (m_role == ChatRole::Time) {
        pGLayout->addWidget(m_pBubble.get(), 0, 1, 1, 1, Qt::AlignCenter);
    } else {
        pGLayout->addWidget(m_pBubble.get(), 1, 1, 1, 1);
    }
}

void ChatItemBase::startMovie(const bool &flag) {
    if(flag) {
        m_loading->show();
        m_loadingMovie->start();
    }
    else {
        m_loading->hide();
        m_loadingMovie->stop();
    }
}

void ChatItemBase::initMovie() {
    //初始化加载动画
    m_loadingMovie = new QMovie(this);
    m_loadingMovie->setFileName(":/Res/window/loading.gif");
    m_loading = new QLabel(this);
    m_loading->setMovie(m_loadingMovie);
    m_loading->setFixedSize(16,16);
    m_loading->setAttribute(Qt::WA_TranslucentBackground , true);
    m_loading->setAutoFillBackground(false);
    m_loading->hide();
    m_loadingMovie->stop();
}
