/**
 * @file KuGouApp.cpp
 * @brief 实现 KuGouApp 类，管理酷狗音乐主界面
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "KuGouClient.h"
#include "Async.h"
#include "ElaMessageBar.h"
#include "MyScrollArea.h"
#include "RefreshMask.h"
#include "RippleButton.h"
#include "logger.hpp"
#include "qtmaterialsnackbar.h"
#include "ui_KuGouClient.h"

#include <QButtonGroup>
#include <QDir>
#include <QShortcut>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR                                                                                                \
    (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 生成圆角图片
 * @param src 源图片
 * @param size 目标大小
 * @param radius 圆角半径
 * @return 处理后的圆角图片
 * @note 使用抗锯齿和裁剪路径生成圆角效果
 */

/**
 * @brief 构造函数，初始化酷狗音乐主界面
 * @param parent 父窗口指针，默认为 nullptr
 */
KuGouClient::KuGouClient(MainWindow *parent)
    : MainWindow(parent)
      , ui(new Ui::KuGouClient)
      , m_menuBtnGroup(std::make_unique<QButtonGroup>(this)) ///< 初始化菜单按钮组
      , m_refreshMask(std::make_unique<RefreshMask>())       ///< 初始化刷新遮罩
      , m_snackbar(std::make_unique<QtMaterialSnackbar>())   ///< 初始化消息提示条
      , m_lyricWidget(std::make_unique<LyricWidget>(this))   ///< 初始化歌词组件
{
    {
        // 初始化日志
        if (!mylog::logger::get().init("../logs/main.log")) {
            qWarning() << "客户端日志初始化失败";
            return; ///< 日志初始化失败，退出
        }
        mylog::logger::get().set_level(spdlog::level::info); ///< 设置日志级别为 info

        // 三种日志输出方式
        STREAM_INFO() << "STREAM_INFO : 客户端初始化（info）" << "成功"; ///< 流式日志
        PRINT_INFO("PRINT_INFO : 客户端初始化（info）%s", "成功");       ///< 格式化日志
        LOG_INFO("LOG_INFO : 客户端初始化（info）{}", "成功");           ///< 模板日志
    }
    ui->setupUi(this); ///< 设置 UI 布局
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/kugou.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll()); ///< 应用样式表
        } else {
            // @note 未使用，保留用于调试
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
            return;
        }
    }

    initPlayer(); ///< 初始化播放器
    initUi();     ///< 初始化界面

    setupButtonConnections(); // 初始化按钮连接

    // @note 动画结束，恢复按钮可交互
    connect(ui->stackedWidget,
            &SlidingStackedWidget::animationFinished,
            [this] {
                if (m_isInitialized) {
                    /// qDebug()<<__LINE__<<" 动画结束，初始化完成，设置按钮可交互";
                    enableButton(true);
                }
            });
    enableButton(true);                       ///< 启用按钮
    ui->stackedWidget->setVerticalMode(true); ///< 设置堆栈窗口垂直滑动
    // @note 默认选中为你推荐页面
    ui->recommend_you_toolButton->clicked();
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源并关闭日志
 */
KuGouClient::~KuGouClient()
{
    // @note 在 spdlog 静态变量销毁前关闭日志
    mylog::logger::get().shutdown();
    delete ui; ///< 释放 UI 界面
}

/**
 * @brief 初始化播放器
 * @note 设置音量、静音和元数据信号连接
 */
void KuGouClient::initPlayer()
{
    // @note 初始化播放器
    VideoPlayer::initPlayer();
    qRegisterMetaType<VideoPlayer::State>(); ///< 注册播放器状态元类型
    // @note 设置事件处理
    this->m_player = new VideoPlayer(this);          ///< 创建播放器
    m_player->setAbility(false, false, true, false); ///< 设置播放器能力
    m_player->setVolume(0.3);                        ///< 设置音量为 0.3
    m_player->setMute(false);                        ///< 取消静音
    /*
    connect(m_player, &VideoPlayer::albumFound, this, [this](const QString &album) {
        // @note 未使用，保留用于调试
        // qDebug() << "albumFound : " << album;
    });                                                            ///< 连接专辑发现信号
    connect(m_player, &VideoPlayer::artistFound, this, [this](const QString &artist) {
        // @note 未使用，保留用于调试
        // qDebug() << "artistFound : " << artist;
    });                                                            ///< 连接艺术家发现信号
    connect(m_player, &VideoPlayer::titleFound, this, [this](const QString &title) {
        // @note 未使用，保留用于调试
        // qDebug() << "titleFound : " << title;
    });                                                            ///< 连接标题发现信号
    connect(m_player, &VideoPlayer::pictureFound, this, [this](const QPixmap &picture) {
        // @note 未使用，保留用于调试
        // qDebug() << "pictureFound : " << picture;
    });                                                            ///< 连接图片发现信号
    */
}

/**
 * @brief 初始化界面
 * @note 初始化字体、窗口属性和子组件
 */
