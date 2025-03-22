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
    auto styleSheet = this->getStyleSheet();
    styleSheet.append(R"(
        QMenu{
            border-radius: 5px;
            background-color: #FFFFFF;
            margin: 2px;
            padding: 10px 5px 10px 5px;
        }
    )");
    this->setStyleSheet(styleSheet);
    this->TrayIconMenu::setShadow(2,5,QColor(150,150,150,60));
}

void TrayIconMenu::initMenu() {
    //qDebug() << "Entering initTrayIconMenu";
    this->setFixedSize(144, 200);
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
    //打开/关闭声音按钮
    auto a_openCloseAction = new QWidgetAction(this);
    {
        auto a_openCloseToolBtn = new MenuBtn(this);
        a_openCloseToolBtn->setFixedSize(130, 28);
        a_openCloseToolBtn->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")));
        a_openCloseToolBtn->initIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")),
                                QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
        a_openCloseToolBtn->setText(QStringLiteral("  打开/关闭声音"));
        a_openCloseAction->setDefaultWidget(a_openCloseToolBtn);
        connect(a_openCloseToolBtn, &QToolButton::clicked, this, [this,a_openCloseToolBtn] {
            this->m_volumeFlag = !this->m_volumeFlag;
            emit noVolume(this->m_volumeFlag);
            if (this->m_volumeFlag) {
                a_openCloseToolBtn->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-blue.svg")));
                a_openCloseToolBtn->initIcon(QIcon(QStringLiteral(":/Res/playbar/volume-off-gray.svg")),
                                             QIcon(QStringLiteral(":/Res/playbar/volume-off-blue.svg")));
            }
            else {
                a_openCloseToolBtn->setIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
                a_openCloseToolBtn->initIcon(QIcon(QStringLiteral(":/Res/playbar/volume-on-gray.svg")),
                                             QIcon(QStringLiteral(":/Res/playbar/volume-on-blue.svg")));
            }
        });
        connect(a_openCloseAction, &QWidgetAction::hovered, this, [a_openCloseToolBtn,this] {
            //qDebug()<<"进入a_openCloseToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_openCloseToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_openCloseToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_openCloseToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //关于我的酷狗按钮
    auto a_aboutKuGouAction = new QWidgetAction(this);
    {
        auto a_aboutKuGouToolBtn = new MenuBtn(this);
        a_aboutKuGouToolBtn->setFixedSize(130, 28);
        a_aboutKuGouToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-black.svg")));
        a_aboutKuGouToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-black.svg")),
                                QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-blue.svg")));
        a_aboutKuGouToolBtn->setText(QStringLiteral("  关于我的酷狗"));
        a_aboutKuGouAction->setDefaultWidget(a_aboutKuGouToolBtn);
        connect(a_aboutKuGouToolBtn, &QToolButton::clicked, this, [this,a_aboutKuGouToolBtn] {
            this->m_aboutFlag = !this->m_aboutFlag;
            emit showAboutDialog(this->m_aboutFlag);
            a_aboutKuGouToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-blue.svg")));
            a_aboutKuGouToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-black.svg")),
                                QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-blue.svg")));
        });
        connect(a_aboutKuGouAction, &QWidgetAction::hovered, this, [a_aboutKuGouToolBtn,this] {
            //qDebug()<<"进入a_aboutKuGouToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_aboutKuGouToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_aboutKuGouToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_aboutKuGouToolBtn->setAttribute(Qt::WA_UnderMouse, true);
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
    this->addAction(a_openCloseAction);
    this->addAction(a_aboutKuGouAction);
    this->addAction(a_exitPlayAction);
    //qDebug() << "Exiting initTrayIconMenu";
    this->hide();
}

const TrayIconMenu* TrayIconMenu::getMenu() const {
    return this;
}

void TrayIconMenu::setShadow(const int &width, const int &radius, const QColor &color) {
    //BaseMenu::setShadow(width, radius, color); //无需继承
    this->m_shadowWidth = width;
    this->m_shadowRadius = radius;
    this->m_shadowColor = color;
}
