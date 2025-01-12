//
// Created by WeiWang on 25-1-12.
//

#include "TitleOptionMenu.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QWidgetAction>

TitleOptionMenu::TitleOptionMenu(QWidget *parent)
    :BaseMenu(parent)
{
}

void TitleOptionMenu::initMenu() {
    this->setFixedSize(380, 600);
    //顶部按钮
    //auto a_topListWidgetAction = new QWidgetAction(this);
    auto a_topListWidgetAction = new QWidgetAction(this); {
        //动态壁纸按钮
        auto a_dynamicWallPaperBtn = new MenuBtn(this);
        a_dynamicWallPaperBtn->setMouseTracking(true); {
            a_dynamicWallPaperBtn->removeFilter();
            a_dynamicWallPaperBtn->setObjectName(QStringLiteral("wallPaperBtn"));
            a_dynamicWallPaperBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_dynamicWallPaperBtn->setFixedSize(75, 85);
            a_dynamicWallPaperBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_dynamicWallPaperBtn->setText(QStringLiteral("动态壁纸"));
            a_dynamicWallPaperBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/wallpaper.png")));
            a_dynamicWallPaperBtn->setIconSize(QSize(35, 35));
            connect(a_dynamicWallPaperBtn, &QToolButton::clicked, this, [this] {
                emit wallpaper();
            });
            //auto a_dynamicWallPaperAction = new QWidgetAction(this);
            //a_dynamicWallPaperAction->setDefaultWidget(a_dynamicWallPaperBtn);
            //connect(a_dynamicWallPaperAction, &QWidgetAction::hovered, this, [a_dynamicWallPaperBtn,this] {
            //    checkHover();
            //    this->m_currentHover.emplace_back(a_dynamicWallPaperBtn);
            //    this->m_lastHover = this->m_currentHover;
            //    QEvent enterEvent(QEvent::Enter); // 创建进入事件
            //    QCoreApplication::sendEvent(a_dynamicWallPaperBtn, &enterEvent); // 发送事件
            //    // 模拟按钮进入 hover 状态
            //    a_dynamicWallPaperBtn->setAttribute(Qt::WA_UnderMouse, true);
            //});
        }
        //手机play按钮
        auto a_phonePlayBtn = new MenuBtn(this);
        a_phonePlayBtn->setMouseTracking(true); {
            a_phonePlayBtn->removeFilter();
            a_phonePlayBtn->setObjectName(QStringLiteral("phonePlayBtn"));
            a_phonePlayBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_phonePlayBtn->setFixedSize(75, 85);
            a_phonePlayBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_phonePlayBtn->setText(QStringLiteral("手机play"));
            a_phonePlayBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/phonePlay.png")));
            a_phonePlayBtn->setIconSize(QSize(35, 35));
            connect(a_phonePlayBtn, &QToolButton::clicked, this, [this] {
                emit phonePlay();
            });
            //auto a_phonePlayAction = new QWidgetAction(this);
            //a_phonePlayAction->setDefaultWidget(a_phonePlayBtn);
            //connect(a_phonePlayAction, &QWidgetAction::hovered, this, [a_phonePlayBtn,this] {
            //    checkHover();
            //    this->m_currentHover.emplace_back(a_phonePlayBtn);
            //    this->m_lastHover = this->m_currentHover;
            //    QEvent enterEvent(QEvent::Enter); // 创建进入事件
            //    QCoreApplication::sendEvent(a_phonePlayBtn, &enterEvent); // 发送事件
            //    // 模拟按钮进入 hover 状态
            //    a_phonePlayBtn->setAttribute(Qt::WA_UnderMouse, true);
            //});
        }
        //传歌到设备按钮
        auto a_uploadToDeviceBtn = new MenuBtn(this);
        a_uploadToDeviceBtn->setMouseTracking(true); {
            a_uploadToDeviceBtn->removeFilter();
            a_uploadToDeviceBtn->setObjectName(QStringLiteral("uploadToDeviceBtn"));
            a_uploadToDeviceBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_uploadToDeviceBtn->setFixedSize(75, 85);
            a_uploadToDeviceBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_uploadToDeviceBtn->setText(QStringLiteral("传歌到设备"));
            a_uploadToDeviceBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/uploadToDevice.png")));
            a_uploadToDeviceBtn->setIconSize(QSize(35, 35));
            connect(a_uploadToDeviceBtn, &QToolButton::clicked, this, [this] {
                emit uploadToDevice();
            });
            //auto a_uploadToDeviceAction = new QWidgetAction(this);
            //a_uploadToDeviceAction->setDefaultWidget(a_uploadToDeviceBtn);
            //connect(a_uploadToDeviceAction, &QWidgetAction::hovered, this, [a_uploadToDeviceBtn,this] {
            //    checkHover();
            //    this->m_currentHover.emplace_back(a_uploadToDeviceBtn);
            //    this->m_lastHover = this->m_currentHover;
            //    QEvent enterEvent(QEvent::Enter); // 创建进
            //    QCoreApplication::sendEvent(a_uploadToDeviceBtn, &enterEvent); // 发送事件
            //    // 模拟按钮进入 hover 状态入事件
            //    a_uploadToDeviceBtn->setAttribute(Qt::WA_UnderMouse, true);
            //});
        }
        //听歌赚金币按钮
        auto a_earnCoinBtn = new MenuBtn(this);
        a_earnCoinBtn->setMouseTracking(true); {
            a_earnCoinBtn->removeFilter();
            a_earnCoinBtn->setObjectName(QStringLiteral("earnCoinBtn"));
            a_earnCoinBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_earnCoinBtn->setFixedSize(75, 85);
            a_earnCoinBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_earnCoinBtn->setText(QStringLiteral("听歌赚金币"));
            a_earnCoinBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/earnCoin.png")));
            a_earnCoinBtn->setIconSize(QSize(35, 35));
            connect(a_earnCoinBtn, &QToolButton::clicked, this, [this] {
                emit earnCoin();
            });
            //auto a_earnCoinAction = new QWidgetAction(this);
            //a_earnCoinAction->setDefaultWidget(a_earnCoinBtn);
            //connect(a_earnCoinAction, &QWidgetAction::hovered, this, [a_earnCoinBtn,this] {
            //    checkHover();
            //    this->m_currentHover.emplace_back(a_earnCoinBtn);
            //    this->m_lastHover = this->m_currentHover;
            //    QEvent enterEvent(QEvent::Enter); // 创建进入事件
            //    QCoreApplication::sendEvent(a_earnCoinBtn, &enterEvent); // 发送事件
            //    // 模拟按钮进入 hover 状态
            //    a_earnCoinBtn->setAttribute(Qt::WA_UnderMouse, true);
            //});
        }
        auto a_listWidget = new QWidget(this);
        a_listWidget->setMouseTracking(true);
        a_listWidget->setObjectName(QStringLiteral("listWidget"));
        a_listWidget->setFixedSize(365, 100);
        a_listWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        auto a_hLayout = new QHBoxLayout(a_listWidget);
        a_hLayout->setAlignment(Qt::AlignCenter);
        a_hLayout->setContentsMargins(0, 0, 0, 0);
        a_hLayout->setSpacing(4);
        a_hLayout->addWidget(a_dynamicWallPaperBtn);
        a_hLayout->addWidget(a_phonePlayBtn);
        a_hLayout->addWidget(a_uploadToDeviceBtn);
        a_hLayout->addWidget(a_earnCoinBtn);
        a_topListWidgetAction->setDefaultWidget(a_listWidget);
    }
    //音乐遥控器按钮
    auto a_controlAction = new QWidgetAction(this); {
        auto a_controlToolBtn = new MenuBtn(this);
        a_controlToolBtn->setFixedSize(360, 37);
        a_controlToolBtn->setIconSize(QSize(20, 20));
        a_controlToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/controller-black.svg")));
        a_controlToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/controller-black.svg")),
                                   QIcon(QStringLiteral(":/Res/menuIcon/controller-blue.svg")));
        a_controlToolBtn->setText(QStringLiteral("   音乐遥控器"));
        a_controlAction->setDefaultWidget(a_controlToolBtn);
        connect(a_controlToolBtn, &QToolButton::clicked, this, [this] {
            emit controller();
        });
        connect(a_controlAction, &QWidgetAction::hovered, this, [a_controlToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_controlToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_controlToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_controlToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //均衡器按钮
    auto a_balanceAction = new QWidgetAction(this); {
        auto a_balanceToolBtn = new MenuBtn(this);
        a_balanceToolBtn->setFixedSize(360, 37);
        a_balanceToolBtn->setIconSize(QSize(20, 20));
        a_balanceToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/balance-black.svg")));
        a_balanceToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/balance-black.svg")),
                                   QIcon(QStringLiteral(":/Res/menuIcon/balance-blue.svg")));
        a_balanceToolBtn->setText(QStringLiteral("   均衡器"));
        a_balanceAction->setDefaultWidget(a_balanceToolBtn);
        connect(a_balanceToolBtn, &QToolButton::clicked, this, [this] {
            emit balance();
        });
        connect(a_balanceAction, &QWidgetAction::hovered, this, [a_balanceToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_balanceToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_balanceToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_balanceToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //AI帮你唱按钮
    auto a_aiHelpAction = new QWidgetAction(this); {
        auto a_aiHelpToolBtn = new MenuBtn(this);
        a_aiHelpToolBtn->setFixedSize(360, 37);
        a_aiHelpToolBtn->setIconSize(QSize(20, 20));
        a_aiHelpToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/aihelp-black.svg")));
        a_aiHelpToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/aihelp-black.svg")),
                                  QIcon(QStringLiteral(":/Res/menuIcon/aihelp-blue.svg")));
        a_aiHelpToolBtn->setText(QStringLiteral("   AI帮你唱"));
        a_aiHelpAction->setDefaultWidget(a_aiHelpToolBtn);
        connect(a_aiHelpToolBtn, &QToolButton::clicked, this, [this] {
            emit aiHelpYou();
        });
        connect(a_aiHelpAction, &QWidgetAction::hovered, this, [a_aiHelpToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_aiHelpToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_aiHelpToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_aiHelpToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //音效插件按钮
    auto a_pluginAction = new QWidgetAction(this); {
        auto a_pluginToolBtn = new MenuBtn(this);
        a_pluginToolBtn->setFixedSize(360, 37);
        a_pluginToolBtn->setIconSize(QSize(20, 20));
        a_pluginToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/soundPlugin-black.svg")));
        a_pluginToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/soundPlugin-black.svg")),
                                  QIcon(QStringLiteral(":/Res/menuIcon/soundPlugin-blue.svg")));
        a_pluginToolBtn->setText(QStringLiteral("   音效插件"));
        a_pluginAction->setDefaultWidget(a_pluginToolBtn);
        connect(a_pluginToolBtn, &QToolButton::clicked, this, [this] {
            emit soundPlugin();
        });
        connect(a_pluginAction, &QWidgetAction::hovered, this, [a_pluginToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_pluginToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_pluginToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_pluginToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //定时设置按钮
    auto a_timeSettingAction = new QWidgetAction(this); {
        auto a_timeSettingToolBtn = new MenuBtn(this);
        a_timeSettingToolBtn->setFixedSize(360, 37);
        a_timeSettingToolBtn->setIconSize(QSize(20, 20));
        a_timeSettingToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/timeSetting-black.svg")));
        a_timeSettingToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/timeSetting-black.svg")),
                                       QIcon(QStringLiteral(":/Res/menuIcon/timeSetting-blue.svg")));
        a_timeSettingToolBtn->setText(QStringLiteral("   定时设置"));
        a_timeSettingAction->setDefaultWidget(a_timeSettingToolBtn);
        connect(a_timeSettingToolBtn, &QToolButton::clicked, this, [this] {
            emit timeSetting();
        });
        connect(a_timeSettingAction, &QWidgetAction::hovered, this, [a_timeSettingToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_timeSettingToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_timeSettingToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_timeSettingToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //应用工具按钮
    auto a_appToolAction = new QWidgetAction(this); {
        auto a_appToolToolBtn = new MenuBtn(this);
        a_appToolToolBtn->setFixedSize(360, 37);
        a_appToolToolBtn->setIconSize(QSize(20, 20));
        a_appToolToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/appTool-black.svg")));
        a_appToolToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/appTool-black.svg")),
                                   QIcon(QStringLiteral(":/Res/menuIcon/appTool-blue.svg")));
        a_appToolToolBtn->setText(QStringLiteral("   应用工具"));
        a_appToolAction->setDefaultWidget(a_appToolToolBtn);
        connect(a_appToolToolBtn, &QToolButton::clicked, this, [this] {
            emit appTool();
        });
        connect(a_appToolAction, &QWidgetAction::hovered, this, [a_appToolToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_appToolToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_appToolToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_appToolToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //恢复窗口按钮
    auto a_restoreWindowAction = new QWidgetAction(this); {
        auto a_restoreWindowBtn = new MenuBtn(this);
        a_restoreWindowBtn->setFixedSize(360, 37);
        a_restoreWindowBtn->setIconSize(QSize(20, 20));
        a_restoreWindowBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/restoreWindow-black.svg")));
        a_restoreWindowBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/restoreWindow-black.svg")),
                                     QIcon(QStringLiteral(":/Res/menuIcon/restoreWindow-blue.svg")));
        a_restoreWindowBtn->setText(QStringLiteral("   恢复窗口"));
        a_restoreWindowAction->setDefaultWidget(a_restoreWindowBtn);
        connect(a_restoreWindowBtn, &QToolButton::clicked, this, [this] {
            emit restoreWindow();
        });
        connect(a_restoreWindowAction, &QWidgetAction::hovered, this, [a_restoreWindowBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_restoreWindowBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_restoreWindowBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_restoreWindowBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //检查更新按钮
    auto a_checkUpdateAction = new QWidgetAction(this); {
        auto a_checkUpdateToolBtn = new MenuBtn(this);
        a_checkUpdateToolBtn->setFixedSize(360, 37);
        a_checkUpdateToolBtn->setIconSize(QSize(20, 20));
        a_checkUpdateToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/checkUpdate-black.svg")));
        a_checkUpdateToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/checkUpdate-black.svg")),
                                       QIcon(QStringLiteral(":/Res/menuIcon/checkUpdate-blue.svg")));
        a_checkUpdateToolBtn->setText(QStringLiteral("   检查更新"));
        a_checkUpdateAction->setDefaultWidget(a_checkUpdateToolBtn);
        connect(a_checkUpdateToolBtn, &QToolButton::clicked, this, [this] {
            emit checkUpdate();
        });
        connect(a_checkUpdateAction, &QWidgetAction::hovered, this, [a_checkUpdateToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_checkUpdateToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_checkUpdateToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_checkUpdateToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //帮助与意见反馈按钮（子目录）
    auto a_helpFacebackAction = new QWidgetAction(this);
    {
        auto widget = new QWidget(this);
        widget->setFixedWidth(360);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        //前后两个按钮
        auto a_helpFacebackToolBtn = new MenuBtn(this);
        a_helpFacebackToolBtn->setObjectName("helpFacebackToolBtn");
        a_helpFacebackToolBtn->setFixedSize(320, 37);
        a_helpFacebackToolBtn->setIconSize(QSize(20, 20));
        a_helpFacebackToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/helpFaceback-black.svg")));
        a_helpFacebackToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/helpFaceback-black.svg")),
                                        QIcon(QStringLiteral(":/Res/menuIcon/helpFaceback-blue.svg")));
        a_helpFacebackToolBtn->setText(QStringLiteral("   帮助与意见反馈"));
        auto a_helpRightBtn = new MenuBtn(this);
        a_helpRightBtn->setObjectName("helpRightBtn");
        a_helpRightBtn->setFixedSize(40, 37);
        a_helpRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_helpRightBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/right-black.svg")));
        a_helpRightBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral(":/Res/menuIcon/right-blue.svg")));
        layout->addWidget(a_helpFacebackToolBtn);
        layout->addWidget(a_helpRightBtn);
        a_helpFacebackAction->setDefaultWidget(widget);
        connect(a_helpFacebackAction, &QWidgetAction::hovered, this, [widget,a_helpFacebackToolBtn,a_helpRightBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(widget);
            this->m_currentHover.emplace_back(a_helpFacebackToolBtn);
            this->m_currentHover.emplace_back(a_helpRightBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_helpFacebackToolBtn, &enterEvent); // 发送事件
            QCoreApplication::sendEvent(a_helpRightBtn, &enterEvent); // 发送事件
            // 模拟widget进入 hover 状态
            widget->setAttribute(Qt::WA_UnderMouse, true);
            a_helpFacebackToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            a_helpRightBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
        //子菜单项
        //使用帮助
        auto a_useHelpAction = new QWidgetAction(this); {
            auto a_useHelpToolBtn = new MenuBtn(this);
            a_useHelpToolBtn->setFixedSize(160, 35);
            a_useHelpToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/useHelp-black.svg")));
            a_useHelpToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/useHelp-black.svg")),
                                           QIcon(QStringLiteral(":/Res/menuIcon/useHelp-blue.svg")));
            a_useHelpToolBtn->setText(QStringLiteral("  使用帮助"));
            a_useHelpAction->setDefaultWidget(a_useHelpToolBtn);
            connect(a_useHelpToolBtn, &QToolButton::clicked, this, [this] {
                emit useHelp();
            });
            connect(a_useHelpAction, &QWidgetAction::hovered, this, [a_useHelpToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_useHelpToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_useHelpToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_useHelpToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //意见反馈
        auto a_feedbackAction = new QWidgetAction(this); {
            auto a_feedbackToolBtn = new MenuBtn(this);
            a_feedbackToolBtn->setFixedSize(160, 35);
            a_feedbackToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/feedback-black.svg")));
            a_feedbackToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/feedback-black.svg")),
                                           QIcon(QStringLiteral(":/Res/menuIcon/feedback-blue.svg")));
            a_feedbackToolBtn->setText(QStringLiteral("  意见反馈"));
            a_feedbackAction->setDefaultWidget(a_feedbackToolBtn);
            connect(a_feedbackToolBtn, &QToolButton::clicked, this, [this] {
                emit feedback();
            });
            connect(a_feedbackAction, &QWidgetAction::hovered, this, [a_feedbackToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_feedbackToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_feedbackToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_feedbackToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //用户反馈社区
        auto a_communityAction = new QWidgetAction(this); {
            auto a_communityToolBtn = new MenuBtn(this);
            a_communityToolBtn->setFixedSize(160, 35);
            a_communityToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/community-black.svg")));
            a_communityToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/community-black.svg")),
                                           QIcon(QStringLiteral(":/Res/menuIcon/community-blue.svg")));
            a_communityToolBtn->setText(QStringLiteral("  用户反馈社区"));
            a_communityAction->setDefaultWidget(a_communityToolBtn);
            connect(a_communityToolBtn, &QToolButton::clicked, this, [this] {
                emit community();
            });
            connect(a_communityAction, &QWidgetAction::hovered, this, [a_communityToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_communityToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_communityToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_communityToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //更新信息
        auto a_updateInfoAction = new QWidgetAction(this); {
            auto a_updateInfoToolBtn = new MenuBtn(this);
            a_updateInfoToolBtn->setFixedSize(160, 35);
            a_updateInfoToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/updateInfo-black.svg")));
            a_updateInfoToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/updateInfo-black.svg")),
                                           QIcon(QStringLiteral(":/Res/menuIcon/updateInfo-blue.svg")));
            a_updateInfoToolBtn->setText(QStringLiteral("  更新信息"));
            a_updateInfoAction->setDefaultWidget(a_updateInfoToolBtn);
            connect(a_updateInfoToolBtn, &QToolButton::clicked, this, [this] {
                emit updateInfo();
            });
            connect(a_updateInfoAction, &QWidgetAction::hovered, this, [a_updateInfoToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_updateInfoToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_updateInfoToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_updateInfoToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //关于应用
        auto a_aboutAction = new QWidgetAction(this); {
            auto a_aboutToolBtn = new MenuBtn(this);
            a_aboutToolBtn->setFixedSize(160, 35);
            a_aboutToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/about-black.svg")));
            a_aboutToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/about-black.svg")),
                                           QIcon(QStringLiteral(":/Res/menuIcon/about-blue.svg")));
            a_aboutToolBtn->setText(QStringLiteral("  关于应用"));
            a_aboutAction->setDefaultWidget(a_aboutToolBtn);
            connect(a_aboutToolBtn, &QToolButton::clicked, this, [this] {
                emit about();
            });
            connect(a_aboutAction, &QWidgetAction::hovered, this, [a_aboutToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_aboutToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_aboutToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_aboutToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子目录添加子项
        auto a_helpFacebackMenu = new BaseMenu(this);
        a_helpFacebackMenu->setFixedSize(180, 220);
        a_helpFacebackMenu->addAction(a_useHelpAction);
        a_helpFacebackMenu->addAction(a_feedbackAction);
        a_helpFacebackMenu->addAction(a_communityAction);
        a_helpFacebackMenu->addSeparator();
        a_helpFacebackMenu->addAction(a_updateInfoAction);
        a_helpFacebackMenu->addAction(a_aboutAction);
        //设置Menu
        a_helpFacebackAction->setMenu(a_helpFacebackMenu);
        //让显示菜单项保持高亮???
    }
    //设置按钮
    auto a_settingsAction = new QWidgetAction(this); {
        auto a_settingsToolBtn = new MenuBtn(this);
        a_settingsToolBtn->setFixedSize(360, 37);
        a_settingsToolBtn->setIconSize(QSize(20, 20));
        a_settingsToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/settings-black.svg")));
        a_settingsToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/settings-black.svg")),
                                    QIcon(QStringLiteral(":/Res/menuIcon/settings-blue.svg")));
        a_settingsToolBtn->setText(QStringLiteral("   设置"));
        a_settingsAction->setDefaultWidget(a_settingsToolBtn);
        connect(a_settingsToolBtn, &QToolButton::clicked, this, [this] {
            emit settings();
        });
        connect(a_settingsAction, &QWidgetAction::hovered, this, [a_settingsToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_settingsToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_settingsToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_settingsToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //退出登录按钮
    auto a_logOutAction = new QWidgetAction(this); {
        auto a_logOutToolBtn = new MenuBtn(this);
        a_logOutToolBtn->setFixedSize(360, 37);
        a_logOutToolBtn->setIconSize(QSize(20, 20));
        a_logOutToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/logOut-black.svg")));
        a_logOutToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/logOut-black.svg")),
                                  QIcon(QStringLiteral(":/Res/menuIcon/logOut-blue.svg")));
        a_logOutToolBtn->setText(QStringLiteral("   退出登录"));
        a_logOutAction->setDefaultWidget(a_logOutToolBtn);
        connect(a_logOutToolBtn, &QToolButton::clicked, this, [this] {
            emit logOut();
        });
        connect(a_logOutAction, &QWidgetAction::hovered, this, [a_logOutToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_logOutToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_logOutToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_logOutToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //退出酷狗音乐按钮
    auto a_exitAction = new QWidgetAction(this); {
        auto a_exitToolBtn = new MenuBtn(this);
        a_exitToolBtn->setFixedSize(360, 37);
        a_exitToolBtn->setIconSize(QSize(20, 20));
        a_exitToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/exit-black.svg")));
        a_exitToolBtn->initIcon(QIcon(QStringLiteral(":/Res/menuIcon/exit-black.svg")),
                                QIcon(QStringLiteral(":/Res/menuIcon/exit-blue.svg")));
        a_exitToolBtn->setText(QStringLiteral("   退出酷狗音乐"));
        a_exitAction->setDefaultWidget(a_exitToolBtn);
        connect(a_exitToolBtn, &QToolButton::clicked, this, [this] {
            emit exit();
        });
        connect(a_exitAction, &QWidgetAction::hovered, this, [a_exitToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_exitToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_exitToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_exitToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }

    //菜单添加按钮
    this->addAction(a_topListWidgetAction);
    this->addSeparator();
    this->addAction(a_controlAction);
    this->addAction(a_balanceAction);
    this->addAction(a_aiHelpAction);
    this->addAction(a_pluginAction);
    this->addAction(a_timeSettingAction);
    this->addSeparator();
    this->addAction(a_appToolAction);
    this->addSeparator();
    this->addAction(a_restoreWindowAction);
    this->addAction(a_checkUpdateAction);
    this->addAction(a_helpFacebackAction);
    this->addAction(a_settingsAction);
    this->addSeparator();
    this->addAction(a_logOutAction);
    this->addAction(a_exitAction);
    this->hide();
}

const TitleOptionMenu * TitleOptionMenu::getMenu() const {
    return this;
}