void KuGouClient::initUi()
{
    this->setWindowIcon(QIcon(QStringLiteral(":/Res/window/windowIcon.png")));  ///< 设置窗口图标
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint); ///< 设置无边框和无阴影
    // @note 移动窗口到屏幕中央
    move(QGuiApplication::primaryScreen()->geometry().width() / 2 - this->width() / 2, 100);

    // @note 设置鼠标追踪
    this->setMouseTracking(true);
    ui->title_widget->setMouseTracking(true);
    ui->center_widget->setMouseTracking(true);
    ui->play_widget->setMouseTracking(true);

    // @note 设置窗口属性
    this->setAttribute(Qt::WA_TranslucentBackground, true); ///< 设置透明背景
    this->setAttribute(Qt::WA_Hover, true);                 ///< 启用悬停事件

    // @note 隐藏刷新遮罩
    m_refreshMask->hide();
    this->m_refreshMask->setParent(ui->stackedWidget); ///< 设置遮罩父对象
    connect(this->m_refreshMask.get(),
            &RefreshMask::loadingFinished,
            [this](const QString &message) {
                if (!message.isEmpty()) {
                    m_snackbar->addMessage(message); ///< 显示加载完成提示
                    m_snackbar->show();
                }
            }); ///< 连接加载完成信号

    // @note 设置消息提示条
    m_snackbar->setParent(ui->stackedWidget);                   ///< 设置提示条父对象
    m_snackbar->setAutoHideDuration(1500);                      ///< 设置自动隐藏时间
    m_snackbar->setBackgroundColor(QColor(132, 202, 192, 200)); ///< 设置背景颜色
    m_snackbar->setStyleSheet("border-radius: 10px;");          ///< 设置圆角样式

    // @note 设置歌词界面 TODO

    initStackedWidget(); ///< 初始化堆栈窗口
    initMenu();          ///< 初始化菜单

    // 初始化搜索结果界面并添加到堆栈窗口
    this->m_searchResultWidget = std::make_unique<SearchResultWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_searchResultWidget.get()); ///< 将搜索结果界面添加到堆栈窗口
    connect(m_searchResultWidget.get(),
            &SearchResultWidget::playMusic,
            this,
            &KuGouClient::onSearchResultMusicPlay); ///< 连接查找更多音乐信号
    connect(m_searchResultWidget.get(),
            &SearchResultWidget::cancelLoopPlay,
            this,
            [this] {
                if (m_isSingleCircle) {
                    onCircleBtnClicked();
                }
            });
    connect(m_searchResultWidget.get(),
            &SearchResultWidget::searchEnable,
            this,
            [this](bool enable) { ui->title_widget->onSetSearchEnable(enable); });

    connectTitleWidget(); ///< 连接标题栏
    connectPlayWidget();  ///< 连接播放栏
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化指定组件并连接跳转信号
 */
void KuGouClient::initStackedWidget()
{
    m_menuBtnGroup->setParent(ui->center_menu_widget);
    for (int i = 0; i < 17; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    ///< 默认构造下面两个界面的原因：由于图片解码后占用内存过高，导致切换时动态申请内存会造成卡顿，因此不采用懒加载
    m_pages[3]->layout()->addWidget(createPage(3)); // Default to RecommendForYou
    m_pages[11]->layout()->addWidget(createPage(11));

    ui->stackedWidget->setCurrentIndex(3);

    /// 都被titleWidget 转发回来了，无需再次连接
    /// connect(m_menuBtnGroup.get(), &QButtonGroup::idClicked, this,&KuGouClient::onSelectedWidget);
}

void KuGouClient::onSelectedWidget(const int &id)
{
    if (m_currentIdx == id)
        return;
    ///< 可以在此处筛选哪些界面不需要动态地创建删除
    if (id == 3 || id == 16 || id == 11) {
        ///< 已经缓存的直接显示
        m_isInitialized = true;
        ///< 删除还是要正常进行
        enableButton(false);
        QWidget *placeholder = m_pages[m_currentIdx];
        if (m_currentIdx != 16 && m_currentIdx != 3 && m_currentIdx != 11) {
            if (!placeholder) {
                qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
                enableButton(true);
                return;
            }
            QLayout *layout = placeholder->layout();
            if (!layout) {
                layout = new QVBoxLayout(placeholder);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setSpacing(0);
            } else {
                while (QLayoutItem *item = layout->takeAt(0)) {
                    if (QWidget *widget = item->widget()) {
                        // qDebug()<<"删除旧控件";
                        widget->deleteLater();
                    }
                    delete item;
                }

                switch (m_currentIdx) {
                case 0: m_live.reset();
                    break;
                case 1: m_listenBook.reset();
                    break;
                case 2: m_search.reset();
                    break;
                // case 3: m_recommendForYou.reset();break;
                case 4: m_musicRepository.reset();
                    break;
                case 5: m_channel.reset();
                    break;
                case 6: m_video.reset();
                    break;
                case 7: m_aiChat.reset();
                    break;
                case 8: m_songList.reset();
                    break;
                case 9: m_dailyRecommend.reset();
                    break;
                case 10: m_collection.reset();
                    break;
                // case 11: m_localDownload.reset();break;
                case 12: m_musicCloudDisk.reset();
                    break;
                case 13: m_purchasedMusic.reset();
                    break;
                case 14: m_recentlyPlayed.reset();
                    break;
                case 15: m_allMusic.reset();
                    break;
                default: break;
                }
            }
        }
        if (id == 16) {
            ui->stackedWidget->setCurrentWidget(this->m_searchResultWidget.get()); ///< 切换到搜索结果界面
            enableButton(true);
        } else
            ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;
        return;
    }
    if (id == 16) {
        m_isInitialized = true;
        m_currentIdx = id;
        return;
    }

    m_refreshMask->hideLoading("");
    m_snackbar->hide();
    m_isInitialized = false;
    /// qDebug()<<__LINE__ << " 设置按钮状态为禁用";
    enableButton(false);
    QWidget *placeholder = m_pages[m_currentIdx];
    if (m_currentIdx != 16 && m_currentIdx != 3 && m_currentIdx != 11) {
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }
        QLayout *layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem *item = layout->takeAt(0)) {
                if (QWidget *widget = item->widget()) {
                    // qDebug()<<"删除旧控件";
                    widget->deleteLater();
                }
                delete item;
            }

            switch (m_currentIdx) {
            case 0: m_live.reset();
                break;
            case 1: m_listenBook.reset();
                break;
            case 2: m_search.reset();
                break;
            // case 3: m_recommendForYou.reset();break;
            case 4: m_musicRepository.reset();
                break;
            case 5: m_channel.reset();
                break;
            case 6: m_video.reset();
                break;
            case 7: m_aiChat.reset();
                break;
            case 8: m_songList.reset();
                break;
            case 9: m_dailyRecommend.reset();
                break;
            case 10: m_collection.reset();
                break;
            // case 11: m_localDownload.reset();break;
            case 12: m_musicCloudDisk.reset();
                break;
            case 13: m_purchasedMusic.reset();
                break;
            case 14: m_recentlyPlayed.reset();
                break;
            case 15: m_allMusic.reset();
                break;
            default: break;
            }
        }
    }
    placeholder = m_pages[id];
    auto layout = placeholder->layout();

    QWidget *realPage = createPage(id);
    if (!realPage) {
        qWarning() << "[WARNING] Failed to create page at index:" << id;
    } else {
        layout->addWidget(realPage);
    }

    ui->stackedWidget->slideInIdx(id);

    m_currentIdx = id;
    STREAM_INFO() << "切换到界面 ID:" << id;
}

