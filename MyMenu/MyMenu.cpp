//
// Created by WeiWang on 24-10-19.
//

#include "MyMenu.h"

#include <QCoreApplication>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QBoxLayout>
#include <QWidgetAction>
#include <QActionGroup>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))
constexpr int SHADOW_WIDTH = 5;
constexpr int RADIUS = 12;

MyMenu::MyMenu(const MenuKind &kind, QWidget *parent)
    : QMenu(parent) {
    initUi();
    switch (kind) {
        case MenuKind::SongOptionMenu: initSongOptionMenu();
            break;
        case MenuKind::SortOptionMenu: initSortOptionMenu();
            break;
        case MenuKind::TitleOptionMenu: initTitleOptionMenu();
            break;
        case MenuKind::None: break;
    }
}

void MyMenu::initUi() {
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setMouseTracking(true);
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/menu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

void MyMenu::initSongOptionMenu() {
    //qDebug() << "Entering initSongOptionMenu";
    this->setFixedSize(200, 470);
    //播放按钮
    auto a_playAction = new QWidgetAction(this); {
        auto a_playToolBtn = new MenuBtn(this);
        a_playToolBtn->setFixedSize(180, 35);
        a_playToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/play-black.svg")));
        a_playToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/play-black.svg")),
                                QIcon(QStringLiteral("://Res/menuIcon/play-blue.svg")));
        a_playToolBtn->setText(QStringLiteral("  播放"));
        a_playAction->setDefaultWidget(a_playToolBtn);
        connect(a_playToolBtn, &QToolButton::clicked, this, [this] {
            emit play();
        });
        connect(a_playAction, &QWidgetAction::hovered, this, [a_playToolBtn,this] {
            //qDebug()<<"进入a_playToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_playToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_playToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_playToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //下一首播放按钮
    auto a_nextPlayAction = new QWidgetAction(this); {
        auto a_nextPlayToolBtn = new MenuBtn(this);
        a_nextPlayToolBtn->setFixedSize(180, 35);
        a_nextPlayToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/nextplay-black.svg")));
        a_nextPlayToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/nextplay-black.svg")),
                                    QIcon(QStringLiteral("://Res/menuIcon/nextplay-blue.svg")));
        a_nextPlayToolBtn->setText(QStringLiteral("  下一首播放"));
        a_nextPlayAction->setDefaultWidget(a_nextPlayToolBtn);
        connect(a_nextPlayToolBtn, &QToolButton::clicked, this, [this] {
            emit nextPlay();
        });
        connect(a_nextPlayAction, &QWidgetAction::hovered, this, [a_nextPlayToolBtn,this] {
            //qDebug()<<"进入a_nextPlayToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_nextPlayToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_nextPlayToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_nextPlayToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //添加到(子目录)
    auto a_addToAction = new QWidgetAction(this); {
        auto widget = new QWidget(this);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        //前后两个按钮
        auto a_addToToolBtn = new MenuBtn(this);
        a_addToToolBtn->setObjectName("addToToolBtn");
        a_addToToolBtn->setFixedSize(145, 35);
        a_addToToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/add-black.svg")));
        a_addToToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/add-black.svg")),
                                 QIcon(QStringLiteral("://Res/menuIcon/add-blue.svg")));
        a_addToToolBtn->setText(QStringLiteral("  添加到"));
        auto a_addToRightBtn = new MenuBtn(this);
        a_addToRightBtn->setFixedSize(35, 35);
        a_addToRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_addToRightBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")));
        a_addToRightBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/right-blue.svg")));
        layout->addWidget(a_addToToolBtn);
        layout->addWidget(a_addToRightBtn);
        a_addToAction->setDefaultWidget(widget);
        connect(a_addToAction, &QWidgetAction::hovered, this, [widget,a_addToRightBtn,a_addToToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(widget);
            this->m_currentHover.emplace_back(a_addToToolBtn);
            this->m_currentHover.emplace_back(a_addToRightBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_addToToolBtn, &enterEvent); // 发送事件
            QCoreApplication::sendEvent(a_addToRightBtn, &enterEvent); // 发送事件
            // 模拟widget进入 hover 状态
            widget->setAttribute(Qt::WA_UnderMouse, true);
            a_addToToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            a_addToRightBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
        //子菜单项
        //播放队列
        auto a_playQueueAction = new QWidgetAction(this); {
            auto a_playQueueToolBtn = new MenuBtn(this);
            a_playQueueToolBtn->setFixedSize(130, 35);
            a_playQueueToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/playqueue-black.svg")));
            a_playQueueToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/playqueue-black.svg")),
                                         QIcon(QStringLiteral("://Res/menuIcon/playqueue-blue.svg")));
            a_playQueueToolBtn->setText(QStringLiteral("  播放队列"));
            a_playQueueAction->setDefaultWidget(a_playQueueToolBtn);
            connect(a_playQueueToolBtn, &QToolButton::clicked, this, [this] {
                emit addToPlayQueue();
            });
            connect(a_playQueueAction, &QWidgetAction::hovered, this, [a_playQueueToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_playQueueToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_playQueueToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_playQueueToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //新建歌单
        auto a_newPlayListAction = new QWidgetAction(this); {
            auto a_newPlayListToolBtn = new MenuBtn(this);
            a_newPlayListToolBtn->setFixedSize(130, 35);
            a_newPlayListToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/add-black.svg")));
            a_newPlayListToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/add-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/add-blue.svg")));
            a_newPlayListToolBtn->setText(QStringLiteral("  新建歌单"));
            a_newPlayListAction->setDefaultWidget(a_newPlayListToolBtn);
            connect(a_newPlayListToolBtn, &QToolButton::clicked, this, [this] {
                emit addToNewSongList();
            });
            connect(a_newPlayListAction, &QWidgetAction::hovered, this, [a_newPlayListToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_newPlayListToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_newPlayListToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_newPlayListToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //我喜欢
        auto a_likeAction = new QWidgetAction(this); {
            auto a_likeToolBtn = new MenuBtn(this);
            a_likeToolBtn->setFixedSize(130, 35);
            a_likeToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/like-black.svg")));
            a_likeToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/like-black.svg")),
                                    QIcon(QStringLiteral("://Res/menuIcon/like-blue.svg")));
            a_likeToolBtn->setText(QStringLiteral("  我喜欢"));
            a_likeAction->setDefaultWidget(a_likeToolBtn);
            connect(a_likeToolBtn, &QToolButton::clicked, this, [this] {
                emit addToLove();
            });
            connect(a_likeAction, &QWidgetAction::hovered, this, [a_likeToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_likeToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_likeToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_likeToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //默认收藏
        auto a_defaultCollectAction = new QWidgetAction(this); {
            auto a_defaultCollectToolBtn = new MenuBtn(this);
            a_defaultCollectToolBtn->setFixedSize(130, 35);
            a_defaultCollectToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/collect-black.svg")));
            a_defaultCollectToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/collect-black.svg")),
                                              QIcon(QStringLiteral("://Res/menuIcon/collect-blue.svg")));
            a_defaultCollectToolBtn->setText(QStringLiteral("  默认收藏"));
            a_defaultCollectAction->setDefaultWidget(a_defaultCollectToolBtn);
            connect(a_defaultCollectToolBtn, &QToolButton::clicked, this, [this] {
                emit addToCollect();
            });
            connect(a_defaultCollectAction, &QWidgetAction::hovered, this, [a_defaultCollectToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_defaultCollectToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_defaultCollectToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_defaultCollectToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子菜单项
        //默认列表
        auto a_defaultListAction = new QWidgetAction(this); {
            auto a_defaultListToolBtn = new MenuBtn(this);
            a_defaultListToolBtn->setFixedSize(130, 35);
            a_defaultListToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/defaultlist-black.svg")));
            a_defaultListToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/defaultlist-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/defaultlist-blue.svg")));
            a_defaultListToolBtn->setText(QStringLiteral("  默认列表"));
            a_defaultListAction->setDefaultWidget(a_defaultListToolBtn);
            connect(a_defaultListToolBtn, &QToolButton::clicked, this, [this] {
                emit addToPlayList();
            });
            connect(a_defaultListAction, &QWidgetAction::hovered, this, [a_defaultListToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_defaultListToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_defaultListToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_defaultListToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }
        //子目录添加子项
        auto a_addToMenu = new MyMenu(MenuKind::None, this);
        //让菜单项保持高亮 ???

        a_addToMenu->setFixedSize(150, 220);
        a_addToMenu->addAction(a_playQueueAction);
        a_addToMenu->addSeparator();
        a_addToMenu->addAction(a_newPlayListAction);
        a_addToMenu->addAction(a_likeAction);
        a_addToMenu->addAction(a_defaultCollectAction);
        a_addToMenu->addAction(a_defaultListAction);
        //设置Menu
        a_addToAction->setMenu(a_addToMenu);
    }
    //下载按钮
    auto a_downloadAction = new QWidgetAction(this); {
        auto a_downloadToolBtn = new MenuBtn(this);
        a_downloadToolBtn->setFixedSize(180, 35);
        a_downloadToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/download-black.svg")));
        a_downloadToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/download-black.svg")),
                                    QIcon(QStringLiteral("://Res/menuIcon/download-blue.svg")));
        a_downloadToolBtn->setText(QStringLiteral("  下载"));
        a_downloadAction->setDefaultWidget(a_downloadToolBtn);
        connect(a_downloadToolBtn, &QToolButton::clicked, this, [this] {
            emit download();
        });
        connect(a_downloadAction, &QWidgetAction::hovered, this, [a_downloadToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_downloadToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_downloadToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_downloadToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //分享按钮
    auto a_shareAction = new QWidgetAction(this); {
        auto a_shareToolBtn = new MenuBtn(this);
        a_shareToolBtn->setFixedSize(180, 35);
        a_shareToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/share-black.svg")));
        a_shareToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/share-black.svg")),
                                 QIcon(QStringLiteral("://Res/menuIcon/share-blue.svg")));
        a_shareToolBtn->setText(QStringLiteral("  分享"));
        a_shareAction->setDefaultWidget(a_shareToolBtn);
        connect(a_shareToolBtn, &QToolButton::clicked, this, [this] {
            emit share();
        });
        connect(a_shareAction, &QWidgetAction::hovered, this, [a_shareToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_shareToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_shareToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_shareToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //查看评论按钮
    auto a_commentAction = new QWidgetAction(this); {
        auto a_commentToolBtn = new MenuBtn(this);
        a_commentToolBtn->setFixedSize(180, 35);
        a_commentToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/comment-black.svg")));
        a_commentToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/comment-black.svg")),
                                   QIcon(QStringLiteral("://Res/menuIcon/comment-blue.svg")));
        a_commentToolBtn->setText(QStringLiteral("  查看评论"));
        a_commentAction->setDefaultWidget(a_commentToolBtn);
        connect(a_commentToolBtn, &QToolButton::clicked, this, [this] {
            emit comment();
        });
        connect(a_commentAction, &QWidgetAction::hovered, this, [a_commentToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_commentToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_commentToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_commentToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //相似歌曲按钮
    auto a_sameSongAction = new QWidgetAction(this); {
        auto a_sameSongToolBtn = new MenuBtn(this);
        a_sameSongToolBtn->setFixedSize(180, 35);
        a_sameSongToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/same-black.svg")));
        a_sameSongToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/same-black.svg")),
                                    QIcon(QStringLiteral("://Res/menuIcon/same-blue.svg")));
        a_sameSongToolBtn->setText(QStringLiteral("  相似歌曲"));
        a_sameSongAction->setDefaultWidget(a_sameSongToolBtn);
        connect(a_sameSongToolBtn, &QToolButton::clicked, this, [this] {
            emit sameSong();
        });
        connect(a_sameSongAction, &QWidgetAction::hovered, this, [a_sameSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sameSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_sameSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_sameSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //查看歌曲信息按钮
    auto a_songInfoAction = new QWidgetAction(this); {
        auto a_songInfoSongToolBtn = new MenuBtn(this);
        a_songInfoSongToolBtn->setFixedSize(180, 35);
        a_songInfoSongToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/songinfo-black.svg")));
        a_songInfoSongToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/songinfo-black.svg")),
                                        QIcon(QStringLiteral("://Res/menuIcon/songinfo-blue.svg")));
        a_songInfoSongToolBtn->setText(QStringLiteral("  查看歌曲信息"));
        a_songInfoAction->setDefaultWidget(a_songInfoSongToolBtn);
        connect(a_songInfoSongToolBtn, &QToolButton::clicked, this, [this] {
            emit songInfo();
        });
        connect(a_songInfoAction, &QWidgetAction::hovered, this, [a_songInfoSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_songInfoSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_songInfoSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_songInfoSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //从列表中删除按钮
    auto a_deleteAction = new QWidgetAction(this); {
        auto a_deleteSongToolBtn = new MenuBtn(this);
        a_deleteSongToolBtn->setFixedSize(180, 35);
        a_deleteSongToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/delete-black.svg")));
        a_deleteSongToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/delete-black.svg")),
                                      QIcon(QStringLiteral("://Res/menuIcon/delete-blue.svg")));
        a_deleteSongToolBtn->setText(QStringLiteral("  从列表中删除"));
        a_deleteAction->setDefaultWidget(a_deleteSongToolBtn);
        connect(a_deleteSongToolBtn, &QToolButton::clicked, this, [this] {
            //qDebug()<<"发送删除信号，删除第 "<<this->m_curIndex<<" 项";
            emit deleteSong(this->m_curIndex);
        });
        connect(a_deleteAction, &QWidgetAction::hovered, this, [a_deleteSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_deleteSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_deleteSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_deleteSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //打开文件所在目录按钮
    auto a_openFileAction = new QWidgetAction(this); {
        auto a_openFileSongToolBtn = new MenuBtn(this);
        a_openFileSongToolBtn->setFixedSize(180, 35);
        a_openFileSongToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/openfile-black.svg")));
        a_openFileSongToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/openfile-black.svg")),
                                        QIcon(QStringLiteral("://Res/menuIcon/openfile-blue.svg")));
        a_openFileSongToolBtn->setText(QStringLiteral("  打开文件所在目录"));
        a_openFileAction->setDefaultWidget(a_openFileSongToolBtn);
        connect(a_openFileSongToolBtn, &QToolButton::clicked, this, [this] {
            emit openInFile();
        });
        connect(a_openFileAction, &QWidgetAction::hovered, this, [a_openFileSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_openFileSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_openFileSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_openFileSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //搜索按钮(子目录)
    auto a_searchAction = new QWidgetAction(this); {
        auto widget = new QWidget(this);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        //前后两个按钮
        auto a_searchToolBtn = new MenuBtn(this);
        a_searchToolBtn->setObjectName("searchToolBtn");
        a_searchToolBtn->setFixedSize(145, 35);
        a_searchToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/search-black.svg")));
        a_searchToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/search-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/search-blue.svg")));
        a_searchToolBtn->setText(QStringLiteral("  搜索"));
        auto a_addToRightBtn = new MenuBtn(this);
        a_addToRightBtn->setFixedSize(35, 35);
        a_addToRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_addToRightBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")));
        a_addToRightBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/right-blue.svg")));
        layout->addWidget(a_searchToolBtn);
        layout->addWidget(a_addToRightBtn);
        a_searchAction->setDefaultWidget(widget);
        connect(a_searchAction, &QWidgetAction::hovered, this, [widget,a_addToRightBtn,a_searchToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(widget);
            this->m_currentHover.emplace_back(a_searchToolBtn);
            this->m_currentHover.emplace_back(a_addToRightBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_searchToolBtn, &enterEvent); // 发送事件
            QCoreApplication::sendEvent(a_addToRightBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            widget->setAttribute(Qt::WA_UnderMouse, true);
            a_searchToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            a_addToRightBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
        //子菜单项
        auto a_searchTitleAction = new QWidgetAction(this); {
            auto a_searchTitleBtn = new QToolButton(this);
            a_searchTitleBtn->setFixedSize(120, 35);
            a_searchTitleBtn->setText(QStringLiteral("搜索本歌曲"));
            a_searchTitleAction->setDefaultWidget(a_searchTitleBtn);
            connect(a_searchTitleBtn, &QToolButton::clicked, this, [this] {
                emit search();
            });
            connect(a_searchTitleAction, &QWidgetAction::hovered, this, [a_searchTitleBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_searchTitleBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_searchTitleBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_searchTitleBtn->setAttribute(Qt::WA_UnderMouse, true);
            });
        }

        auto a_searchMenu = new MyMenu(MenuKind::None, this);
        a_searchMenu->setFixedSize(140, 65);
        a_searchMenu->addAction(a_searchTitleAction);
        a_searchAction->setMenu(a_searchMenu);
    }
    //上传到音乐云盘按钮
    auto a_uploadAction = new QWidgetAction(this); {
        auto a_uploadSongToolBtn = new MenuBtn(this);
        a_uploadSongToolBtn->setFixedSize(180, 35);
        a_uploadSongToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/upload-black.svg")));
        a_uploadSongToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/upload-black.svg")),
                                      QIcon(QStringLiteral("://Res/menuIcon/upload-blue.svg")));
        a_uploadSongToolBtn->setText(QStringLiteral("  上传到音乐云盘"));
        a_uploadAction->setDefaultWidget(a_uploadSongToolBtn);
        connect(a_uploadSongToolBtn, &QToolButton::clicked, this, [this] {
            emit upload();
        });
        connect(a_uploadAction, &QWidgetAction::hovered, this, [a_uploadSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_uploadSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_uploadSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_uploadSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }

    this->addAction(a_playAction);
    this->addAction(a_nextPlayAction);
    this->addSeparator();
    this->addAction(a_addToAction);
    this->addAction(a_downloadAction);
    this->addAction(a_shareAction);
    this->addSeparator();
    this->addAction(a_commentAction);
    this->addAction(a_sameSongAction);
    this->addAction(a_songInfoAction);
    this->addSeparator();
    this->addAction(a_deleteAction);
    this->addAction(a_openFileAction);
    this->addAction(a_searchAction);
    this->addAction(a_uploadAction);
    //qDebug() << "Exiting initSongOptionMenu";
}

void MyMenu::initSortOptionMenu() {
    this->setFixedSize(140, 280);
    //默认排序
    auto a_sortByDefaultAction = new QWidgetAction(this); {
        auto a_sortByDefaultToolBtn = new MenuBtn(this);
        a_sortByDefaultToolBtn->removeFilter();
        a_sortByDefaultToolBtn->setFixedSize(120, 35);
        a_sortByDefaultToolBtn->setIconSize(QSize(18, 18));
        a_sortByDefaultToolBtn->setIcon(QIcon());
        a_sortByDefaultToolBtn->setText(QStringLiteral("  默认排序"));
        a_sortByDefaultAction->setDefaultWidget(a_sortByDefaultToolBtn);
        connect(a_sortByDefaultToolBtn, &QToolButton::clicked, this, [this,a_sortByDefaultToolBtn] {
            //默认排序无图标
            emit deselected();
            emit defaultSort();
            this->m_curSelect = a_sortByDefaultToolBtn;
            //清除之前的图标
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
        });
        connect(a_sortByDefaultAction, &QWidgetAction::hovered, this, [a_sortByDefaultToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByDefaultToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByDefaultToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //添加时间
    auto a_sortByAddTimeAction = new QWidgetAction(this); {
        auto a_sortByAddTimeToolBtn = new MenuBtn(this);
        a_sortByAddTimeToolBtn->removeFilter();
        a_sortByAddTimeToolBtn->setFixedSize(120, 35);
        a_sortByAddTimeToolBtn->setIconSize(QSize(18, 18));
        a_sortByAddTimeToolBtn->setIcon(QIcon());
        a_sortByAddTimeToolBtn->setText(QStringLiteral("  添加时间"));
        a_sortByAddTimeAction->setDefaultWidget(a_sortByAddTimeToolBtn);
        connect(a_sortByAddTimeToolBtn, &QToolButton::clicked, this, [a_sortByAddTimeToolBtn,this] {
            //qDebug()<<"点击";
            emit selected();
            this->m_curSelect = a_sortByAddTimeToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByAddTimeToolBtn->icon().isNull()) {
                //qDebug()<<"无图标，设置图标";
                emit addTimeSort(true);
                a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByAddTimeToolBtn->m_cnt = (a_sortByAddTimeToolBtn->m_cnt + 1) % 2;
                if (a_sortByAddTimeToolBtn->m_cnt & 1) {
                    emit addTimeSort(false);
                    a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortup.svg")));
                } else {
                    emit addTimeSort(true);
                    a_sortByAddTimeToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortByAddTimeAction, &QWidgetAction::hovered, this, [a_sortByAddTimeToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByAddTimeToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByAddTimeToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //歌曲名
    auto a_sortBySongNameAction = new QWidgetAction(this); {
        auto a_sortBySongNameToolBtn = new MenuBtn(this);
        a_sortBySongNameToolBtn->removeFilter();
        a_sortBySongNameToolBtn->setFixedSize(120, 35);
        a_sortBySongNameToolBtn->setIconSize(QSize(18, 18));
        a_sortBySongNameToolBtn->setIcon(QIcon());
        a_sortBySongNameToolBtn->setText(QStringLiteral("  歌曲名    "));
        a_sortBySongNameAction->setDefaultWidget(a_sortBySongNameToolBtn);
        connect(a_sortBySongNameToolBtn, &QToolButton::clicked, this, [this,a_sortBySongNameToolBtn] {
            emit selected();

            this->m_curSelect = a_sortBySongNameToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;

            if (a_sortBySongNameToolBtn->icon().isNull()) {
                emit songNameSort(true);
                a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
            } else {
                a_sortBySongNameToolBtn->m_cnt = (a_sortBySongNameToolBtn->m_cnt + 1) % 2;
                if (a_sortBySongNameToolBtn->m_cnt & 1) {
                    emit songNameSort(false);
                    a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortup.svg")));
                } else {
                    emit songNameSort(true);
                    a_sortBySongNameToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortBySongNameAction, &QWidgetAction::hovered, this, [a_sortBySongNameToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortBySongNameToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortBySongNameToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //歌手
    auto a_sortBySingerAction = new QWidgetAction(this); {
        auto a_sortBySingerToolBtn = new MenuBtn(this);
        a_sortBySingerToolBtn->removeFilter();
        a_sortBySingerToolBtn->setFixedSize(120, 35);
        a_sortBySingerToolBtn->setIconSize(QSize(18, 18));
        a_sortBySingerToolBtn->setIcon(QIcon());
        a_sortBySingerToolBtn->setText(QStringLiteral("  歌手       "));
        a_sortBySingerAction->setDefaultWidget(a_sortBySingerToolBtn);
        connect(a_sortBySingerToolBtn, &QToolButton::clicked, this, [this,a_sortBySingerToolBtn] {
            emit selected();
            this->m_curSelect = a_sortBySingerToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortBySingerToolBtn->icon().isNull()) {
                emit singerSort(true);
                a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
            } else {
                a_sortBySingerToolBtn->m_cnt = (a_sortBySingerToolBtn->m_cnt + 1) % 2;
                if (a_sortBySingerToolBtn->m_cnt & 1) {
                    emit singerSort(false);
                    a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortup.svg")));
                } else {
                    emit singerSort(true);
                    a_sortBySingerToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortBySingerAction, &QWidgetAction::hovered, this, [a_sortBySingerToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortBySingerToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortBySingerToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //时长
    auto a_sortByDurationAction = new QWidgetAction(this); {
        auto a_sortByDurationToolBtn = new MenuBtn(this);
        a_sortByDurationToolBtn->removeFilter();
        a_sortByDurationToolBtn->setFixedSize(120, 35);
        a_sortByDurationToolBtn->setIconSize(QSize(18, 18));
        a_sortByDurationToolBtn->setIcon(QIcon());
        a_sortByDurationToolBtn->setText(QStringLiteral("  时长       "));
        a_sortByDurationAction->setDefaultWidget(a_sortByDurationToolBtn);
        connect(a_sortByDurationToolBtn, &QToolButton::clicked, this, [this,a_sortByDurationToolBtn] {
            emit selected();
            this->m_curSelect = a_sortByDurationToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByDurationToolBtn->icon().isNull()) {
                emit durationSort(true);
                a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByDurationToolBtn->m_cnt = (a_sortByDurationToolBtn->m_cnt + 1) % 2;
                if (a_sortByDurationToolBtn->m_cnt & 1) {
                    emit durationSort(false);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortup.svg")));
                } else {
                    emit durationSort(true);
                    a_sortByDurationToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortByDurationAction, &QWidgetAction::hovered, this, [a_sortByDurationToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByDurationToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByDurationToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //播放次数
    auto a_sortByPlayCountAction = new QWidgetAction(this); {
        auto a_sortByPlayCountToolBtn = new MenuBtn(this);
        a_sortByPlayCountToolBtn->removeFilter();
        a_sortByPlayCountToolBtn->setFixedSize(120, 35);
        a_sortByPlayCountToolBtn->setIconSize(QSize(18, 18));
        a_sortByPlayCountToolBtn->setIcon(QIcon());
        a_sortByPlayCountToolBtn->setText(QStringLiteral("  播放次数"));
        a_sortByPlayCountAction->setDefaultWidget(a_sortByPlayCountToolBtn);
        connect(a_sortByPlayCountToolBtn, &QToolButton::clicked, this, [this,a_sortByPlayCountToolBtn] {
            emit selected();
            this->m_curSelect = a_sortByPlayCountToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
            if (a_sortByPlayCountToolBtn->icon().isNull()) {
                emit playCountSort(true);
                a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
            } else {
                a_sortByPlayCountToolBtn->m_cnt = (a_sortByPlayCountToolBtn->m_cnt + 1) % 2;
                if (a_sortByPlayCountToolBtn->m_cnt & 1) {
                    emit playCountSort(false);
                    a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortup.svg")));
                } else {
                    emit playCountSort(true);
                    a_sortByPlayCountToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/sortdown.svg")));
                }
            }
        });
        connect(a_sortByPlayCountAction, &QWidgetAction::hovered, this, [a_sortByPlayCountToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByPlayCountToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByPlayCountToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //随机排序
    auto a_sortByRandomAction = new QWidgetAction(this); {
        auto a_sortByRandomToolBtn = new MenuBtn(this);
        a_sortByRandomToolBtn->setFixedSize(120, 35);
        a_sortByRandomToolBtn->removeFilter();
        a_sortByRandomToolBtn->setIconSize(QSize(18, 18));
        a_sortByRandomToolBtn->setIcon(QIcon());
        a_sortByRandomToolBtn->setText(QStringLiteral("  随机排序"));
        a_sortByRandomAction->setDefaultWidget(a_sortByRandomToolBtn);
        connect(a_sortByRandomToolBtn, &QToolButton::clicked, this, [this,a_sortByRandomToolBtn] {
            emit selected();
            emit randomSort();
            this->m_curSelect = a_sortByRandomToolBtn;
            checkSelection();
            this->m_lastSelect = this->m_curSelect;
        });
        connect(a_sortByRandomAction, &QWidgetAction::hovered, this, [a_sortByRandomToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sortByRandomToolBtn);
            this->m_lastHover = this->m_currentHover;
            // 模拟hover
            a_sortByRandomToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });
    }
    //添加Action
    this->addAction(a_sortByDefaultAction);
    this->addAction(a_sortByAddTimeAction);
    this->addAction(a_sortBySongNameAction);
    this->addAction(a_sortBySingerAction);
    this->addAction(a_sortByDurationAction);
    this->addAction(a_sortByPlayCountAction);
    this->addAction(a_sortByRandomAction);
}

void MyMenu::initTitleOptionMenu() {
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
            a_dynamicWallPaperBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/wallpaper.png")));
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
            a_phonePlayBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/phonePlay.png")));
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
            a_uploadToDeviceBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/uploadToDevice.png")));
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
            a_earnCoinBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/earnCoin.png")));
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
        a_controlToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/controller-black.svg")));
        a_controlToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/controller-black.svg")),
                                   QIcon(QStringLiteral("://Res/menuIcon/controller-blue.svg")));
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
        a_balanceToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/balance-black.svg")));
        a_balanceToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/balance-black.svg")),
                                   QIcon(QStringLiteral("://Res/menuIcon/balance-blue.svg")));
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
        a_aiHelpToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/aihelp-black.svg")));
        a_aiHelpToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/aihelp-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/aihelp-blue.svg")));
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
        a_pluginToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/soundPlugin-black.svg")));
        a_pluginToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/soundPlugin-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/soundPlugin-blue.svg")));
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
        a_timeSettingToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/timeSetting-black.svg")));
        a_timeSettingToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/timeSetting-black.svg")),
                                       QIcon(QStringLiteral("://Res/menuIcon/timeSetting-blue.svg")));
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
        a_appToolToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/appTool-black.svg")));
        a_appToolToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/appTool-black.svg")),
                                   QIcon(QStringLiteral("://Res/menuIcon/appTool-blue.svg")));
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
        a_restoreWindowBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/restoreWindow-black.svg")));
        a_restoreWindowBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/restoreWindow-black.svg")),
                                     QIcon(QStringLiteral("://Res/menuIcon/restoreWindow-blue.svg")));
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
        a_checkUpdateToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/checkUpdate-black.svg")));
        a_checkUpdateToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/checkUpdate-black.svg")),
                                       QIcon(QStringLiteral("://Res/menuIcon/checkUpdate-blue.svg")));
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
        a_helpFacebackToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/helpFaceback-black.svg")));
        a_helpFacebackToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/helpFaceback-black.svg")),
                                        QIcon(QStringLiteral("://Res/menuIcon/helpFaceback-blue.svg")));
        a_helpFacebackToolBtn->setText(QStringLiteral("   帮助与意见反馈"));
        auto a_helpRightBtn = new MenuBtn(this);
        a_helpRightBtn->setObjectName("helpRightBtn");
        a_helpRightBtn->setFixedSize(40, 37);
        a_helpRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_helpRightBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")));
        a_helpRightBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/right-blue.svg")));
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
            a_useHelpToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/useHelp-black.svg")));
            a_useHelpToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/useHelp-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/useHelp-blue.svg")));
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
            a_feedbackToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/feedback-black.svg")));
            a_feedbackToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/feedback-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/feedback-blue.svg")));
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
            a_communityToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/community-black.svg")));
            a_communityToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/community-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/community-blue.svg")));
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
            a_updateInfoToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/updateInfo-black.svg")));
            a_updateInfoToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/updateInfo-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/updateInfo-blue.svg")));
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
            a_aboutToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/about-black.svg")));
            a_aboutToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/about-black.svg")),
                                           QIcon(QStringLiteral("://Res/menuIcon/about-blue.svg")));
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
        auto a_helpFacebackMenu = new MyMenu(MenuKind::None, this);
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
        //connect(a_helpFacebackMenu,&MyMenu::showSelf,this,[a_helpFacebackAction] {
        //    a_helpFacebackAction->hovered();
        //    qDebug()<<"子菜单显示，触发悬停";
        //});
    }
    //设置按钮
    auto a_settingsAction = new QWidgetAction(this); {
        auto a_settingsToolBtn = new MenuBtn(this);
        a_settingsToolBtn->setFixedSize(360, 37);
        a_settingsToolBtn->setIconSize(QSize(20, 20));
        a_settingsToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/settings-black.svg")));
        a_settingsToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/settings-black.svg")),
                                    QIcon(QStringLiteral("://Res/menuIcon/settings-blue.svg")));
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
        a_logOutToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/logOut-black.svg")));
        a_logOutToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/logOut-black.svg")),
                                  QIcon(QStringLiteral("://Res/menuIcon/logOut-blue.svg")));
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
        a_exitToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/exit-black.svg")));
        a_exitToolBtn->initIcon(QIcon(QStringLiteral("://Res/menuIcon/exit-black.svg")),
                                QIcon(QStringLiteral("://Res/menuIcon/exit-blue.svg")));
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
}

void MyMenu::checkHover() {
    if (!this->m_lastHover.isEmpty()) {
        QEvent leaveEvent(QEvent::Leave); // 创建进入事件
        for (const auto val: m_lastHover) {
            QCoreApplication::sendEvent(val, &leaveEvent); // 发送事件
        }
        m_lastHover.clear();
        m_currentHover.clear();
    }
}

void MyMenu::checkSelection() {
    if (m_lastSelect) {
        //qDebug()<<"之前指向目标存在";
        if (m_lastSelect == m_curSelect)return;
        if (!m_lastSelect->icon().isNull()) {
            m_lastSelect->setIcon(QIcon());
            //qDebug()<<"图标设置为空";
        }
    }
}

void MyMenu::setCurIndex(const int &idx) {
    this->m_curIndex = idx;
}

void MyMenu::paintEvent(QPaintEvent *event) {
    QMenu::paintEvent(event);
    //绘制阴影
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,
                        RADIUS, RADIUS);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2,
                            this->height() - (SHADOW_WIDTH - i) * 2, RADIUS, RADIUS);
        color.setAlpha(180 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void MyMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    //this->setFocus(); // 强制widget接收焦点
    emit showSelf();
}

void MyMenu::leaveEvent(QEvent *event) {
    QMenu::leaveEvent(event);
    checkHover();
}
