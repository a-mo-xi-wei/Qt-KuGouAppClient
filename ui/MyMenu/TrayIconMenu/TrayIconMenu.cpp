//
// Created by WeiWang on 25-3-15.
//

#include "TrayIconMenu.h"
#include "logger.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QWidgetAction>

TrayIconMenu::TrayIconMenu(QWidget* parent)
    :BaseMenu(parent)
{
}

void TrayIconMenu::initMenu() {
    //qDebug() << "Entering initTrayIconMenu";
    this->setFixedSize(150, 200);
    //打开窗口按钮
    auto a_openWindowAction = new QWidgetAction(this);
    {
        auto a_openWindowToolBtn = new MenuBtn(this);
        a_openWindowToolBtn->setFixedSize(130, 28);
        a_openWindowToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openWindow-black.svg")));
        a_openWindowToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openWindow-black.svg")),
                                QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openWindow-blue.svg")));
        a_openWindowToolBtn->setText(QStringLiteral("  打开我的酷狗"));
        a_openWindowAction->setDefaultWidget(a_openWindowToolBtn);
        connect(a_openWindowToolBtn, &QToolButton::clicked, this, [this] {
            emit openWindow();
        });
        connect(a_openWindowAction, &QWidgetAction::hovered, this, [a_openWindowToolBtn,this] {
            //qDebug()<<"进入a_openWindowToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_openWindowToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_openWindowToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_openWindowToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //退出按钮
    auto a_exitPlayAction = new QWidgetAction(this);
    {
        auto a_exitPlayToolBtn = new MenuBtn(this);
        a_exitPlayToolBtn->setFixedSize(130, 28);
        a_exitPlayToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-black.svg")));
        a_exitPlayToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-blue.svg")));
        a_exitPlayToolBtn->setText(QStringLiteral("  退出我的酷狗"));
        a_exitPlayAction->setDefaultWidget(a_exitPlayToolBtn);
        connect(a_exitPlayToolBtn, &QToolButton::clicked, this, [this] {
            emit exit();
        });
        connect(a_exitPlayAction, &QWidgetAction::hovered, this, [a_exitPlayToolBtn,this] {
            //qDebug()<<"进入a_exitPlayToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_exitPlayToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_exitPlayToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_exitPlayToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }

    this->addAction(a_openWindowAction);
    this->addAction(a_exitPlayAction);
    //qDebug() << "Exiting initTrayIconMenu";
    this->hide();
}

const TrayIconMenu* TrayIconMenu::getMenu() const {
    return this;
}