/**
 * @brief 初始化标题栏
 * @note 连接左侧菜单、堆栈切换、最大化和关于对话框信号
 */
void KuGouClient::connectTitleWidget()
{
    // @note 响应左侧菜单显示
    connect(ui->title_widget, &TitleWidget::leftMenuShow, this, &KuGouClient::onLeftMenuShow);
    // @note 响应堆栈窗口切换
    connect(ui->title_widget,
            &TitleWidget::currentStackChange,
            this,
            &KuGouClient::onTitleCurrentStackChange);
    // @note 响应关于对话框显示
    connect(ui->title_widget,
            &TitleWidget::showAboutDialog,
            this,
            [this] {
                MainWindow::onShowAboutDialog(true); ///< 显示关于对话框
            });
    // @note 响应刷新窗口
    connect(ui->title_widget,
            &TitleWidget::refresh,
            this,
            [this] {
                this->m_refreshMask->showLoading(); ///< 显示加载遮罩
                this->m_refreshMask->raise();       ///< 提升遮罩层级
            });
    // @note 搜索项触发
    connect(ui->title_widget,
            &TitleWidget::suggestionClicked,
            this,
            &KuGouClient::handleSuggestBoxSuggestionClicked);
    connect(ui->title_widget,
            &TitleWidget::searchTextReturnPressed,
            this,
            &KuGouClient::handleSuggestBoxSuggestionClicked);
    // @note 退出当前账号，即切换账户
    connect(ui->title_widget, &TitleWidget::logOut, this, &KuGouClient::logOut);
}

/**
 * @brief 初始化播放栏
 * @note 设置图标、快捷键、工具提示和封面事件
 */
