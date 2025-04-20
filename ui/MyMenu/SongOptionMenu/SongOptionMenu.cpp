//
// Created by WeiWang on 25-1-12.
//

#include "SongOptionMenu.h"
#include "logger.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QWidgetAction>

SongOptionMenu::SongOptionMenu(QWidget *parent)
    : BaseMenu(parent) {
}

void SongOptionMenu::initMenu() {
    //qDebug() << "Entering initSongOptionMenu";
    this->setFixedSize(200, 470);
    //播放按钮
    auto a_playAction = new QWidgetAction(this); {
        auto a_playToolBtn = new MenuBtn(this);
        a_playToolBtn->setFixedSize(180, 35);
        a_playToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/play-black.svg")));
        a_playToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/play-black.svg")),
                                QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/play-blue.svg")));
        a_playToolBtn->setText(QStringLiteral("  播放"));
        a_playAction->setDefaultWidget(a_playToolBtn);
        connect(a_playToolBtn, &QToolButton::clicked, this, [this] {
            emit play();
            this->hide();
        });
        /*connect(a_playAction, &QWidgetAction::hovered, this, [a_playToolBtn,this] {
            //qDebug()<<"进入a_playToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_playToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_playToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_playToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_playAction,a_playToolBtn);
    }
    //下一首播放按钮
    auto a_nextPlayAction = new QWidgetAction(this); {
        auto a_nextPlayToolBtn = new MenuBtn(this);
        a_nextPlayToolBtn->setFixedSize(180, 35);
        a_nextPlayToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/nextplay-black.svg")));
        a_nextPlayToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/nextplay-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/nextplay-blue.svg")));
        a_nextPlayToolBtn->setText(QStringLiteral("  下一首播放"));
        a_nextPlayAction->setDefaultWidget(a_nextPlayToolBtn);
        connect(a_nextPlayToolBtn, &QToolButton::clicked, this, [this] {
            emit nextPlay();
            this->hide();
        });
        /*connect(a_nextPlayAction, &QWidgetAction::hovered, this, [a_nextPlayToolBtn,this] {
            //qDebug()<<"进入a_nextPlayToolBtn";
            checkHover();
            this->m_currentHover.emplace_back(a_nextPlayToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_nextPlayToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_nextPlayToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_nextPlayAction,a_nextPlayToolBtn);
    }
    //添加到(子目录)
    auto a_addToAction = new QWidgetAction(this); {
        auto widget = new QWidget(this);
        widget->setContentsMargins(0,0,0,0);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        //前后两个按钮
        auto a_addToToolBtn = new MenuBtn(widget);
        a_addToToolBtn->setObjectName("addToToolBtn");
        a_addToToolBtn->setStyleSheet("border-top-right-radius: 0px;border-bottom-right-radius: 0px;margin-right: 0;");
        a_addToToolBtn->setFixedSize(145, 35);
        a_addToToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-black.svg")));
        a_addToToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-black.svg")),
                                 QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-blue.svg")));
        a_addToToolBtn->setText(QStringLiteral("  添加到"));
        auto a_addToRightBtn = new MenuBtn(widget);
        a_addToRightBtn->setObjectName("addToRightBtn");
        a_addToRightBtn->setStyleSheet("border-top-left-radius: 0px;border-bottom-left-radius: 0px;margin-left: 0;");
        a_addToRightBtn->setFixedSize(35, 35);
        a_addToRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_addToRightBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")));
        a_addToRightBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-blue.svg")));
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
            a_playQueueToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/playqueue-black.svg")));
            a_playQueueToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/playqueue-black.svg")),
                                         QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/playqueue-blue.svg")));
            a_playQueueToolBtn->setText(QStringLiteral("  播放队列"));
            a_playQueueAction->setDefaultWidget(a_playQueueToolBtn);
            connect(a_playQueueToolBtn, &QToolButton::clicked, this, [this] {
                emit addToPlayQueue();
                this->hide();
            });
            /*connect(a_playQueueAction, &QWidgetAction::hovered, this, [a_playQueueToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_playQueueToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_playQueueToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_playQueueToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_playQueueAction,a_playQueueToolBtn);
        }
        //子菜单项
        //新建歌单
        auto a_newPlayListAction = new QWidgetAction(this); {
            auto a_newPlayListToolBtn = new MenuBtn(this);
            a_newPlayListToolBtn->setFixedSize(130, 35);
            a_newPlayListToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-black.svg")));
            a_newPlayListToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-black.svg")),
                                           QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/add-blue.svg")));
            a_newPlayListToolBtn->setText(QStringLiteral("  新建歌单"));
            a_newPlayListAction->setDefaultWidget(a_newPlayListToolBtn);
            connect(a_newPlayListToolBtn, &QToolButton::clicked, this, [this] {
                emit addToNewSongList();
                this->hide();
            });
            /*connect(a_newPlayListAction, &QWidgetAction::hovered, this, [a_newPlayListToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_newPlayListToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_newPlayListToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_newPlayListToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_newPlayListAction,a_newPlayListToolBtn);
        }
        //子菜单项
        //我喜欢
        auto a_likeAction = new QWidgetAction(this); {
            auto a_likeToolBtn = new MenuBtn(this);
            a_likeToolBtn->setFixedSize(130, 35);
            a_likeToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/like-black.svg")));
            a_likeToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/like-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/like-blue.svg")));
            a_likeToolBtn->setText(QStringLiteral("  我喜欢"));
            a_likeAction->setDefaultWidget(a_likeToolBtn);
            connect(a_likeToolBtn, &QToolButton::clicked, this, [this] {
                emit addToLove();
                this->hide();
            });
            /*connect(a_likeAction, &QWidgetAction::hovered, this, [a_likeToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_likeToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_likeToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_likeToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_likeAction,a_likeToolBtn);
        }
        //子菜单项
        //默认收藏
        auto a_defaultCollectAction = new QWidgetAction(this); {
            auto a_defaultCollectToolBtn = new MenuBtn(this);
            a_defaultCollectToolBtn->setFixedSize(130, 35);
            a_defaultCollectToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/collect-black.svg")));
            a_defaultCollectToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/collect-black.svg")),
                                              QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/collect-blue.svg")));
            a_defaultCollectToolBtn->setText(QStringLiteral("  默认收藏"));
            a_defaultCollectAction->setDefaultWidget(a_defaultCollectToolBtn);
            connect(a_defaultCollectToolBtn, &QToolButton::clicked, this, [this] {
                emit addToCollect();
                this->hide();
            });
            /*connect(a_defaultCollectAction, &QWidgetAction::hovered, this, [a_defaultCollectToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_defaultCollectToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_defaultCollectToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_defaultCollectToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_defaultCollectAction,a_defaultCollectToolBtn);
        }
        //子菜单项
        //默认列表
        auto a_defaultListAction = new QWidgetAction(this); {
            auto a_defaultListToolBtn = new MenuBtn(this);
            a_defaultListToolBtn->setFixedSize(130, 35);
            a_defaultListToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/defaultlist-black.svg")));
            a_defaultListToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/defaultlist-black.svg")),
                                           QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/defaultlist-blue.svg")));
            a_defaultListToolBtn->setText(QStringLiteral("  默认列表"));
            a_defaultListAction->setDefaultWidget(a_defaultListToolBtn);
            connect(a_defaultListToolBtn, &QToolButton::clicked, this, [this] {
                emit addToPlayList();
                this->hide();
            });
            /*connect(a_defaultListAction, &QWidgetAction::hovered, this, [a_defaultListToolBtn,this] {
                checkHover();
                this->m_currentHover.emplace_back(a_defaultListToolBtn);
                this->m_lastHover = this->m_currentHover;
                QEvent enterEvent(QEvent::Enter); // 创建进入事件
                QCoreApplication::sendEvent(a_defaultListToolBtn, &enterEvent); // 发送事件
                // 模拟按钮进入 hover 状态
                a_defaultListToolBtn->setAttribute(Qt::WA_UnderMouse, true);
            });*/
            connectAction(a_defaultListAction,a_defaultListToolBtn);
        }
        //子目录添加子项
        auto a_addToMenu = new BaseMenu(this);
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
        a_downloadToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-black.svg")));
        a_downloadToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg")));
        a_downloadToolBtn->setText(QStringLiteral("  下载"));
        a_downloadAction->setDefaultWidget(a_downloadToolBtn);
        connect(a_downloadToolBtn, &QToolButton::clicked, this, [this] {
            emit download();
            this->hide();
        });
        /*connect(a_downloadAction, &QWidgetAction::hovered, this, [a_downloadToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_downloadToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_downloadToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_downloadToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_downloadAction,a_downloadToolBtn);
    }
    //分享按钮
    auto a_shareAction = new QWidgetAction(this); {
        auto a_shareToolBtn = new MenuBtn(this);
        a_shareToolBtn->setFixedSize(180, 35);
        a_shareToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/share-black.svg")));
        a_shareToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/share-black.svg")),
                                 QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/share-blue.svg")));
        a_shareToolBtn->setText(QStringLiteral("  分享"));
        a_shareAction->setDefaultWidget(a_shareToolBtn);
        connect(a_shareToolBtn, &QToolButton::clicked, this, [this] {
            emit share();
            this->hide();
        });
        /*connect(a_shareAction, &QWidgetAction::hovered, this, [a_shareToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_shareToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_shareToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_shareToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_shareAction,a_shareToolBtn);
    }
    //查看评论按钮
    auto a_commentAction = new QWidgetAction(this); {
        auto a_commentToolBtn = new MenuBtn(this);
        a_commentToolBtn->setFixedSize(180, 35);
        a_commentToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/comment-black.svg")));
        a_commentToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/comment-black.svg")),
                                   QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/comment-blue.svg")));
        a_commentToolBtn->setText(QStringLiteral("  查看评论"));
        a_commentAction->setDefaultWidget(a_commentToolBtn);
        connect(a_commentToolBtn, &QToolButton::clicked, this, [this] {
            emit comment();
            this->hide();
        });
        /*connect(a_commentAction, &QWidgetAction::hovered, this, [a_commentToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_commentToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_commentToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_commentToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_commentAction,a_commentToolBtn);
    }
    //相似歌曲按钮
    auto a_sameSongAction = new QWidgetAction(this); {
        auto a_sameSongToolBtn = new MenuBtn(this);
        a_sameSongToolBtn->setFixedSize(180, 35);
        a_sameSongToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/same-black.svg")));
        a_sameSongToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/same-black.svg")),
                                    QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/same-blue.svg")));
        a_sameSongToolBtn->setText(QStringLiteral("  相似歌曲"));
        a_sameSongAction->setDefaultWidget(a_sameSongToolBtn);
        connect(a_sameSongToolBtn, &QToolButton::clicked, this, [this] {
            emit sameSong();
            this->hide();
        });
        /*connect(a_sameSongAction, &QWidgetAction::hovered, this, [a_sameSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_sameSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_sameSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_sameSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_sameSongAction,a_sameSongToolBtn);
    }
    //查看歌曲信息按钮
    auto a_songInfoAction = new QWidgetAction(this); {
        auto a_songInfoSongToolBtn = new MenuBtn(this);
        a_songInfoSongToolBtn->setFixedSize(180, 35);
        a_songInfoSongToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/songinfo-black.svg")));
        a_songInfoSongToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/songinfo-black.svg")),
                                        QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/songinfo-blue.svg")));
        a_songInfoSongToolBtn->setText(QStringLiteral("  查看歌曲信息"));
        a_songInfoAction->setDefaultWidget(a_songInfoSongToolBtn);
        connect(a_songInfoSongToolBtn, &QToolButton::clicked, this, [this] {
            emit songInfo();
            this->hide();
        });
        /*connect(a_songInfoAction, &QWidgetAction::hovered, this, [a_songInfoSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_songInfoSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_songInfoSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_songInfoSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_songInfoAction,a_songInfoSongToolBtn);
    }
    //从列表中删除按钮
    auto a_deleteAction = new QWidgetAction(this); {
        auto a_deleteSongToolBtn = new MenuBtn(this);
        a_deleteSongToolBtn->setFixedSize(180, 35);
        a_deleteSongToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg")));
        a_deleteSongToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg")),
                                      QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-blue.svg")));
        a_deleteSongToolBtn->setText(QStringLiteral("  从列表中删除"));
        a_deleteAction->setDefaultWidget(a_deleteSongToolBtn);
        connect(a_deleteSongToolBtn, &QToolButton::clicked, this, [this] {
            //qDebug()<<"发送删除信号，删除第 "<<this->m_curIndex<<" 项";
            emit deleteSong(this->m_curIndex);
            this->hide();
        });
        /*connect(a_deleteAction, &QWidgetAction::hovered, this, [a_deleteSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_deleteSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_deleteSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_deleteSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_deleteAction,a_deleteSongToolBtn);
    }
    //打开文件所在目录按钮
    auto a_openFileAction = new QWidgetAction(this); {
        auto a_openFileSongToolBtn = new MenuBtn(this);
        a_openFileSongToolBtn->setFixedSize(180, 35);
        a_openFileSongToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openfile-black.svg")));
        a_openFileSongToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openfile-black.svg")),
                                        QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/openfile-blue.svg")));
        a_openFileSongToolBtn->setText(QStringLiteral("  打开文件所在目录"));
        a_openFileAction->setDefaultWidget(a_openFileSongToolBtn);
        connect(a_openFileSongToolBtn, &QToolButton::clicked, this, [this] {
            emit openInFile();
            this->hide();
        });
        /*connect(a_openFileAction, &QWidgetAction::hovered, this, [a_openFileSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_openFileSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_openFileSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_openFileSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_openFileAction,a_openFileSongToolBtn);
    }
    //搜索按钮(子目录)
    auto a_searchAction = new QWidgetAction(this); {
        auto widget = new QWidget(this);
        widget->setContentsMargins(0,0,0,0);
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        //前后两个按钮
        auto a_searchToolBtn = new MenuBtn(widget);
        a_searchToolBtn->setObjectName("searchToolBtn");
        a_searchToolBtn->setStyleSheet("border-top-right-radius: 0px;border-bottom-right-radius: 0px;margin-right: 0;");
        a_searchToolBtn->setFixedSize(145, 35);
        a_searchToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        a_searchToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        a_searchToolBtn->setText(QStringLiteral("  搜索"));
        auto a_addToRightBtn = new MenuBtn(widget);
        a_addToRightBtn->setObjectName("addToRightBtn");
        a_addToRightBtn->setStyleSheet("border-top-left-radius: 0px;border-bottom-left-radius: 0px;margin-left: 0;");
        a_addToRightBtn->setFixedSize(35, 35);
        a_addToRightBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        a_addToRightBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")));
        a_addToRightBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg")),
                                  QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-blue.svg")));
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
                this->hide();
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

        auto a_searchMenu = new BaseMenu(this);
        a_searchMenu->setFixedSize(140, 65);
        a_searchMenu->addAction(a_searchTitleAction);
        a_searchAction->setMenu(a_searchMenu);
    }
    //上传到音乐云盘按钮
    auto a_uploadAction = new QWidgetAction(this); {
        auto a_uploadSongToolBtn = new MenuBtn(this);
        a_uploadSongToolBtn->setFixedSize(180, 35);
        a_uploadSongToolBtn->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-black.svg")));
        a_uploadSongToolBtn->initIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-black.svg")),
                                      QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-blue.svg")));
        a_uploadSongToolBtn->setText(QStringLiteral("  上传到音乐云盘"));
        a_uploadAction->setDefaultWidget(a_uploadSongToolBtn);
        connect(a_uploadSongToolBtn, &QToolButton::clicked, this, [this] {
            emit upload();
            this->hide();
        });
        /*connect(a_uploadAction, &QWidgetAction::hovered, this, [a_uploadSongToolBtn,this] {
            checkHover();
            this->m_currentHover.emplace_back(a_uploadSongToolBtn);
            this->m_lastHover = this->m_currentHover;
            QEvent enterEvent(QEvent::Enter); // 创建进入事件
            QCoreApplication::sendEvent(a_uploadSongToolBtn, &enterEvent); // 发送事件
            // 模拟按钮进入 hover 状态
            a_uploadSongToolBtn->setAttribute(Qt::WA_UnderMouse, true);
        });*/
        connectAction(a_uploadAction,a_uploadSongToolBtn);
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
    this->hide();
}

const SongOptionMenu *SongOptionMenu::getMenu() const {
    return this;
}
