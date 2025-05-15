/**
 * @file TitleOptionMenu.cpp
 * @brief 实现 TitleOptionMenu 类，提供标题栏选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#include "TitleOptionMenu.h"
#include "logger.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QWidgetAction>

/**
 * @brief 构造函数，初始化标题栏选项菜单
 * @param parent 父控件指针，默认为 nullptr
 */
TitleOptionMenu::TitleOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
}

/**
 * @brief 初始化菜单布局和内容
 */
void TitleOptionMenu::initMenu() {
    this->setFixedSize(380, 600);
    //顶部按钮
    //auto a_topListWidgetAction = new QWidgetAction(this);
    auto a_topListWidgetAction = new QWidgetAction(this); {
        //动态壁纸按钮
        auto a_dynamicWallPaperBtn = new MenuBtn(this);
        a_dynamicWallPaperBtn->setMouseTracking(true);
        {
            a_dynamicWallPaperBtn->removeFilter(); ///< 移除默认事件过滤器
            a_dynamicWallPaperBtn->setObjectName(QStringLiteral("wallPaperBtn"));
            a_dynamicWallPaperBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_dynamicWallPaperBtn->setFixedSize(75, 85);
            a_dynamicWallPaperBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_dynamicWallPaperBtn->setText(QStringLiteral("动态壁纸"));
            a_dynamicWallPaperBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/wallpaper.png")));
            a_dynamicWallPaperBtn->setIconSize(QSize(35, 35));
            connect(a_dynamicWallPaperBtn, &QToolButton::clicked, this, [this] {
                emit wallpaper(); ///< 发出设置动态壁纸信号
                this->hide();
            });
        }

        // 手机播放按钮
        auto a_phonePlayBtn = new MenuBtn(this);
        a_phonePlayBtn->setMouseTracking(true);
        {
            a_phonePlayBtn->removeFilter();
            a_phonePlayBtn->setObjectName(QStringLiteral("phonePlayBtn"));
            a_phonePlayBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_phonePlayBtn->setFixedSize(75, 85);
            a_phonePlayBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_phonePlayBtn->setText(QStringLiteral("手机play"));
            a_phonePlayBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/phonePlay.png")));
            a_phonePlayBtn->setIconSize(QSize(35, 35));
            connect(a_phonePlayBtn, &QToolButton::clicked, this, [this] {
                emit phonePlay(); ///< 发出手机播放信号
                this->hide();
            });
        }

        // 传歌到设备按钮
        auto a_uploadToDeviceBtn = new MenuBtn(this);
        a_uploadToDeviceBtn->setMouseTracking(true);
        {
            a_uploadToDeviceBtn->removeFilter();
            a_uploadToDeviceBtn->setObjectName(QStringLiteral("uploadToDeviceBtn"));
            a_uploadToDeviceBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_uploadToDeviceBtn->setFixedSize(75, 85);
            a_uploadToDeviceBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_uploadToDeviceBtn->setText(QStringLiteral("传歌到设备"));
            a_uploadToDeviceBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/uploadToDevice.png")));
            a_uploadToDeviceBtn->setIconSize(QSize(35, 35));
            connect(a_uploadToDeviceBtn, &QToolButton::clicked, this, [this] {
                emit uploadToDevice(); ///< 发出传歌到设备信号
                this->hide();
            });
        }

        // 听歌赚金币按钮
        auto a_earnCoinBtn = new MenuBtn(this);
        a_earnCoinBtn->setMouseTracking(true);
        {
            a_earnCoinBtn->removeFilter();
            a_earnCoinBtn->setObjectName(QStringLiteral("earnCoinBtn"));
            a_earnCoinBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            a_earnCoinBtn->setFixedSize(75, 85);
            a_earnCoinBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            a_earnCoinBtn->setText(QStringLiteral("听歌赚金币"));
            a_earnCoinBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/earnCoin.png")));
            a_earnCoinBtn->setIconSize(QSize(35, 35));
            connect(a_earnCoinBtn, &QToolButton::clicked, this, [this] {
                emit earnCoin(); ///< 发出听歌赚金币信号
                this->hide();
            });
        }

        // 顶部按钮容器
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

    // 音乐遥控器按钮
    auto a_controlAction = new QWidgetAction(this);
    {
        auto a_controlToolBtn = new MenuBtn(this);
        a_controlToolBtn->setFixedSize(360, 37);
        a_controlToolBtn->setIconSize(QSize(20, 20));
        a_controlToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/controller-black.svg")));
        a_controlToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/controller-black.svg")),
                                   QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/controller-blue.svg"))); ///< 设置正常和悬停图标
        a_controlToolBtn->setText(QStringLiteral("   音乐遥控器"));
        a_controlAction->setDefaultWidget(a_controlToolBtn);
        connect(a_controlToolBtn, &QToolButton::clicked, this, [this] {
            emit controller(); ///< 发出音乐遥控器信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_controlAction, &QWidgetAction::hovered, this, [a_controlToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_controlToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_controlToolBtn, &enterEvent);
            a_controlToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_controlAction, a_controlToolBtn);
    }

    // 均衡器按钮
    auto a_balanceAction = new QWidgetAction(this);
    {
        auto a_balanceToolBtn = new MenuBtn(this);
        a_balanceToolBtn->setFixedSize(360, 37);
        a_balanceToolBtn->setIconSize(QSize(20, 20));
        a_balanceToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/balance-black.svg")));
        a_balanceToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/balance-black.svg")),
                                   QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/balance-blue.svg")));
        a_balanceToolBtn->setText(QStringLiteral("   均衡器"));
        a_balanceAction->setDefaultWidget(a_balanceToolBtn);
        connect(a_balanceToolBtn, &QToolButton::clicked, this, [this] {
            emit balance(); ///< 发出均衡器信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_balanceAction, &QWidgetAction::hovered, this, [a_balanceToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_balanceToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_balanceToolBtn, &enterEvent);
            a_balanceToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_balanceAction, a_balanceToolBtn);
    }

    // AI帮你唱按钮
    auto a_aiHelpAction = new QWidgetAction(this);
    {
        auto a_aiHelpToolBtn = new MenuBtn(this);
        a_aiHelpToolBtn->setFixedSize(360, 37);
        a_aiHelpToolBtn->setIconSize(QSize(20, 20));
        a_aiHelpToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/aihelp-black.svg")));
        a_aiHelpToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/aihelp-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/aihelp-blue.svg")));
        a_aiHelpToolBtn->setText(QStringLiteral("   AI帮你唱"));
        a_aiHelpAction->setDefaultWidget(a_aiHelpToolBtn);
        connect(a_aiHelpToolBtn, &QToolButton::clicked, this, [this] {
            emit aiHelpYou(); ///< 发出 AI 帮你唱信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_aiHelpAction, &QWidgetAction::hovered, this, [a_aiHelpToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_aiHelpToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_aiHelpToolBtn, &enterEvent);
            a_aiHelpToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_aiHelpAction, a_aiHelpToolBtn);
    }

    // 音效插件按钮
    auto a_pluginAction = new QWidgetAction(this);
    {
        auto a_pluginToolBtn = new MenuBtn(this);
        a_pluginToolBtn->setFixedSize(360, 37);
        a_pluginToolBtn->setIconSize(QSize(20, 20));
        a_pluginToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/soundPlugin-black.svg")));
        a_pluginToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/soundPlugin-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/soundPlugin-blue.svg")));
        a_pluginToolBtn->setText(QStringLiteral("   音效插件"));
        a_pluginAction->setDefaultWidget(a_pluginToolBtn);
        connect(a_pluginToolBtn, &QToolButton::clicked, this, [this] {
            emit soundPlugin(); ///< 发出音效插件信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_pluginAction, &QWidgetAction::hovered, this, [a_pluginToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_pluginToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_pluginToolBtn, &enterEvent);
            a_pluginToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_pluginAction, a_pluginToolBtn);
    }

    // 定时设置按钮
    auto a_timeSettingAction = new QWidgetAction(this);
    {
        auto a_timeSettingToolBtn = new MenuBtn(this);
        a_timeSettingToolBtn->setFixedSize(360, 37);
        a_timeSettingToolBtn->setIconSize(QSize(20, 20));
        a_timeSettingToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/timeSetting-black.svg")));
        a_timeSettingToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/timeSetting-black.svg")),
                                       QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/timeSetting-blue.svg")));
        a_timeSettingToolBtn->setText(QStringLiteral("   定时设置"));
        a_timeSettingAction->setDefaultWidget(a_timeSettingToolBtn);
        connect(a_timeSettingToolBtn, &QToolButton::clicked, this, [this] {
            emit timeSetting(); ///< 发出定时设置信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_timeSettingAction, &QWidgetAction::hovered, this, [a_timeSettingToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_timeSettingToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_timeSettingToolBtn, &enterEvent);
            a_timeSettingToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_timeSettingAction, a_timeSettingToolBtn);
    }

    // 应用工具按钮
    auto a_appToolAction = new QWidgetAction(this);
    {
        auto a_appToolToolBtn = new MenuBtn(this);
        a_appToolToolBtn->setFixedSize(360, 37);
        a_appToolToolBtn->setIconSize(QSize(20, 20));
        a_appToolToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/appTool-black.svg")));
        a_appToolToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/appTool-black.svg")),
                                   QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/appTool-blue.svg")));
        a_appToolToolBtn->setText(QStringLiteral("   应用工具"));
        a_appToolAction->setDefaultWidget(a_appToolToolBtn);
        connect(a_appToolToolBtn, &QToolButton::clicked, this, [this] {
            emit appTool(); ///< 发出应用工具信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_appToolAction, &QWidgetAction::hovered, this, [a_appToolToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_appToolToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_appToolToolBtn, &enterEvent);
            a_appToolToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_appToolAction, a_appToolToolBtn);
    }

    // 恢复窗口按钮
    auto a_restoreWindowAction = new QWidgetAction(this);
    {
        auto a_restoreWindowBtn = new MenuBtn(this);
        a_restoreWindowBtn->setFixedSize(360, 37);
        a_restoreWindowBtn->setIconSize(QSize(20, 20));
        a_restoreWindowBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/restoreWindow-black.svg")));
        a_restoreWindowBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/restoreWindow-black.svg")),
                                     QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/restoreWindow-blue.svg")));
        a_restoreWindowBtn->setText(QStringLiteral("   恢复窗口"));
        a_restoreWindowAction->setDefaultWidget(a_restoreWindowBtn);
        connect(a_restoreWindowBtn, &QToolButton::clicked, this, [this] {
            emit restoreWindow(); ///< 发出恢复窗口信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_restoreWindowAction, &QWidgetAction::hovered, this, [a_restoreWindowBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_restoreWindowBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_restoreWindowBtn, &enterEvent);
            a_restoreWindowBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_restoreWindowAction, a_restoreWindowBtn);
    }

    // 检查更新按钮
    auto a_checkUpdateAction = new QWidgetAction(this);
    {
        auto a_checkUpdateToolBtn = new MenuBtn(this);
        a_checkUpdateToolBtn->setFixedSize(360, 37);
        a_checkUpdateToolBtn->setIconSize(QSize(20, 20));
        a_checkUpdateToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/checkUpdate-black.svg")));
        a_checkUpdateToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/checkUpdate-black.svg")),
                                       QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/checkUpdate-blue.svg")));
        a_checkUpdateToolBtn->setText(QStringLiteral("   检查更新"));
        a_checkUpdateAction->setDefaultWidget(a_checkUpdateToolBtn);
        connect(a_checkUpdateToolBtn, &QToolButton::clicked, this, [this] {
            emit checkUpdate(); ///< 发出检查更新信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_checkUpdateAction, &QWidgetAction::hovered, this, [a_checkUpdateToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_checkUpdateToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_checkUpdateToolBtn, &enterEvent);
            a_checkUpdateToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_checkUpdateAction, a_checkUpdateToolBtn);
    }

    // 帮助与意见反馈子菜单
    auto a_helpFacebackAction = new QWidgetAction(this);
    {
        auto widget = new QWidget(this);
        widget->setFixedWidth(360);
        widget->setContentsMargins(0, 0, 0, 0);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // 主按钮
        auto a_helpFacebackToolBtn = new MenuBtn(this);
        a_helpFacebackToolBtn->setObjectName("helpFacebackToolBtn");
        a_helpFacebackToolBtn->setStyleSheet("border-top-right-radius: 0px;border-bottom-right-radius: 0px;margin-right: 0;");
        a_helpFacebackToolBtn->setFixedSize(325, 37);
        a_helpFacebackToolBtn->setIconSize(QSize(20, 20));
        a_helpFacebackToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/helpFaceback-black.svg")));
        a_helpFacebackToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/helpFaceback-black.svg")),
                                        QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/helpFaceback-blue.svg")));
        a_helpFacebackToolBtn->setText(QStringLiteral("   帮助与意见反馈"));

        // 右侧箭头按钮
        auto a_helpRightBtn = new MenuBtn(this);
        a_helpRightBtn->setObjectName("helpRightBtn");
        a_helpRightBtn->setStyleSheet("border-top-left-radius: 0px;border-bottom-left-radius: 0px;margin-left: 0;");
        a_helpRightBtn->setFixedSize(35, 37);
        a_helpRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_helpRightBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")));
        a_helpRightBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")),
                                 QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-blue.svg")));

        layout->addWidget(a_helpFacebackToolBtn);
        layout->addWidget(a_helpRightBtn);
        a_helpFacebackAction->setDefaultWidget(widget);

        connect(a_helpFacebackAction, &QWidgetAction::hovered, this, [widget, a_helpFacebackToolBtn, a_helpRightBtn, this] {
            checkHover();
            this->m_currentHover.emplace_back(widget);
            this->m_currentHover.emplace_back(a_helpFacebackToolBtn);
            this->m_currentHover.emplace_back(a_helpRightBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_helpFacebackToolBtn, &enterEvent);
            QCoreApplication::sendEvent(a_helpRightBtn, &enterEvent);
            widget->setAttribute(Qt::WA_UnderMouse, true);
            a_helpFacebackToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            a_helpRightBtn->setAttribute(Qt::WA_UnderMouse, true); ///< 模拟控件进入悬停状态
        });

        // 子菜单项 - 使用帮助
        auto a_useHelpAction = new QWidgetAction(this);
        {
            auto a_useHelpToolBtn = new MenuBtn(this);
            a_useHelpToolBtn->setFixedSize(160, 35);
            a_useHelpToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/useHelp-black.svg")));
            a_useHelpToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/useHelp-black.svg")),
                                       QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/useHelp-blue.svg")));
            a_useHelpToolBtn->setText(QStringLiteral("  使用帮助"));
            a_useHelpAction->setDefaultWidget(a_useHelpToolBtn);
            connect(a_useHelpToolBtn, &QToolButton::clicked, this, [this] {
                emit useHelp(); ///< 发出使用帮助信号
                this->hide();
            });
            // 以下为注释掉的悬停事件处理代码，保留以供调试
            /*connect(a_useHelpAction, &QWidgetAction::hovered, this, [a_useHelpToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_useHelpToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter);
                QCoreApplication::sendEvent(a_useHelpToolBtn, &enterEvent);
                a_useHelpToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_useHelpAction, a_useHelpToolBtn);
        }

        // 子菜单项 - 意见反馈
        auto a_feedbackAction = new QWidgetAction(this);
        {
            auto a_feedbackToolBtn = new MenuBtn(this);
            a_feedbackToolBtn->setFixedSize(160, 35);
            a_feedbackToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/feedback-black.svg")));
            a_feedbackToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/feedback-black.svg")),
                                        QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/feedback-blue.svg")));
            a_feedbackToolBtn->setText(QStringLiteral("  意见反馈"));
            a_feedbackAction->setDefaultWidget(a_feedbackToolBtn);
            connect(a_feedbackToolBtn, &QToolButton::clicked, this, [this] {
                emit feedback(); ///< 发出意见反馈信号
                this->hide();
            });
            // 以下为注释掉的悬停事件处理代码，保留以供调试
            /*connect(a_feedbackAction, &QWidgetAction::hovered, this, [a_feedbackToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_feedbackToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter);
                QCoreApplication::sendEvent(a_feedbackToolBtn, &enterEvent);
                a_feedbackToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_feedbackAction, a_feedbackToolBtn);
        }

        // 子菜单项 - 用户反馈社区
        auto a_communityAction = new QWidgetAction(this);
        {
            auto a_communityToolBtn = new MenuBtn(this);
            a_communityToolBtn->setFixedSize(160, 35);
            a_communityToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/community-black.svg")));
            a_communityToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/community-black.svg")),
                                         QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/community-blue.svg")));
            a_communityToolBtn->setText(QStringLiteral("  用户反馈社区"));
            a_communityAction->setDefaultWidget(a_communityToolBtn);
            connect(a_communityToolBtn, &QToolButton::clicked, this, [this] {
                emit community(); ///< 发出用户反馈社区信号
                this->hide();
            });
            // 以下为注释掉的悬停事件处理代码，保留以供调试
            /*connect(a_communityAction, &QWidgetAction::hovered, this, [a_communityToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_communityToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter);
                QCoreApplication::sendEvent(a_communityToolBtn, &enterEvent);
                a_communityToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_communityAction, a_communityToolBtn);
        }

        // 子菜单项 - 更新信息
        auto a_updateInfoAction = new QWidgetAction(this);
        {
            auto a_updateInfoToolBtn = new MenuBtn(this);
            a_updateInfoToolBtn->setFixedSize(160, 35);
            a_updateInfoToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/updateInfo-black.svg")));
            a_updateInfoToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/updateInfo-black.svg")),
                                          QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/updateInfo-blue.svg")));
            a_updateInfoToolBtn->setText(QStringLiteral("  更新信息"));
            a_updateInfoAction->setDefaultWidget(a_updateInfoToolBtn);
            connect(a_updateInfoToolBtn, &QToolButton::clicked, this, [this] {
                emit updateInfo(); ///< 发出更新信息信号
                this->hide();
            });
            // 以下为注释掉的悬停事件处理代码，保留以供调试
            /*connect(a_updateInfoAction, &QWidgetAction::hovered, this, [a_updateInfoToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_updateInfoToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter);
                QCoreApplication::sendEvent(a_updateInfoToolBtn, &enterEvent);
                a_updateInfoToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_updateInfoAction, a_updateInfoToolBtn);
        }

        // 子菜单项 - 关于应用
        auto a_aboutAction = new QWidgetAction(this);
        {
            auto a_aboutToolBtn = new MenuBtn(this);
            a_aboutToolBtn->setFixedSize(160, 35);
            a_aboutToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-black.svg")));
            a_aboutToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-black.svg")),
                                     QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/about-blue.svg")));
            a_aboutToolBtn->setText(QStringLiteral("  关于应用"));
            a_aboutAction->setDefaultWidget(a_aboutToolBtn);
            connect(a_aboutToolBtn, &QToolButton::clicked, this, [this] {
                emit about(); ///< 发出关于应用信号
                this->hide();
            });
            // 以下为注释掉的悬停事件处理代码，保留以供调试
            /*connect(a_aboutAction, &QWidgetAction::hovered, this, [a_aboutToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_aboutToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter);
                QCoreApplication::sendEvent(a_aboutToolBtn, &enterEvent);
                a_aboutToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_aboutAction, a_aboutToolBtn);
        }

        // 子菜单
        auto a_helpFacebackMenu = new BaseMenu(this);
        a_helpFacebackMenu->setFixedSize(180, 220);
        a_helpFacebackMenu->addAction(a_useHelpAction);
        a_helpFacebackMenu->addAction(a_feedbackAction);
        a_helpFacebackMenu->addAction(a_communityAction);
        a_helpFacebackMenu->addSeparator();
        a_helpFacebackMenu->addAction(a_updateInfoAction);
        a_helpFacebackMenu->addAction(a_aboutAction);
        a_helpFacebackAction->setMenu(a_helpFacebackMenu); ///< 设置帮助与意见反馈子菜单
    }

    // 设置按钮
    auto a_settingsAction = new QWidgetAction(this);
    {
        auto a_settingsToolBtn = new MenuBtn(this);
        a_settingsToolBtn->setFixedSize(360, 37);
        a_settingsToolBtn->setIconSize(QSize(20, 20));
        a_settingsToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/settings-black.svg")));
        a_settingsToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/settings-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/settings-blue.svg")));
        a_settingsToolBtn->setText(QStringLiteral("   设置"));
        a_settingsAction->setDefaultWidget(a_settingsToolBtn);
        connect(a_settingsToolBtn, &QToolButton::clicked, this, [this] {
            emit settings(); ///< 发出设置信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_settingsAction, &QWidgetAction::hovered, this, [a_settingsToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_settingsToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_settingsToolBtn, &enterEvent);
            a_settingsToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_settingsAction, a_settingsToolBtn);
    }

    // 退出登录按钮
    auto a_logOutAction = new QWidgetAction(this);
    {
        auto a_logOutToolBtn = new MenuBtn(this);
        a_logOutToolBtn->setFixedSize(360, 37);
        a_logOutToolBtn->setIconSize(QSize(20, 20));
        a_logOutToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/logOut-black.svg")));
        a_logOutToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/logOut-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/logOut-blue.svg")));
        a_logOutToolBtn->setText(QStringLiteral("   退出登录"));
        a_logOutAction->setDefaultWidget(a_logOutToolBtn);
        connect(a_logOutToolBtn, &QToolButton::clicked, this, [this] {
            emit logOut(); ///< 发出退出登录信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_logOutAction, &QWidgetAction::hovered, this, [a_logOutToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_logOutToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_logOutToolBtn, &enterEvent);
            a_logOutToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_logOutAction, a_logOutToolBtn);
    }

    // 退出酷狗音乐按钮
    auto a_exitAction = new QWidgetAction(this);
    {
        auto a_exitToolBtn = new MenuBtn(this);
        a_exitToolBtn->setFixedSize(360, 37);
        a_exitToolBtn->setIconSize(QSize(20, 20));
        a_exitToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-black.svg")));
        a_exitToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-black.svg")),
                                QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/exit-blue.svg")));
        a_exitToolBtn->setText(QStringLiteral("   退出酷狗音乐"));
        a_exitAction->setDefaultWidget(a_exitToolBtn);
        connect(a_exitToolBtn, &QToolButton::clicked, this, [this] {
            emit exit(); ///< 发出退出应用信号
            this->hide();
        });
        // 以下为注释掉的悬停事件处理代码，保留以供调试
        /*connect(a_exitAction, &QWidgetAction::hovered, this, [a_exitToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_exitToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter);
            QCoreApplication::sendEvent(a_exitToolBtn, &enterEvent);
            a_exitToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_exitAction, a_exitToolBtn);
    }

    // 添加所有动作到菜单
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

/**
 * @brief 获取当前菜单对象
 * @return 当前菜单对象指针
 */
const TitleOptionMenu *TitleOptionMenu::getMenu() const {
    return this;
}