void KuGouClient::connectPlayWidget()
{
    // @note 设置快捷键
    new QShortcut(QKeySequence("Space"), this, SLOT(onKeyPause())); ///< 空格键暂停/播放
    new QShortcut(QKeySequence("Right"), this, SLOT(onKeyRight())); ///< 右箭头快进
    new QShortcut(QKeySequence("Left"), this, SLOT(onKeyLeft()));   ///< 左箭头快退
    connect(this->m_player,
            &VideoPlayer::positionChanged,
            ui->play_widget,
            &PlayWidget::onSliderPositionChanged);
    connect(this->m_player,
            &VideoPlayer::positionChanged,
            this,
            [this](const int &pos) {
                //qDebug() << "播放器位置变化：" << pos;
                //改变预览歌词时的滚动状态
                if (m_lyricWidget->isLyricValid()) {
                    m_lyricWidget->setViewerHighlightLineLyricAtPos(pos);
                }
            });

    connect(this->m_player,
            &VideoPlayer::durationChanged,
            ui->play_widget,
            &PlayWidget::updateSliderRange); ///< 连接时长变化信号

    connect(this->m_player,
            &VideoPlayer::pictureFound,
            ui->play_widget,
            &PlayWidget::onCoverChanged); ///< 连接封面图片发现信号
    connect(this->m_player,
            &VideoPlayer::pictureFound,
            this,
            [this](const QPixmap &pixmap) {
                if (!pixmap.isNull())
                    m_lyricWidget->AlbumImageChanged(pixmap);
                else {
                    m_lyricWidget->setToDefaultAlbumImage();
                }
            });

    connect(this->m_player,
            &VideoPlayer::titleFound,
            ui->play_widget,
            &PlayWidget::onSongNameChanged); ///< 连接歌曲标题发现信号
    connect(this->m_player,
            &VideoPlayer::titleFound,
            this,
            [this](const QString &title) {
                qDebug() << "标题：" << title;
                if (!title.isEmpty()) {
                    m_lyricWidget->setMusicTitle(title);
                }
            });

    connect(this->m_player,
            &VideoPlayer::artistFound,
            ui->play_widget,
            &PlayWidget::onSingerNameChanged);
    connect(this->m_player,
            &VideoPlayer::artistFound,
            this,
            [this](const QString &singer) {
                qDebug() << "歌手：" << singer;
                if (!singer.isEmpty())
                    m_lyricWidget->setMusicSinger(singer);
            });

    connect(this->m_player,
            &VideoPlayer::audioPlay,
            ui->play_widget,
            &PlayWidget::onAudioPlay);
    connect(this->m_player,
            &VideoPlayer::audioPlay,
            this,
            [this] {
                m_lyricWidget->playPhonograph();
            });

    connect(this->m_player,
            &VideoPlayer::audioPause,
            ui->play_widget,
            &PlayWidget::onAudioPause);

    connect(this->m_player,
            &VideoPlayer::audioPause,
            this,
            [this] {
                m_lyricWidget->stopPhonograph();
            });

    mediaStatusConnection = connect(this->m_player,
                                    &VideoPlayer::audioFinish,
                                    this,
                                    [this] {
                                        // @note 未使用，保留用于调试
                                        // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
                                        ui->play_widget->setPlayPauseIcon(false);

                                        // @note 未使用，保留用于调试
                                        // qDebug() << __LINE__ << "正常结束";
                                        // qDebug() << __LINE__ << "当前界面下标：" << ui->stackedWidget->currentIndex();
                                        if (ui->stackedWidget->currentIndex() == static_cast<int>(
                                                TitleWidget::StackType::LocalDownload) &&
                                            this->m_localDownload) {
                                            // qDebug() << "通知本地下载界面播放结束";
                                            ///< 通知本地下载组件
                                            this->m_localDownload->audioFinished();
                                        }
                                        if (ui->stackedWidget->currentWidget() ==
                                            static_cast<QWidget *>(this->
                                                m_searchResultWidget.get()) &&
                                            this->m_searchResultWidget) {
                                            ///< 通知搜索结果组件
                                            // qDebug() << "通知搜索结果界面播放结束";
                                            this->m_searchResultWidget->onAudioFinished();
                                        }
                                    }); ///< 连接正常播放结束信号
    connect(this->m_player,
            &VideoPlayer::errorOccur,
            this,
            [this](const QString &msg) {
                ElaMessageBar::error(ElaMessageBarType::BottomRight,
                                     "Error",
                                     msg,
                                     2000,
                                     this->window()); ///< 显示错误提示
            });                                       ///< 连接错误信号

    connect(ui->play_widget,
            &PlayWidget::volumeChange,
            this,
            [this](int value) {
                if (this->m_player) {
                    //qDebug() << __LINE__ << "音量变化：" << value;
                    this->m_player->setVolume(value / 100.0); ///< 设置播放器音量
                }
            }); ///< 连接音量变化信号

    connect(ui->play_widget,
            &PlayWidget::sliderPressed,
            this,
            [this](const int &value) {
                ///< 鼠标按下事件
                if (this->m_player->state() == VideoPlayer::State::Stop) {
                    this->m_player->replay(true); ///< 重新播放
                }
                this->m_player->pause();     ///< 暂停播放
                this->m_player->seek(value); ///< 跳转到指定位置
                this->m_player->play();      ///< 继续播放
            });
    connect(m_lyricWidget.get(),
            &LyricWidget::jumpToTime,
            this,
            [this](const int &pos) {
                ///< 鼠标按下事件
                if (this->m_player->state() == VideoPlayer::State::Stop) {
                    this->m_player->replay(true); ///< 重新播放
                }
                this->m_player->pause();   ///< 暂停播放
                this->m_player->seek(pos); ///< 跳转到指定位置
                this->m_player->play();    ///< 继续播放
            });

    connect(ui->play_widget,
            &PlayWidget::sliderReleased,
            this,
            &KuGouClient::updateProcess);

    connect(ui->play_widget,
            &PlayWidget::clickedPlayPauseBtn,
            this,
            [this] {
                if (this->m_player->state() == VideoPlayer::State::Playing) {
                    this->m_player->pause(); ///< 暂停播放
                    ui->play_widget->setPlayPauseIcon(false);
                } else if (this->m_player->state() == VideoPlayer::State::Pause) {
                    this->m_player->play(); ///< 继续播放
                    ui->play_widget->setPlayPauseIcon(true);
                } else if (this->m_player->state() == VideoPlayer::State::Stop) {
                    this->m_player->replay(true);            ///< 重新播放
                    ui->play_widget->setPlayPauseIcon(true); ///< 设置播放图标
                }
            });

    connect(ui->play_widget,
            &PlayWidget::clickedCircleBtn,
            this,
            &KuGouClient::onCircleBtnClicked);

    connect(ui->play_widget, &PlayWidget::clickedPreBtn, this, &KuGouClient::onPreBtnClicked);

    connect(ui->play_widget,
            &PlayWidget::clickedNextBtn,
            this,
            &KuGouClient::onNextBtnClicked); ///< 连接下一首按钮点击信号

    connect(ui->play_widget,
            &PlayWidget::doubleClicked,
            this,
            [this] {
                ui->title_widget->setMaxScreen();
            });
    connect(ui->play_widget,
            &PlayWidget::showLyricWidget,
            this,
            [this] {
                m_lyricWidget->toggleAnimation();
                if (m_lyricWidget->isVisible()) {
                    m_lyricWidget->raise();
                    ui->play_widget->raise();
                    ui->play_widget->setTextColor(true);
                } else {
                    ui->play_widget->lower(); // 恢复到底层
                    ui->play_widget->setTextColor(false);
                }
            });
}

/**
 * @brief 初始化菜单
 * @note 设置 14 个互斥按钮的图标和分组
 */
