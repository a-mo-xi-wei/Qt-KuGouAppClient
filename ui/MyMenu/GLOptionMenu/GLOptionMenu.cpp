//
// Created by WeiWang on 25-2-24.
//

#include "GLOptionMenu.h"

#include <QCoreApplication>
#include <QWidgetAction>

GLOptionMenu::GLOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
}

const GLOptionMenu * GLOptionMenu::getMenu() const {
    return this;
}

void GLOptionMenu::initMenu() {
    this->setMouseTracking(true);
    this->setFixedSize(60,90);
    auto a_foundAction = new QWidgetAction(this);
    auto a_foundBtn = new QToolButton(this);
    a_foundBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    a_foundBtn->setMouseTracking(true);
    a_foundBtn->setCursor(Qt::PointingHandCursor);
    a_foundBtn->setFixedSize(32, 20);
    a_foundBtn->setText(QStringLiteral("发现"));
    a_foundBtn->setStyleSheet("QToolButton{color:black;font-size:11px;background-color:rgba(255,255,255,0);}QToolButton:hover{color:#268cff;}");
    a_foundAction->setDefaultWidget(a_foundBtn);
    connect(a_foundBtn, &QToolButton::clicked, this, [this,a_foundBtn] {
            emit getModel(a_foundBtn->text());
    });
    connect(a_foundAction,&QWidgetAction::hovered,this,[this,a_foundBtn] {
        checkHover();
        this->m_currentHover.emplace_back(a_foundBtn);
        this->m_lastHover = this->m_currentHover;
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(a_foundBtn, &enterEvent); // 发送事件
        // 模拟按钮进入 hover 状态
        a_foundBtn->setAttribute(Qt::WA_UnderMouse, true);
    });

    auto a_minorityAction = new QWidgetAction(this);
    auto a_minorityBtn = new QToolButton(this);
    a_minorityBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    a_minorityBtn->setMouseTracking(true);
    a_minorityBtn->setCursor(Qt::PointingHandCursor);
    a_minorityBtn->setFixedSize(32, 20);
    a_minorityBtn->setText(QStringLiteral("小众"));
    a_minorityBtn->setStyleSheet("QToolButton{color:black;font-size:11px;background-color:rgba(255,255,255,0);}QToolButton:hover{color:#268cff;}");
    a_minorityAction->setDefaultWidget(a_minorityBtn);
    connect(a_minorityBtn, &QToolButton::clicked, this, [this,a_minorityBtn] {
            emit getModel(a_minorityBtn->text());
    });
    connect(a_minorityAction,&QWidgetAction::hovered,this,[this,a_minorityBtn] {
        checkHover();
        this->m_currentHover.emplace_back(a_minorityBtn);
        this->m_lastHover = this->m_currentHover;
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(a_minorityBtn, &enterEvent); // 发送事件
        // 模拟按钮进入 hover 状态
        a_minorityBtn->setAttribute(Qt::WA_UnderMouse, true);
    });

    auto a_30sAction = new QWidgetAction(this);
    auto a_30sBtn = new QToolButton(this);
    a_30sBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    a_30sBtn->setMouseTracking(true);
    a_30sBtn->setCursor(Qt::PointingHandCursor);
    a_30sBtn->setFixedSize(32, 20);
    a_30sBtn->setText(QStringLiteral("30s"));
    a_30sBtn->setStyleSheet("QToolButton{color:black;font-size:11px;background-color:rgba(255,255,255,0);}QToolButton:hover{color:#268cff;}");
    a_30sAction->setDefaultWidget(a_30sBtn);
    connect(a_30sBtn, &QToolButton::clicked, this, [this,a_30sBtn] {
        emit getModel(a_30sBtn->text());
    });
    connect(a_30sAction,&QWidgetAction::hovered,this,[this,a_30sBtn] {
        checkHover();
        this->m_currentHover.emplace_back(a_30sBtn);
        this->m_lastHover = this->m_currentHover;
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(a_30sBtn, &enterEvent); // 发送事件
        // 模拟按钮进入 hover 状态
        a_30sBtn->setAttribute(Qt::WA_UnderMouse, true);
    });


    this->addAction(a_foundAction);
    this->addAction(a_minorityAction);
    this->addAction(a_30sAction);
    this->hide();
}