void KuGouClient::initMenu()
{
    ui->menu_scrollAreaWidgetContents->setAttribute(Qt::WA_TranslucentBackground);
    ui->menu_scrollAreaWidgetContents->setAutoFillBackground(false);

    this->m_menuBtnGroup->setParent(ui->center_menu_widget);

    ui->recommend_you_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/recommend.svg")));
    ///< 设置推荐图标
    ui->music_repository_toolButton->setIcon(
        QIcon(QStringLiteral(":/Res/window/music-library.svg"))); ///< 设置音乐库图标
    ui->channel_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/my-channel.svg")));
    ///< 设置频道图标
    ui->video_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/video.svg"))); ///< 设置视频图标
    ui->live_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/live.svg")));   ///< 设置直播图标
    ui->ai_chat_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/ai-chat.svg")));
    ///< 设置 AI 聊天图标
    ui->song_list_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/song-list.svg")));
    ///< 设置歌单图标
    ui->daily_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/daily.svg")));
    ///< 设置每日推荐图标
    ui->my_collection_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/collect.svg")));
    ///< 设置收藏图标
    ui->local_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg")));
    ///< 设置本地下载图标
    ui->music_cloud_disk_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/cloud.svg")));
    ///< 设置云盘图标
    ui->purchased_music_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/bought.svg")));
    ///< 设置已购音乐图标
    ui->recently_played_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/history.svg")));
    ///< 设置最近播放图标
    ui->all_music_toolButton->setIcon(QIcon(QStringLiteral(":/Res/titlebar/menu-black.svg")));
    ///< 设置全部音乐图标
    // @note 设置互斥按钮组
    this->m_menuBtnGroup->addButton(ui->recommend_you_toolButton, 3);     ///< 添加推荐按钮
    this->m_menuBtnGroup->addButton(ui->music_repository_toolButton, 4);  ///< 添加音乐库按钮
    this->m_menuBtnGroup->addButton(ui->channel_toolButton, 5);           ///< 添加频道按钮
    this->m_menuBtnGroup->addButton(ui->video_toolButton, 6);             ///< 添加视频按钮
    this->m_menuBtnGroup->addButton(ui->live_toolButton, 0);              ///< 添加直播按钮
    this->m_menuBtnGroup->addButton(ui->ai_chat_toolButton, 7);           ///< 添加 AI 聊天按钮
    this->m_menuBtnGroup->addButton(ui->song_list_toolButton, 8);         ///< 添加歌单按钮
    this->m_menuBtnGroup->addButton(ui->daily_recommend_toolButton, 9);   ///< 添加每日推荐按钮
    this->m_menuBtnGroup->addButton(ui->my_collection_toolButton, 10);    ///< 添加收藏按钮
    this->m_menuBtnGroup->addButton(ui->local_download_toolButton, 11);   ///< 添加本地下载按钮
    this->m_menuBtnGroup->addButton(ui->music_cloud_disk_toolButton, 12); ///< 添加云盘按钮
    this->m_menuBtnGroup->addButton(ui->purchased_music_toolButton, 13);  ///< 添加已购音乐按钮
    this->m_menuBtnGroup->addButton(ui->recently_played_toolButton, 14);  ///< 添加最近播放按钮
    this->m_menuBtnGroup->addButton(ui->all_music_toolButton, 15);        ///< 添加全部音乐按钮
    this->m_menuBtnGroup->setExclusive(true);                             ///< 设置互斥
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 * @note 控制 14 个菜单按钮和标题栏的交互
 */
void KuGouClient::enableButton(const bool &flag)
{
    QToolButton *buttons[] = {ui->recommend_you_toolButton,
                              ui->music_repository_toolButton,
                              ui->song_list_toolButton,
                              ui->channel_toolButton,
                              ui->video_toolButton,
                              ui->live_toolButton,
                              ui->ai_chat_toolButton,
                              ui->daily_recommend_toolButton,
                              ui->my_collection_toolButton,
                              ui->local_download_toolButton,
                              ui->music_cloud_disk_toolButton,
                              ui->purchased_music_toolButton,
                              ui->recently_played_toolButton,
                              ui->all_music_toolButton};
    for (auto *button : buttons) {
        button->setEnabled(flag);
    }

    ui->title_widget->setEnableChange(flag);
    ui->title_widget->setEnableTitleButton(flag);
}

QWidget *KuGouClient::createPage(int id)
{
    QWidget *page = nullptr;
    switch (id) {
    case 0: // Live
        if (!m_live) {
            m_live = std::make_unique<Live>(ui->stackedWidget);
            connect(
                m_live.get(),
                &Live::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_live.get();
        page = m_live.get();
        break;
    case 1: // ListenBook
        if (!m_listenBook) {
            m_listenBook = std::make_unique<ListenBook>(ui->stackedWidget);
            connect(
                m_listenBook.get(),
                &ListenBook::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_listenBook.get();
        page = m_listenBook.get();
        break;
    case 2: // Search
        if (!m_search) {
            m_search = std::make_unique<Search>(ui->stackedWidget);
            connect(
                m_search.get(),
                &Search::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_search.get();
        page = m_search.get();
        break;
    case 3: // RecommendForYou
        if (!m_recommendForYou) {
            m_recommendForYou = std::make_unique<RecommendForYou>(ui->stackedWidget);
            connect(
                m_recommendForYou.get(),
                &RecommendForYou::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_recommendForYou.get();
        page = m_recommendForYou.get();
        break;
    case 4: // MusicRepository
        if (!m_musicRepository) {
            m_musicRepository = std::make_unique<MusicRepository>(ui->stackedWidget);
            connect(
                m_musicRepository.get(),
                &MusicRepository::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_musicRepository.get();
        page = m_musicRepository.get();
        break;
    case 5: // Channel
        if (!m_channel) {
            m_channel = std::make_unique<Channel>(ui->stackedWidget);
            connect(
                m_channel.get(),
                &Channel::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_channel.get();
        page = m_channel.get();
        break;
    case 6: // Video
        if (!m_video) {
            m_video = std::make_unique<Video>(ui->stackedWidget);
            connect(
                m_video.get(),
                &Video::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_video.get();
        page = m_video.get();
        break;
    case 7: // AiChat
        if (!m_aiChat) {
            m_aiChat = std::make_unique<AiChat>(ui->stackedWidget);
            connect(
                m_aiChat.get(),
                &AiChat::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_aiChat.get();
        page = m_aiChat.get();
        break;
    case 8: // SongList
        if (!m_songList) {
            m_songList = std::make_unique<SongList>(ui->stackedWidget);
            connect(
                m_songList.get(),
                &SongList::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_songList.get();
        page = m_songList.get();
        break;
    case 9: // DailyRecommend
        if (!m_dailyRecommend) {
            m_dailyRecommend = std::make_unique<DailyRecommend>(ui->stackedWidget);
            connect(
                m_dailyRecommend.get(),
                &DailyRecommend::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_dailyRecommend.get();
        page = m_dailyRecommend.get();
        break;
    case 10: // MyCollection
        if (!m_collection) {
            m_collection = std::make_unique<MyCollection>(ui->stackedWidget);
            connect(m_collection.get(),
                    &MyCollection::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(
                m_collection.get(),
                &MyCollection::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_collection.get();
        page = m_collection.get();
        break;
    case 11: // LocalDownload
        if (!m_localDownload) {
            m_localDownload = std::make_unique<LocalDownload>(ui->stackedWidget);
            connect(m_localDownload.get(),
                    &LocalDownload::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(m_localDownload.get(),
                    &LocalDownload::playMusic,
                    this,
                    &KuGouClient::onPlayLocalMusic);
            connect(m_localDownload.get(),
                    &LocalDownload::cancelLoopPlay,
                    this,
                    [this] {
                        if (m_isSingleCircle) {
                            onCircleBtnClicked();
                        }
                    });
            connect(
                m_localDownload.get(),
                &LocalDownload::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_localDownload.get();
        page = m_localDownload.get();
        break;
    case 12: // MusicCloudDisk
        if (!m_musicCloudDisk) {
            m_musicCloudDisk = std::make_unique<MusicCloudDisk>(ui->stackedWidget);
            connect(m_musicCloudDisk.get(),
                    &MusicCloudDisk::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(
                m_musicCloudDisk.get(),
                &MusicCloudDisk::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_musicCloudDisk.get();
        page = m_musicCloudDisk.get();
        break;
    case 13: // PurchasedMusic
        if (!m_purchasedMusic) {
            m_purchasedMusic = std::make_unique<PurchasedMusic>(ui->stackedWidget);
            connect(m_purchasedMusic.get(),
                    &PurchasedMusic::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(
                m_purchasedMusic.get(),
                &PurchasedMusic::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_purchasedMusic.get();
        page = m_purchasedMusic.get();
        break;
    case 14: // RecentlyPlayed
        if (!m_recentlyPlayed) {
            m_recentlyPlayed = std::make_unique<RecentlyPlayed>(ui->stackedWidget);
            connect(m_recentlyPlayed.get(),
                    &RecentlyPlayed::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(m_recentlyPlayed.get(),
                    &RecentlyPlayed::find_more_channel,
                    this,
                    [this] { ui->channel_toolButton->click(); });
            connect(
                m_recentlyPlayed.get(),
                &RecentlyPlayed::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_recentlyPlayed.get();
        page = m_recentlyPlayed.get();
        break;
    case 15: // AllMusic
        if (!m_allMusic) {
            m_allMusic = std::make_unique<AllMusic>(ui->stackedWidget);
            connect(m_allMusic.get(),
                    &AllMusic::find_more_music,
                    this,
                    [this] { ui->music_repository_toolButton->click(); });
            connect(
                m_allMusic.get(),
                &AllMusic::initialized,
                this,
                [this] {
                    this->m_isInitialized = true;
                    enableButton(true);
                },
                Qt::QueuedConnection);
        } else
            return m_allMusic.get();
        page = m_allMusic.get();
        break;
    default:
        qWarning() << "[WARNING] Invalid page ID:" << id;
        return nullptr;
    }
    return page;
}

// 实现文件
void KuGouClient::setupButtonConnections()
{
    // 建立按钮与对应函数的映射关系
    QMap<QToolButton *, void (TitleWidget::*)()> buttonMap = {
        {ui->recommend_you_toolButton, &TitleWidget::onLeftMenu_recommend_clicked},
        {ui->music_repository_toolButton, &TitleWidget::onLeftMenu_musicRepository_clicked},
        {ui->channel_toolButton, &TitleWidget::onLeftMenu_channel_clicked},
        {ui->video_toolButton, &TitleWidget::onLeftMenu_video_clicked},
        {ui->live_toolButton, &TitleWidget::onLeftMenu_live_clicked},
        {ui->ai_chat_toolButton, &TitleWidget::onLeftMenu_ai_chat_clicked},
        {ui->song_list_toolButton, &TitleWidget::onLeftMenu_songList_clicked},
        {ui->daily_recommend_toolButton, &TitleWidget::onLeftMenu_dailyRecommend_clicked},
        {ui->my_collection_toolButton, &TitleWidget::onLeftMenu_collection_clicked},
        {ui->local_download_toolButton, &TitleWidget::onLeftMenu_localDownload_clicked},
        {ui->music_cloud_disk_toolButton, &TitleWidget::onLeftMenu_musicCloudDisk_clicked},
        {ui->purchased_music_toolButton, &TitleWidget::onLeftMenu_purchasedMusic_clicked},
        {ui->recently_played_toolButton, &TitleWidget::onLeftMenu_recentlyPlayed_clicked},
        {ui->all_music_toolButton, &TitleWidget::onLeftMenu_allMusic_clicked}};

    // 统一连接所有按钮信号
    for (auto it = buttonMap.begin(); it != buttonMap.end(); ++it) {
        connect(it.key(),
                &QToolButton::clicked,
                this,
                [this, func = it.value()] {
                    (ui->title_widget->*func)(); // 调用对应的成员函数
                });
    }
}

void KuGouClient::mousePressEvent(QMouseEvent *ev)
{
    MainWindow::mousePressEvent(ev);

    if (ev->button() == Qt::LeftButton) {
        this->m_pressPos = ev->pos(); ///< 记录按下位置
    }
}

/**
 * @brief 鼠标移动事件
 * @param event 鼠标事件
 * @note 处理窗口拖动和最大化还原
 */
void KuGouClient::mouseMoveEvent(QMouseEvent *event)
{
    MainWindow::mouseMoveEvent(event);                          ///< 调用父类处理
    point_offset = event->globalPosition().toPoint() - mousePs; ///< 计算鼠标偏移量

    if (isPress) {
        if (mouse_press_region == kMousePositionMid) {
            if (ui->title_widget->geometry().contains(m_pressPos) || ui->play_widget->geometry().
                contains(m_pressPos)) {
                move(windowsLastPs + point_offset);
            }
        }
    }
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 更新最大化状态、角标位置和文本省略
 */
void KuGouClient::resizeEvent(QResizeEvent *event)
{
    MainWindow::resizeEvent(event); ///< 调用父类处理

    // @note 同步刷新遮罩大小
    auto rect = ui->stackedWidget->geometry();
    rect.setLeft(5);
    rect.setRight(rect.width() - 6);
    this->m_refreshMask->setGeometry(rect); ///< 设置遮罩几何形状

    m_lyricWidget->resize(size());
}

/**
 * @brief 事件处理
 * @param event 事件
 * @return 是否处理事件
 * @note 处理鼠标移动事件
 */
bool KuGouClient::event(QEvent *event)
{
    if (QEvent::HoverMove == event->type()) {
        ///< 鼠标移动事件
        auto ev = static_cast<QMouseEvent *>(event);
        this->mouseMoveEvent(ev); ///< 处理鼠标移动
        return true;
    }
    return MainWindow::event(event); ///< 调用父类处理
}

/**
 * @brief 处理suggestBox选中项槽函数
 * @note 切换搜索结果界面
 */
void KuGouClient::handleSuggestBoxSuggestionClicked(const QString &suggestText,
                                                    const QVariantMap &suggestData)
{
    if (suggestText.isEmpty() || suggestText.trimmed().isEmpty() || suggestText.isNull()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               "Empty Suggestion",
                               2000,
                               this->window());
        ///< 显示播放失败提示
        return;
    }
    ///< 之前的还没有加载完成就等候
    if (this->m_refreshMask->isLoading())
        return;
    ///< 切换到音乐界面
    onLeftMenuShow(true);
    qDebug() << "选中：" << suggestText << " 附带数据：" << suggestData;
    onSelectedWidget(16);

    this->m_searchResultWidget->handleSuggestion(suggestText);
}

/**
 * @brief 更新播放进度
 * @note 根据进度条位置调整播放器进度
 */
void KuGouClient::updateProcess(const int &sliderValue, const int &maxSliderValue)
{
    qint64 position = sliderValue * this->m_player->getTotalTime() / maxSliderValue;

    ///< 计算播放位置
    // @note 未使用，保留用于调试
    // qDebug() << "position : " << position;
    this->m_player->pause();        ///< 暂停播放
    this->m_player->seek(position); ///< 跳转到指定位置
    this->m_player->play();         ///< 继续播放
}

/**
 * @brief 空格键快捷键槽函数
 * @note 切换播放/暂停状态
 */
void KuGouClient::onKeyPause()
{
    if (this->m_player->state() == VideoPlayer::State::Playing) {
        this->m_player->pause(); ///< 暂停播放
    } else {
        if (!this->m_player->getMusicPath().isEmpty())
            this->m_player->play(); ///< 继续播放
    }
}

/**
 * @brief 左箭头快捷键槽函数
 * @note 快退 5 秒
 */
void KuGouClient::onKeyLeft()
{
    // @note 未使用，保留用于调试
    // qDebug() << "getCurrentTime() : " << this->m_player->getCurrentTime();
    this->m_player->seek(this->m_player->getCurrentTime() * 1000 - 5000000); ///< 快退 5 秒
    if (this->m_player->state() == VideoPlayer::State::Pause) {
        this->m_player->play(); ///< 继续播放
    }
}

/**
 * @brief 右箭头快捷键槽函数
 * @note 快进 5 秒
 */
void KuGouClient::onKeyRight()
{
    // @note 未使用，保留用于调试
    // qDebug() << "getCurrentTime() : " << this->m_player->getCurrentTime();
    this->m_player->seek(this->m_player->getCurrentTime() * 1000 + 5000000); ///< 快进 5 秒
    if (this->m_player->state() == VideoPlayer::State::Pause) {
        this->m_player->play(); ///< 继续播放
    }
}

/**
 * @brief 标题栏堆栈切换槽函数
 * @param index 目标页面索引
 * @param slide 是否滑动切换
 * @note 更新堆栈页面和按钮状态
 */
void KuGouClient::onTitleCurrentStackChange(const int &index)
{
    if (m_currentIdx == index)
        return;
    m_isInitialized = false;
    enableButton(false); ///< 禁用按钮
    onSelectedWidget(index);
    m_currentIdx = index;
    this->m_refreshMask->hideLoading(""); ///< 隐藏刷新遮罩
    this->m_snackbar->hide();             ///< 隐藏消息提示条

    ui->stackedWidget->slideInIdx(index); ///< 滑动切换页面

    switch (index) {
    case 3: ui->recommend_you_toolButton->setChecked(true);
        break; ///< 推荐页面
    case 4: ui->music_repository_toolButton->setChecked(true);
        break; ///< 音乐库页面
    case 5: ui->channel_toolButton->setChecked(true);
        break; ///< 频道页面
    case 6: ui->video_toolButton->setChecked(true);
        break; ///< 视频页面
    case 7: ui->ai_chat_toolButton->setChecked(true);
        break; ///< AI 聊天页面
    case 8: ui->song_list_toolButton->setChecked(true);
        break; ///< 歌单页面
    case 9: ui->daily_recommend_toolButton->setChecked(true);
        break; ///< 每日推荐页面
    case 10: ui->my_collection_toolButton->setChecked(true);
        break; ///< 收藏页面
    case 11: ui->local_download_toolButton->setChecked(true);
        break; ///< 本地下载页面
    case 12: ui->music_cloud_disk_toolButton->setChecked(true);
        break; ///< 云盘页面
    case 13: ui->purchased_music_toolButton->setChecked(true);
        break; ///< 已购音乐页面
    case 14: ui->recently_played_toolButton->setChecked(true);
        break; ///< 最近播放页面
    case 15: ui->all_music_toolButton->setChecked(true);
        break; ///< 全部音乐页面
    default: break;
    }
}

/**
 * @brief 左侧菜单显示槽函数
 * @param flag 是否显示
 * @note 显示或隐藏菜单滚动区域
 */
void KuGouClient::onLeftMenuShow(const bool &flag) const
{
    if (flag) {
        ui->menu_scrollArea->show(); ///< 显示菜单
    } else {
        ui->menu_scrollArea->hide(); ///< 隐藏菜单
    }
}

/**
 * @brief 播放本地音乐
 * @param localPath 本地音乐路径
 * @note 检查文件存在并启动播放
 */
void KuGouClient::onPlayLocalMusic(const QString &localPath)
{
    // @note 未使用，保留用于调试
    // qDebug() << "播放：" << localPath;
    if (!QFile::exists(localPath)) {
        // @note 未使用，保留用于调试
        // qDebug() << "File does not exist:" << localPath;
        return;
    }
    if (!m_player->startPlay(localPath.toStdString())) {
        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                             "Error",
                             "Failed to start playback",
                             2000,
                             this->window()); ///< 显示播放失败提示
    }
}

void KuGouClient::onSearchResultMusicPlay(const MusicItemWidget *item)
{
    if (!m_player->startPlay(item->m_information.netUrl.toStdString())) {
        ElaMessageBar::error(ElaMessageBarType::BottomRight,
                             "Error",
                             "Failed to start playback",
                             2000,
                             this->window()); ///< 显示播放失败提示
    }
    //qDebug() << "设置封面：" << item->m_information.cover;
    ui->play_widget->setCover(item->m_information.cover);
    if (!item->m_information.cover.isNull()) {
        m_lyricWidget->AlbumImageChanged(item->m_information.cover);
    } else {
        m_lyricWidget->setToDefaultAlbumImage();
    }

    ui->play_widget->setSongName(item->m_information.songName);
    ui->play_widget->setSingerName(item->m_information.singer);

    m_lyricWidget->setMusicTitle(item->m_information.songName);
    m_lyricWidget->setMusicSinger(item->m_information.singer);
}

void KuGouClient::onTrayIconNoVolume(const bool &flag)
{
    ui->play_widget->setNoVolume(flag);
}

void KuGouClient::onTrayIconExit()
{
    ui->title_widget->on_close_toolButton_clicked();
}

/**
 * @brief 循环播放按钮点击槽函数
 * @note 切换单曲循环状态
 */
void KuGouClient::onCircleBtnClicked()
{
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    m_isSingleCircle = !m_isSingleCircle; ///< 切换循环状态
    ui->play_widget->changeCircleToolButtonState(m_isSingleCircle);
    if (m_isSingleCircle) {
        ///< 设置单曲循环样式
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection); ///< 断开原有连接
            mediaStatusConnection = connect(this->m_player,
                                            &VideoPlayer::audioFinish,
                                            this,
                                            [this] {
                                                // @note 未使用，保留用于调试
                                                // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
                                                ui->play_widget->setPlayPauseIcon(false);

                                                // @note 未使用，保留用于调试
                                                // qDebug() << __LINE__ << " 重新播放";
                                                this->m_player->replay(true); ///< 重新播放
                                            });                               ///< 连接单曲循环信号
        } else {
            // @note 未使用，保留用于调试
            qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty"; ///< 记录警告日志
        }
    } else {
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection); ///< 断开原有连接
            mediaStatusConnection = connect(this->m_player,
                                            &VideoPlayer::audioFinish,
                                            this,
                                            [this] {
                                                // @note 未使用，保留用于调试
                                                // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
                                                ui->play_widget->setPlayPauseIcon(false);

                                                // @note 未使用，保留用于调试
                                                // qDebug() << __LINE__ << "正常结束";
                                                // qDebug() << __LINE__ << "当前界面下标：" << ui->stackedWidget->currentIndex();
                                                if (ui->stackedWidget->currentIndex() == static_cast
                                                    <int>(
                                                        TitleWidget::StackType::LocalDownload) &&
                                                    this->m_localDownload) {
                                                    // qDebug() << "通知本地下载界面播放结束";
                                                    ///< 通知本地下载组件
                                                    this->m_localDownload->audioFinished();
                                                }
                                                if (ui->stackedWidget->currentWidget() ==
                                                    static_cast<QWidget *>(this->
                                                        m_searchResultWidget.get()) &&
                                                    this->m_searchResultWidget) {
                                                    ///< 通知搜索结果组件
                                                    // qDebug() << "通知搜索结果界面播放结束";
                                                    this->m_searchResultWidget->onAudioFinished();
                                                }
                                            }); ///< 连接正常播放结束信号
        } else {
            // @note 未使用，保留用于调试
            qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty"; ///< 记录警告日志
        }
    }
}

/**
 * @brief 上一首按钮点击槽函数
 * @note 播放上一首本地歌曲
 */
void KuGouClient::onPreBtnClicked()
{
    ///< 点击下一首/上一首时需要判断当前是否播放过音乐，如果没有播放过音乐，需要显示无音乐提示
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    if (m_player->getMusicPath().startsWith("http://") || m_player->getMusicPath().
        startsWith("https://")) {
        m_searchResultWidget->playPreviousMusic();
    } else if (m_localDownload) {
        this->m_localDownload->playLocalSongPrevSong(); ///< 播放上一首
    }
}

/**
 * @brief 下一首按钮点击槽函数
 * @note 播放下一首本地歌曲
 */
void KuGouClient::onNextBtnClicked()
{
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,
                               "Warning",
                               QStringLiteral("暂无可播放音乐"),
                               1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    if (m_player->getMusicPath().startsWith("http://") || m_player->getMusicPath().
        startsWith("https://")) {
        m_searchResultWidget->playNextMusic();
    } else if (m_localDownload) {
        this->m_localDownload->playLocalSongNextSong(); ///< 播放下一首
    }
}