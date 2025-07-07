/**
 * @file KuGouApp.cpp
 * @brief 实现 KuGouApp 类，管理酷狗音乐主界面
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "KuGouClient.h"
#include "ui_KuGouClient.h"
#include "logger.hpp"
#include "RippleButton.h"
#include "RefreshMask.h"
#include "qtmaterialsnackbar.h"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"
#include "SpeedDialog/SpeedDialog.h"
#include "MyScrollArea.h"
#include "Async.h"

#include <QPainterPath>
#include <QPainter>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QPoint>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSizeGrip>
#include <QPropertyAnimation>
#include <QShortcut>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 生成圆角图片
 * @param src 源图片
 * @param size 目标大小
 * @param radius 圆角半径
 * @return 处理后的圆角图片
 * @note 使用抗锯齿和裁剪路径生成圆角效果
 */
QPixmap roundedPixmap(const QPixmap &src, QSize size, int radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放图片
    QPixmap dest(size);
    dest.fill(Qt::transparent);                         ///< 设置透明背景

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);      ///< 启用抗锯齿
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius); ///< 创建圆角矩形路径
    painter.setClipPath(path);                          ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                   ///< 绘制图片

    return dest;
}

/**
 * @brief 构造函数，初始化酷狗音乐主界面
 * @param parent 父窗口指针，默认为 nullptr
 */
KuGouClient::KuGouClient(MainWindow *parent)
    : MainWindow(parent)
    , ui(new Ui::KuGouClient)
    , m_menuBtnGroup(std::make_unique<QButtonGroup>(this))          ///< 初始化菜单按钮组
    , m_sizeGrip(std::make_unique<QSizeGrip>(this))                ///< 初始化窗口大小调整角标
    , m_animation(std::make_unique<QPropertyAnimation>(this, "geometry")) ///< 初始化窗口动画
    , m_refreshMask(std::make_unique<RefreshMask>())               ///< 初始化刷新遮罩
    , m_snackbar(std::make_unique<QtMaterialSnackbar>())           ///< 初始化消息提示条
{
    {
        // 初始化日志
        if (!mylog::logger::get().init("../logs/main.log"))
        {
            qWarning()<<"客户端日志初始化失败";
            return; ///< 日志初始化失败，退出
        }
        mylog::logger::get().set_level(spdlog::level::info); ///< 设置日志级别为 info

        // 三种日志输出方式
        STREAM_INFO() << "STREAM_INFO : 客户端初始化（info）" << "成功"; ///< 流式日志
        PRINT_INFO("PRINT_INFO : 客户端初始化（info）%s", "成功");       ///< 格式化日志
        LOG_INFO("LOG_INFO : 客户端初始化（info）{}", "成功");           ///< 模板日志
    }
    // @note 初始化字体资源
    initFontRes();
    ui->setupUi(this);                                             ///< 设置 UI 布局

    QFile file(GET_CURRENT_DIR + QStringLiteral("/kugou.css"));   ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());                       ///< 应用样式表
    } else {
        // @note 未使用，保留用于调试
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";                    ///< 记录错误日志
        return;
    }
    initPlayer();                                                  ///< 初始化播放器
    initUi();                                                      ///< 初始化界面

    // @note 清除历史记录
    m_speedDialogState.reset();

    // @note 动画结束，恢复按钮可交互
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);                                            ///< 启用按钮
    ui->stackedWidget->setVerticalMode(true);                      ///< 设置堆栈窗口垂直滑动
    // @note 默认选中为你推荐页面
    ui->recommend_you_toolButton->clicked();
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源并关闭日志
 */
KuGouClient::~KuGouClient() {
    // @note 在 spdlog 静态变量销毁前关闭日志
    mylog::logger::get().shutdown();
    delete ui;  ///< 释放 UI 界面
}

/**
 * @brief 初始化播放器
 * @note 设置音量、静音和元数据信号连接
 */
void KuGouClient::initPlayer() {
    // @note 初始化播放器
    VideoPlayer::initPlayer();
    qRegisterMetaType<VideoPlayer::State>();                       ///< 注册播放器状态元类型
    // @note 设置事件处理
    this->m_player = new VideoPlayer(this);                        ///< 创建播放器
    m_player->setAbility(false, false, true, false);               ///< 设置播放器能力
    m_player->setVolume(0.3);                                      ///< 设置音量为 0.3
    m_player->setMute(false);                                      ///< 取消静音
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
}

/**
 * @brief 初始化字体资源
 * @note 加载五种字体资源
 */
void KuGouClient::initFontRes() {
    // 加载 dialog.ttf 字体
    auto fontId = QFontDatabase::addApplicationFont(":/Res/font/dialog.ttf"); ///< 加载对话字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //AaSongLiuKaiTi

    // 加载 ElaAwesome.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/ElaAwesome.ttf"); ///< 加载图标字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //ElaAwesome

    // 加载 qing-ning-you-yuan.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/qing-ning-you-yuan.ttf"); ///< 加载优圆字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //YouYuan

    // 加载 JetBrainsMonoNerdFont-Bold.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/JetBrainsMonoNerdFont-Bold.ttf"); ///< 加载代码字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //JetBrainsMono NF

    // 加载 chinese-simplify.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/chinese-simplify.ttf"); ///< 加载简体中文字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称    //dingliehuobanfont
}

/**
 * @brief 初始化界面
 * @note 初始化字体、窗口属性和子组件
 */
void KuGouClient::initUi() {
    this->setWindowIcon(QIcon(QStringLiteral(":/Res/window/windowIcon.png"))); ///< 设置窗口图标
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint); ///< 设置无边框和无阴影
    // @note 移动窗口到屏幕中央
    move(QGuiApplication::primaryScreen()->geometry().width() / 2 - this->width() / 2, 100);

    // @note 设置鼠标追踪
    this->setMouseTracking(true);
    ui->title_widget->setMouseTracking(true);
    ui->center_widget->setMouseTracking(true);
    ui->play_widget->setMouseTracking(true);
    // @note 设置窗口属性
    this->setAttribute(Qt::WA_TranslucentBackground, true);       ///< 设置透明背景
    this->setAttribute(Qt::WA_Hover, true);                       ///< 启用悬停事件

    // @note 隐藏刷新遮罩
    m_refreshMask->hide();
    this->m_refreshMask->setParent(ui->stackedWidget);            ///< 设置遮罩父对象
    connect(this->m_refreshMask.get(), &RefreshMask::loadingFinished, [this](const QString& message) {
        m_snackbar->addMessage(message);                         ///< 显示加载完成提示
        m_snackbar->show();
    });                                                           ///< 连接加载完成信号
    // @note 设置消息提示条
    m_snackbar->setParent(ui->stackedWidget);                     ///< 设置提示条父对象
    m_snackbar->setAutoHideDuration(1500);                        ///< 设置自动隐藏时间
    m_snackbar->setBackgroundColor(QColor(132, 202, 192, 200));   ///< 设置背景颜色
    m_snackbar->setStyleSheet("border-radius: 10px;");            ///< 设置圆角样式

    initTitleWidget(); ///< 初始化标题栏
    initStackedWidget(); ///< 初始化堆栈窗口
    initSearchResultWidget(); ///< 初始化搜索结果界面
    initPlayWidget(); ///< 初始化播放栏
    initMenu(); ///< 初始化菜单
    initCornerWidget(); ///< 初始化角标
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化指定组件并连接跳转信号
 */
void KuGouClient::initStackedWidget() {
    // @note 使用模板函数初始化组件
    {
        // @note 未使用，用于调试以减少编译时间，但可能会有不可预料的bug
        /*
         * // initComponent(m_live,0);
         * // initComponent(m_listenBook,1);
         * // initComponent(m_search,2);
         * initComponent(m_recommendForYou,3);
         * initComponent(m_musicRepository,4);
         * // initComponent(m_channel,5);
         * // initComponent(m_video,6);
         * // initComponent(m_aiChat,7);
         * // initComponent(m_songList,8);
         * // initComponent(m_dailyRecommend,9);
         * // initComponent(m_collection,10);
         * // initComponent(m_localDownload,11);
         * // initComponent(m_musicCloudDisk,12);
         * // initComponent(m_purchasedMusic,13);
         * // initComponent(m_recentlyPlayed,14);
         * // initComponent(m_allMusic,15);
         */
        initComponent(m_live,0);                               ///< 初始化直播界面
        initComponent(m_listenBook,1);                         ///< 初始化听书界面
        initComponent(m_search,2);                             ///< 初始化搜索界面
        initComponent(m_recommendForYou,3);                    ///< 初始化为你推荐界面
        initComponent(m_musicRepository,4);                    ///< 初始化乐库界面
        initComponent(m_channel,5);                            ///< 初始化频道界面
        initComponent(m_video,6);                              ///< 初始化视频界面
        initComponent(m_aiChat,7);                             ///< 初始化ai对话界面
        initComponent(m_songList,8);                           ///< 初始化歌单界面
        initComponent(m_dailyRecommend,9);                     ///< 初始化每日推荐界面
        initComponent(m_collection,10);                        ///< 初始化收藏界面
        initComponent(m_localDownload,11);                     ///< 初始化本地与下载界面
        initComponent(m_musicCloudDisk,12);                    ///< 初始化音乐云盘界面
        initComponent(m_purchasedMusic,13);                    ///< 初始化已购音乐界面
        initComponent(m_recentlyPlayed,14);                    ///< 初始化最近播放界面
        initComponent(m_allMusic,15);                          ///< 初始化全部音乐界面
    }

    // @note 响应相关跳转
    connect(this->m_collection.get(), &MyCollection::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接收藏跳转
    connect(this->m_localDownload.get(), &LocalDownload::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接本地下载跳转
    connect(this->m_musicCloudDisk.get(), &MusicCloudDisk::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接云盘跳转
    connect(this->m_purchasedMusic.get(), &PurchasedMusic::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接已购音乐跳转
    connect(this->m_recentlyPlayed.get(), &RecentlyPlayed::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接最近播放跳转
    connect(this->m_recentlyPlayed.get(), &RecentlyPlayed::find_more_channel, [this] { on_channel_toolButton_clicked(); }); ///< 连接频道跳转
    connect(this->m_allMusic.get(), &AllMusic::find_more_music, [this] { on_music_repository_toolButton_clicked(); }); ///< 连接全部音乐跳转

    // @note 本地下载相关信号
    connect(this->m_localDownload.get(), &LocalDownload::playMusic, this, &KuGouClient::onPlayLocalMusic); ///< 连接播放本地音乐
    connect(this->m_localDownload.get(), &LocalDownload::cancelLoopPlay, this, [this] {
        if (m_isSingleCircle) ui->circle_toolButton->clicked(); ///< 取消单曲循环
    });                                                           ///< 连接取消循环播放
    connect(this, &KuGouClient::maxScreen, this->m_localDownload.get(), &LocalDownload::onMaxScreenHandle); ///< 连接最大化屏幕处理
}

void KuGouClient::initSearchResultWidget() {
    this->m_searchResultWidget = std::make_unique<QWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_searchResultWidget.get()); ///< 添加搜索结果界面
    this->m_searchResultWidget->setObjectName("searchResultWidget");

    auto hlay1 = new QHBoxLayout; ///< 搜索结果顶部水平布局
    {
        auto topLab = new QLabel("搜索到相关歌曲");
        topLab->setObjectName("searchResultTopLabel");
        hlay1->addSpacing(15);
        hlay1->addWidget(topLab);
        hlay1->addStretch();
    }
    auto hlay2 = new QHBoxLayout; ///< 搜索结果中间水平布局
    {
        hlay2->setSpacing(15);
        auto playAllBtn = new QToolButton;
        playAllBtn->setObjectName("searchResultWidget-playAllBtn");
        playAllBtn->setCursor(Qt::PointingHandCursor);
        playAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置播放全部按钮样式
        playAllBtn->setFixedSize(100, 30); ///< 设置播放全部按钮大小
        playAllBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
        playAllBtn->setText("播放全部");
        auto highListenBtn = new QToolButton;
        highListenBtn->setObjectName("searchResultWidget-highListenBtn");
        highListenBtn->setCursor(Qt::PointingHandCursor);
        highListenBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置高音质按钮样式
        highListenBtn->setFixedSize(100, 30); ///< 设置高音质按钮大小
        highListenBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/highListen-white.svg"))); ///< 设置高音质按钮图标
        highListenBtn->setText("高潮试听");
        auto downloadAllBtn = new QToolButton;
        downloadAllBtn->setObjectName("searchResultWidget-downloadAllBtn");
        downloadAllBtn->setCursor(Qt::PointingHandCursor);
        downloadAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置下载按钮样式
        downloadAllBtn->setFixedSize(100, 30); ///< 设置下载按钮大小
        downloadAllBtn->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg"))); ///< 设置下载按钮图标
        downloadAllBtn->setText("下载全部");
        auto batchOperationBtn = new QToolButton;
        batchOperationBtn->setObjectName("searchResultWidget-batchOperationBtn");
        batchOperationBtn->setCursor(Qt::PointingHandCursor);
        batchOperationBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置批量操作按钮样式
        batchOperationBtn->setFixedSize(100, 30); ///< 设置批量操作按钮大小
        batchOperationBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/batch-operation-black.svg"))); ///< 设置批量操作按钮图标
        batchOperationBtn->setText("批量操作");
        hlay2->addSpacing(15);
        hlay2->addWidget(playAllBtn);
        hlay2->addWidget(highListenBtn);
        hlay2->addWidget(downloadAllBtn);
        hlay2->addWidget(batchOperationBtn);
        hlay2->addStretch();
        connect(playAllBtn,&QToolButton::clicked, [this,playAllBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(playAllBtn->text()),
                             1000, this->window());    ///< 显示提示
        });
        connect(highListenBtn,&QToolButton::clicked, [this,highListenBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(highListenBtn->text()),
                             1000, this->window());    ///< 显示提示
            });
        connect(downloadAllBtn,&QToolButton::clicked, [this,downloadAllBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(downloadAllBtn->text()),
                             1000, this->window());    ///< 显示提示
        });
        connect(batchOperationBtn,&QToolButton::clicked, [this,batchOperationBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(batchOperationBtn->text()),
                             1000, this->window());    ///< 显示提示
        });
    }

    auto scrollArea = new MyScrollArea;
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    scrollArea->setObjectName("searchResultWidgetScrollArea"); ///< 设置对象名称
    scrollArea->setFrameShape(QFrame::NoFrame); ///< 设置无边框
    auto scrollWidget = new QWidget;
    scrollWidget->setObjectName("searchResultWidgetScrollWidget");
    scrollWidget->setAutoFillBackground(true);
    scrollWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto scrollWidgetVLay = new QVBoxLayout(scrollWidget);
    scrollWidgetVLay->addStretch();
    scrollArea->setWidget(scrollWidget);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); ///< 显示垂直滚动条
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); ///< 隐藏水平滚动条


    auto vlay = new QVBoxLayout(this->m_searchResultWidget.get()); ///< 搜索结果中间垂直布局
    vlay->setSpacing(10);
    vlay->addLayout(hlay1);
    vlay->addSpacing(5);
    vlay->addLayout(hlay2);
    vlay->addWidget(scrollArea);
    vlay->addSpacerItem(new QSpacerItem(QSizePolicy::Preferred, QSizePolicy::Preferred));
}

void KuGouClient::initSearchResultMusicItem(MusicItemWidget *item) {
    item->setFillColor(QColor(QStringLiteral("#B0EDF6"))); ///< 设置高亮颜色
    item->setRadius(12); ///< 设置圆角
    item->setInterval(1); ///< 设置间隔
    // connect(item, &MusicItemWidget::play, this, [item, this] {
    //     setPlayItemHighlight(item);                      ///< 设置高亮
    // });
}

/**
 * @brief 异步加载搜索结果里面的封面图片
 * @param item 音乐项
 * @param imageUrl 封面图片的网络路径
 */
void KuGouClient::loadCoverAsync(MusicItemWidget *item, const QString &imageUrl) {
    auto watcher = new QFutureWatcher<QPixmap>(this);
    connect(watcher, &QFutureWatcher<QPixmap>::finished, [item, watcher] {
        item->setCover(watcher->result());
        watcher->deleteLater();
    });

    watcher->setFuture(Async::runAsync([imageUrl] {
        QNetworkAccessManager manager;
        QNetworkRequest request(imageUrl);
        QNetworkReply *imageReply = manager.get(request);

        QEventLoop loop;
        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QPixmap cover;
        if (imageReply->error() == QNetworkReply::NoError) {
            cover.loadFromData(imageReply->readAll());
            //qDebug()<<"加载图片成功："<<cover;
        }
        else {
            //qDebug() << "封面图片下载失败：" << imageReply->errorString();
            return QPixmap(); // 如果失败，设置为空 QPixmap
        }
        imageReply->deleteLater();
        return cover;
    }));
}

// 模板函数实现
template<typename T>
void KuGouClient::initComponent(std::unique_ptr<T>& component, const int& index) {
    component = std::make_unique<T>(ui->stackedWidget);
    ui->stackedWidget->insertWidget(index, component.get());
}

/**
 * @brief 初始化标题栏
 * @note 连接左侧菜单、堆栈切换、最大化和关于对话框信号
 */
void KuGouClient::initTitleWidget() {
    // @note 响应左侧菜单显示
    connect(ui->title_widget, &TitleWidget::leftMenuShow, this, &KuGouClient::onLeftMenuShow);
    // @note 响应堆栈窗口切换
    connect(ui->title_widget, &TitleWidget::currentStackChange, this, &KuGouClient::onTitleCurrentStackChange);
    // @note 响应最大化窗口
    connect(ui->title_widget, &TitleWidget::maxScreen, this, &KuGouClient::onTitleMaxScreen);
    // @note 响应关于对话框显示
    connect(ui->title_widget, &TitleWidget::showAboutDialog, this, [this] {
        MainWindow::onShowAboutDialog(true); ///< 显示关于对话框
    });
    // @note 响应刷新窗口
    connect(ui->title_widget, &TitleWidget::refresh, this, [this] {
        updateSize(); ///< 更新窗口大小
        this->m_refreshMask->showLoading(); ///< 显示加载遮罩
        this->m_refreshMask->raise(); ///< 提升遮罩层级
    });
    // @note 发送退出信号
    connect(this, &MainWindow::exit, ui->title_widget, &TitleWidget::on_close_toolButton_clicked);

    // @note
    connect(ui->title_widget, &TitleWidget::suggestionClicked, this, &KuGouClient::handleSuggestBoxSuggestionClicked);
    connect(ui->title_widget, &TitleWidget::searchTextReturnPressed, this, &KuGouClient::handleSuggestBoxSuggestionClicked);
}

/**
 * @brief 初始化播放栏
 * @note 设置图标、快捷键、工具提示和封面事件
 */
void KuGouClient::initPlayWidget() {
    ui->love_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/collect.svg"))); ///< 设置收藏图标
    ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/download.svg"))); ///< 设置下载图标
    ui->comment_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/comment.svg"))); ///< 设置评论图标
    ui->share_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/share.svg"))); ///< 设置分享图标
    ui->more_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/more.svg"))); ///< 设置更多图标
    ui->pre_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/previous-song.svg"))); ///< 设置上一首图标
    ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置播放/暂停图标
    ui->next_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/next-song.svg"))); ///< 设置下一首图标
    ui->erji_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/together.svg"))); ///< 设置一起听图标
    ui->lyrics_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/song-words.svg"))); ///< 设置歌词图标
    ui->song_queue_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play-list.svg"))); ///< 设置播放队列图标

    ui->cover_label->setPixmap(roundedPixmap(QPixmap(":/Res/playbar/default-cover-gray.svg"), ui->cover_label->size(), 8)); ///< 设置默认封面
    ui->cover_label->installEventFilter(this);                    ///< 安装封面事件过滤器

    // @note 设置快捷键
    new QShortcut(QKeySequence("Space"), this, SLOT(onKeyPause())); ///< 空格键暂停/播放
    new QShortcut(QKeySequence("Right"), this, SLOT(onKeyRight())); ///< 右箭头快进
    new QShortcut(QKeySequence("Left"), this, SLOT(onKeyLeft()));   ///< 左箭头快退

    // @note 设置工具提示
    MainWindow::setElaToolTip(ui->love_toolButton, "我喜欢");      ///< 收藏按钮提示
    MainWindow::setElaToolTip(ui->download_toolButton, "下载");    ///< 下载按钮提示
    MainWindow::setElaToolTip(ui->comment_toolButton, "评论");     ///< 评论按钮提示
    MainWindow::setElaToolTip(ui->share_toolButton, "分享");       ///< 分享按钮提示
    MainWindow::setElaToolTip(ui->more_toolButton, "更多");        ///< 更多按钮提示
    MainWindow::setElaToolTip(ui->circle_toolButton, "循环播放");  ///< 循环播放按钮提示
    MainWindow::setElaToolTip(ui->pre_toolButton, "上一首");       ///< 上一首按钮提示
    MainWindow::setElaToolTip(ui->play_or_pause_toolButton, "播放/暂停"); ///< 播放/暂停按钮提示
    MainWindow::setElaToolTip(ui->next_toolButton, "下一首");       ///< 下一首按钮提示
    MainWindow::setElaToolTip(ui->stander_pushButton, "音质选择");  ///< 音质选择按钮提示
    MainWindow::setElaToolTip(ui->acoustics_pushButton, "音效");   ///< 音效按钮提示
    MainWindow::setElaToolTip(ui->erji_toolButton, "邀请好友一起听"); ///< 一起听按钮提示
    MainWindow::setElaToolTip(ui->lyrics_toolButton, "打开桌面歌词"); ///< 歌词按钮提示
    MainWindow::setElaToolTip(ui->song_queue_toolButton, "播放队列"); ///< 播放队列按钮提示
    auto song_name_text_toolTip = new ElaToolTip(ui->song_name_text); ///< 创建歌曲名称提示
    song_name_text_toolTip->setToolTip("网络歌曲");               ///< 设置默认歌曲提示
    auto singer_text_toolTip = new ElaToolTip(ui->singer_text);    ///< 创建歌手名称提示
    singer_text_toolTip->setToolTip("网络歌手");                  ///< 设置默认歌手提示

    auto font = QFont("AaSongLiuKaiTi");                           ///< 设置字体
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    ui->song_name_text->setFont(font);                             ///< 应用歌曲名称字体
    ui->singer_text->setFont(font);                                ///< 应用歌手名称字体
    ui->song_name_text->adjustSize();
    ui->singer_text->adjustSize();

    connect(this, &KuGouClient::curPlaySongNameChange, [this, song_name_text_toolTip](const QString &songName) {
        song_name_text_toolTip->setToolTip(songName);              ///< 更新歌曲名称提示
        const QFontMetrics fm(ui->song_name_text->font());
        constexpr int maxWidth = 100;
        /// qDebug()<<"歌曲文本长度："<<fm.horizontalAdvance(songName);
        if (fm.horizontalAdvance(songName) > 60)ui->song_name_text->setFixedWidth(fm.horizontalAdvance(songName) > maxWidth ?
            maxWidth : fm.horizontalAdvance(songName));
        else ui->song_name_text->setFixedWidth(fm.horizontalAdvance(songName));
        QString elidedText = fm.elidedText(songName, Qt::ElideRight, ui->song_name_text->width()); ///< 计算省略文本
        /// qDebug()<<"elidedText: "<<elidedText<<" 歌曲文本长度："<<fm.horizontalAdvance(songName) << " ui->song_name_text->width() : "<<ui->song_name_text->width();
        ui->song_name_text->setText(elidedText);                   ///< 设置省略文本
        ui->song_name_text->adjustSize();                          ///< 适应文本
        song_name_text_toolTip->adjustSize();                      ///< 调整提示尺寸
        ui->singer_song_HLayout->update();
    });                                                            ///< 连接歌曲名称变化信号

    connect(this, &KuGouClient::curPlaySingerChange, [this, singer_text_toolTip](const QString &singerName) {
        singer_text_toolTip->setToolTip(singerName);               ///< 更新歌手名称提示
        const QFontMetrics fm(ui->singer_text->font());
        constexpr int maxWidth = 120;
        /// qDebug()<<"歌手文本长度："<<fm.horizontalAdvance(singerName);
        if (fm.horizontalAdvance(singerName) > 60)ui->singer_text->setFixedWidth(fm.horizontalAdvance(singerName)> maxWidth ?
            maxWidth : fm.horizontalAdvance(singerName));
        else  ui->singer_text->setFixedWidth(fm.horizontalAdvance(singerName));
        ui->singer_text->setText(fm.elidedText(singerName, Qt::ElideRight, ui->singer_text->width())); ///< 设置省略文本
        ui->singer_text->adjustSize();                             ///< 适应文本
        singer_text_toolTip->adjustSize();                         ///< 调整提示尺寸
        ui->singer_song_HLayout->update();
    });                                                            ///< 连接歌手名称变化信号

    connect(ui->volume_toolButton, &VolumeToolBtn::volumeChange, this, [this](const int value) {
        this->m_player->setVolume(value / 100.0);                  ///< 设置播放器音量
    });                                                            ///< 连接音量变化信号
    connect(this, &MainWindow::fromTray_noVolume, this, [this](const bool &flag) {
        // @note 未使用，保留用于调试
        // STREAM_INFO() << "KuGouApp 托盘图标点击: " << (flag ? "静音" : "开启声音");
        if ((flag && ui->volume_toolButton->getVolumeValue()) || (!flag && !ui->volume_toolButton->getVolumeValue())) {
            QCoreApplication::sendEvent(ui->volume_toolButton, new QEvent(QEvent::Enter)); ///< 触发进入事件
            ui->volume_toolButton->clicked();                       ///< 模拟点击
        }
    });                                                            ///< 连接托盘音量控制信号

    connect(this->m_player, &VideoPlayer::positionChanged, this, [this](int position) {
        if (ui->progressSlider->isSliderDown()) return;
        ui->progressSlider->setValue(position);                    ///< 更新进度条
        ui->position_label->setText(QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss")); ///< 更新时间标签
    });                                                            ///< 连接播放位置变化信号
    connect(this->m_player, &VideoPlayer::durationChanged, this, &KuGouClient::updateSliderRange); ///< 连接时长变化信号

    connect(this->m_player, &VideoPlayer::pictureFound, this, [this](const QPixmap &pix) {
        if (pix.isNull()) {
            ui->cover_label->installEventFilter(this);             ///< 安装事件过滤器
            ui->cover_label->setPixmap(roundedPixmap(QPixmap(":/Res/playbar/default-cover-gray.svg"), ui->cover_label->size(), 8)); ///< 设置默认封面
        } else {
            ui->cover_label->removeEventFilter(this);              ///< 移除事件过滤器
            ui->cover_label->setPixmap(roundedPixmap(pix, ui->cover_label->size(), 8)); ///< 设置封面图片
        }
    });                                                            ///< 连接封面图片发现信号
    connect(this->m_player, &VideoPlayer::titleFound, this, [this](const QString &song) {
        // @note 未使用，保留用于调试
        // qDebug() << "歌曲：" << song;
        m_musicTitle = song;                                       ///< 存储歌曲标题
        emit curPlaySongNameChange(song);                          ///< 发射歌曲名称变化信号
    });                                                            ///< 连接歌曲标题发现信号
    connect(this->m_player, &VideoPlayer::artistFound, this, [this](const QString &singer) {
        // @note 未使用，保留用于调试
        // qDebug() << "歌手：" << singer;
        m_musicArtist = singer;                                    ///< 存储歌手名称
        emit curPlaySingerChange(singer);                          ///< 发射歌手名称变化信号
    });                                                            ///< 连接歌手名称发现信号

    connect(this->m_player, &VideoPlayer::audioPlay, this, [this] {
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg"))); ///< 设置播放图标
    });                                                            ///< 连接音频播放信号
    connect(this->m_player, &VideoPlayer::audioPause, this, [this] {
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置暂停图标
    });                                                            ///< 连接音频暂停信号

    mediaStatusConnection = connect(this->m_player, &VideoPlayer::audioFinish, this, [this] {
        // @note 未使用，保留用于调试
        // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置暂停图标
        this->m_localDownload->audioFinished();                    ///< 通知本地下载组件
    });                                                            ///< 连接音频播放结束信号
    connect(this->m_player, &VideoPlayer::errorOccur, this, [this](const QString &msg) {
        ElaMessageBar::error(ElaMessageBarType::BottomRight, "Error", msg, 2000, this->window()); ///< 显示错误提示
    });                                                            ///< 连接错误信号

    ui->progressSlider->installEventFilter(this);                  ///< 安装进度条事件过滤器
    connect(ui->progressSlider, &QSlider::sliderReleased, this, &KuGouClient::updateProcess); ///< 连接进度条释放信号
    connect(ui->play_widget, &PlayWidget::doubleClicked, this, &KuGouClient::onTitleMaxScreen); ///< 连接播放栏双击信号
}

/**
 * @brief 初始化菜单
 * @note 设置 14 个互斥按钮的图标和分组
 */
void KuGouClient::initMenu() {
    // @note 设置菜单按钮父对象
    this->m_menuBtnGroup->setParent(ui->center_menu_widget);

    ui->recommend_you_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/recommend.svg"))); ///< 设置推荐图标
    ui->music_repository_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/music-library.svg"))); ///< 设置音乐库图标
    ui->channel_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/my-channel.svg"))); ///< 设置频道图标
    ui->video_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/video.svg"))); ///< 设置视频图标
    ui->live_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/live.svg"))); ///< 设置直播图标
    ui->ai_chat_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/ai-chat.svg"))); ///< 设置 AI 聊天图标
    ui->song_list_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/song-list.svg"))); ///< 设置歌单图标
    ui->daily_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/daily.svg"))); ///< 设置每日推荐图标
    ui->my_collection_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/collect.svg"))); ///< 设置收藏图标
    ui->local_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg"))); ///< 设置本地下载图标
    ui->music_cloud_disk_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/cloud.svg"))); ///< 设置云盘图标
    ui->purchased_music_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/bought.svg"))); ///< 设置已购音乐图标
    ui->recently_played_toolButton->setIcon(QIcon(QStringLiteral(":/Res/window/history.svg"))); ///< 设置最近播放图标
    ui->all_music_toolButton->setIcon(QIcon(QStringLiteral(":/Res/titlebar/menu-black.svg"))); ///< 设置全部音乐图标
    // @note 设置互斥按钮组
    m_menuBtnGroup->addButton(ui->recommend_you_toolButton);      ///< 添加推荐按钮
    m_menuBtnGroup->addButton(ui->music_repository_toolButton);   ///< 添加音乐库按钮
    m_menuBtnGroup->addButton(ui->channel_toolButton);            ///< 添加频道按钮
    m_menuBtnGroup->addButton(ui->video_toolButton);              ///< 添加视频按钮
    m_menuBtnGroup->addButton(ui->live_toolButton);               ///< 添加直播按钮
    m_menuBtnGroup->addButton(ui->ai_chat_toolButton);            ///< 添加 AI 聊天按钮
    m_menuBtnGroup->addButton(ui->song_list_toolButton);          ///< 添加歌单按钮
    m_menuBtnGroup->addButton(ui->daily_recommend_toolButton);    ///< 添加每日推荐按钮
    m_menuBtnGroup->addButton(ui->my_collection_toolButton);      ///< 添加收藏按钮
    m_menuBtnGroup->addButton(ui->local_download_toolButton);     ///< 添加本地下载按钮
    m_menuBtnGroup->addButton(ui->music_cloud_disk_toolButton);   ///< 添加云盘按钮
    m_menuBtnGroup->addButton(ui->purchased_music_toolButton);    ///< 添加已购音乐按钮
    m_menuBtnGroup->addButton(ui->recently_played_toolButton);    ///< 添加最近播放按钮
    m_menuBtnGroup->addButton(ui->all_music_toolButton);          ///< 添加全部音乐按钮
    m_menuBtnGroup->setExclusive(true);                           ///< 设置互斥
}

/**
 * @brief 初始化角标
 * @note 设置窗口大小调整角标
 */
void KuGouClient::initCornerWidget() {
    this->m_sizeGrip->setFixedSize(11, 11);                       ///< 设置角标大小
    this->m_sizeGrip->setObjectName(QStringLiteral("sizegrip"));  ///< 设置对象名称
}

/**
 * @brief 更新窗口大小
 * @note 触发重绘并同步遮罩大小
 */
void KuGouClient::updateSize() {
    QResizeEvent resizeEvent(this->size(), this->size());         ///< 创建调整大小事件
    QApplication::sendEvent(this, &resizeEvent);                   ///< 发送事件
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 * @note 控制 14 个菜单按钮和标题栏的交互
 */
void KuGouClient::enableButton(const bool &flag) {
    ui->recommend_you_toolButton->setEnabled(flag);               ///< 设置推荐按钮
    ui->music_repository_toolButton->setEnabled(flag);            ///< 设置音乐库按钮
    ui->song_list_toolButton->setEnabled(flag);                   ///< 设置歌单按钮
    ui->channel_toolButton->setEnabled(flag);                     ///< 设置频道按钮
    ui->video_toolButton->setEnabled(flag);                       ///< 设置视频按钮
    ui->live_toolButton->setEnabled(flag);                        ///< 设置直播按钮
    ui->ai_chat_toolButton->setEnabled(flag);                     ///< 设置 AI 聊天按钮
    ui->daily_recommend_toolButton->setEnabled(flag);             ///< 设置每日推荐按钮
    ui->my_collection_toolButton->setEnabled(flag);               ///< 设置收藏按钮
    ui->local_download_toolButton->setEnabled(flag);              ///< 设置本地下载按钮
    ui->music_cloud_disk_toolButton->setEnabled(flag);            ///< 设置云盘按钮
    ui->purchased_music_toolButton->setEnabled(flag);             ///< 设置已购音乐按钮
    ui->recently_played_toolButton->setEnabled(flag);             ///< 设置最近播放按钮
    ui->all_music_toolButton->setEnabled(flag);                   ///< 设置全部音乐按钮
    ui->title_widget->setEnableChange(flag);                      ///< 设置标题栏交互
}

/**
 * @brief 鼠标按下事件
 * @param ev 鼠标事件
 * @note 处理窗口拖动逻辑
 */
void KuGouClient::mousePressEvent(QMouseEvent *ev) {
    MainWindow::mousePressEvent(ev); ///< 调用父类处理
    if (this->m_isTransForming) return;
    if (ev->button() == Qt::LeftButton) {
        this->m_pressPos = ev->pos(); ///< 记录按下位置
    }
}

/**
 * @brief 鼠标移动事件
 * @param event 鼠标事件
 * @note 处理窗口拖动和最大化还原
 */
void KuGouClient::mouseMoveEvent(QMouseEvent *event) {
    MainWindow::mouseMoveEvent(event); ///< 调用父类处理
    if (this->m_isTransForming) return;
    point_offset = event->globalPosition().toPoint() - mousePs; ///< 计算鼠标偏移量

    if (isPress) {
        if (mouse_press_region == kMousePositionMid) {
            if (ui->title_widget->geometry().contains(this->m_pressPos)) {
                if (this->m_isMaxScreen == true) {
                    ///< 最大化状态下拖动
                    this->m_startGeometry.setX(event->pos().x() - this->m_normalGeometry.width() / 2);
                    this->m_startGeometry.setY(event->pos().y() - 20);
                    this->m_startGeometry.setWidth(this->m_normalGeometry.width());
                    this->m_startGeometry.setHeight(this->m_normalGeometry.height());
                    ui->title_widget->max_toolButton()->clicked(); ///< 触发最大化按钮
                    return;
                }
                move(windowsLastPs + point_offset); ///< 移动窗口
            }
            if (ui->play_widget->geometry().contains(this->m_pressPos)) {
                if (this->m_isMaxScreen == true) {
                    ///< 最大化状态下拖动
                    this->m_startGeometry.setX(event->pos().x() - this->m_normalGeometry.width() / 2);
                    this->m_startGeometry.setY(event->pos().y() - this->m_normalGeometry.height() + 20);
                    this->m_startGeometry.setWidth(this->m_normalGeometry.width());
                    this->m_startGeometry.setHeight(this->m_normalGeometry.height());
                    ui->title_widget->max_toolButton()->clicked(); ///< 触发最大化按钮
                    return;
                }
                move(windowsLastPs + point_offset); ///< 移动窗口
            }
        }
    }
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 默认实现
 */
void KuGouClient::paintEvent(QPaintEvent *ev) {
    MainWindow::paintEvent(ev); ///< 调用父类处理
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 更新最大化状态、角标位置和文本省略
 */
void KuGouClient::resizeEvent(QResizeEvent *event) {
    MainWindow::resizeEvent(event); ///< 调用父类处理
    if (this->geometry() != this->screen()->availableGeometry()) {
        this->m_isMaxScreen = false; ///< 非全屏状态
    } else {
        this->m_isMaxScreen = true; ///< 全屏状态
    }
    // @note 移动角标
    this->m_sizeGrip->move(this->width() - this->m_sizeGrip->width() - 8,
                           this->height() - this->m_sizeGrip->height() - 8);
    this->m_sizeGrip->raise(); ///< 提升角标层级
    this->m_sizeGrip->setVisible(true); ///< 显示角标
    // @note 调整歌曲信息宽度
    ui->song_info_widget->setFixedWidth(this->width() / 8 + 20);
    // @note 更新歌曲和歌手文本
    if (!this->m_player->getMusicPath().isEmpty()) {
        const QFontMetrics fm(ui->song_name_text->font());
        ui->song_name_text->setText(fm.elidedText(m_musicTitle, Qt::ElideRight, ui->song_name_text->width()));
        ///< 设置歌曲省略文本
        ui->singer_text->setText(fm.elidedText(m_musicArtist, Qt::ElideRight, ui->singer_text->width())); ///< 设置歌手省略文本
    }
    // @note 同步刷新遮罩大小
    auto rect = ui->stackedWidget->geometry();
    rect.setLeft(5);
    rect.setRight(rect.width() - 6);
    this->m_refreshMask->setGeometry(rect); ///< 设置遮罩几何形状
}

/**
 * @brief 事件处理
 * @param event 事件
 * @return 是否处理事件
 * @note 处理鼠标移动事件
 */
bool KuGouClient::event(QEvent *event) {
    if (QEvent::HoverMove == event->type()) {
        ///< 鼠标移动事件
        auto ev = static_cast<QMouseEvent *>(event);
        this->mouseMoveEvent(ev); ///< 处理鼠标移动
        return true;
    }
    return MainWindow::event(event); ///< 调用父类处理
}

/**
 * @brief 事件过滤器
 * @param watched 监控对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理进度条和封面标签事件
 */
bool KuGouClient::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->progressSlider) {
        // @note 禁用进度条拖拽
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() ==
            QEvent::MouseMove) {
            if (this->m_player->getMusicPath().isEmpty()) {
                return true; ///< 拦截鼠标事件
            }
        }
        if (event->type() == QEvent::MouseButtonPress) {
            ///< 鼠标按下事件
            if (this->m_player->state() == VideoPlayer::State::Stop) {
                this->m_player->replay(true); ///< 重新播放
            }
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                ///< 左键按下
                qint64 value = QStyle::sliderValueFromPosition(ui->progressSlider->minimum(),
                                                               ui->progressSlider->maximum(),
                                                               mouseEvent->pos().x(),
                                                               ui->progressSlider->width()); ///< 计算进度值
                // @note 未使用，保留用于调试
                /*
                auto ani = new QPropertyAnimation(ui->progressSlider, "sliderPosition");
                ani->setDuration(100);
                ani->setStartValue(ui->progressSlider->value());
                ani->setEndValue(value);
                ani->setEasingCurve(QEasingCurve::Linear);
                ani->start(QAbstractAnimation::DeleteWhenStopped);
                */
                this->m_player->pause(); ///< 暂停播放
                this->m_player->seek(value * 1000); ///< 跳转到指定位置
                this->m_player->play(); ///< 继续播放
            }
        }
    } else if (watched == ui->cover_label) {
        if (event->type() == QEvent::Enter) {
            ui->cover_label->setPixmap(roundedPixmap(QPixmap(":/Res/playbar/default-cover-blue.svg"),
                                                     ui->cover_label->size(), 8)); ///< 设置悬停封面
        } else if (event->type() == QEvent::Leave) {
            ui->cover_label->setPixmap(roundedPixmap(QPixmap(":/Res/playbar/default-cover-gray.svg"),
                                                     ui->cover_label->size(), 8)); ///< 设置默认封面
        }
    }
    return MainWindow::eventFilter(watched, event); ///< 调用父类处理
}

/**
 * @brief 显示事件
 * @param event 显示事件
 * @note 更新窗口大小
 */
void KuGouClient::showEvent(QShowEvent *event) {
    MainWindow::showEvent(event); ///< 调用父类处理
    updateSize(); ///< 更新窗口大小
}

/**
 * @brief 关闭事件
 * @param event 关闭事件
 * @note 默认实现
 */
void KuGouClient::closeEvent(QCloseEvent *event) {
    MainWindow::closeEvent(event); ///< 调用父类处理
}

/**
 * @brief 推荐按钮点击槽函数
 * @note 触发标题栏推荐页面切换
 */
void KuGouClient::on_recommend_you_toolButton_clicked() {
    ui->title_widget->onLeftMenu_recommend_clicked(); ///< 切换到推荐页面
}

/**
 * @brief 音乐库按钮点击槽函数
 * @note 触发标题栏音乐库页面切换
 */
void KuGouClient::on_music_repository_toolButton_clicked() {
    ui->title_widget->onLeftMenu_musicRepository_clicked(); ///< 切换到音乐库页面
}

/**
 * @brief 频道按钮点击槽函数
 * @note 触发标题栏频道页面切换
 */
void KuGouClient::on_channel_toolButton_clicked() {
    ui->title_widget->onLeftMenu_channel_clicked(); ///< 切换到频道页面
}

/**
 * @brief 视频按钮点击槽函数
 * @note 触发标题栏视频页面切换
 */
void KuGouClient::on_video_toolButton_clicked() {
    ui->title_widget->onLeftMenu_video_clicked(); ///< 切换到视频页面
}

/**
 * @brief 直播按钮点击槽函数
 * @note 触发标题栏直播页面切换
 */
void KuGouClient::on_live_toolButton_clicked() {
    ui->title_widget->onLeftMenu_live_clicked(); ///< 切换到直播页面
}

/**
 * @brief AI 聊天按钮点击槽函数
 * @note 触发标题栏 AI 聊天页面切换
 */
void KuGouClient::on_ai_chat_toolButton_clicked() {
    ui->title_widget->onLeftMenu_ai_chat_clicked(); ///< 切换到 AI 聊天页面
}

/**
 * @brief 歌单按钮点击槽函数
 * @note 触发标题栏歌单页面切换
 */
void KuGouClient::on_song_list_toolButton_clicked() {
    ui->title_widget->onLeftMenu_songList_clicked(); ///< 切换到歌单页面
}

/**
 * @brief 每日推荐按钮点击槽函数
 * @note 触发标题栏每日推荐页面切换
 */
void KuGouClient::on_daily_recommend_toolButton_clicked() {
    ui->title_widget->onLeftMenu_dailyRecommend_clicked(); ///< 切换到每日推荐页面
}

/**
 * @brief 收藏按钮点击槽函数
 * @note 触发标题栏收藏页面切换
 */
void KuGouClient::on_my_collection_toolButton_clicked() {
    ui->title_widget->onLeftMenu_collection_clicked(); ///< 切换到收藏页面
}

/**
 * @brief 本地下载按钮点击槽函数
 * @note 触发标题栏本地下载页面切换
 */
void KuGouClient::on_local_download_toolButton_clicked() {
    ui->title_widget->onLeftMenu_localDownload_clicked(); ///< 切换到本地下载页面
}

/**
 * @brief 云盘按钮点击槽函数
 * @note 触发标题栏云盘页面切换
 */
void KuGouClient::on_music_cloud_disk_toolButton_clicked() {
    ui->title_widget->onLeftMenu_musicCloudDisk_clicked(); ///< 切换到云盘页面
}

/**
 * @brief 已购音乐按钮点击槽函数
 * @note 触发标题栏已购音乐页面切换
 */
void KuGouClient::on_purchased_music_toolButton_clicked() {
    ui->title_widget->onLeftMenu_purchasedMusic_clicked(); ///< 切换到已购音乐页面
}

/**
 * @brief 最近播放按钮点击槽函数
 * @note 触发标题栏最近播放页面切换
 */
void KuGouClient::on_recently_played_toolButton_clicked() {
    ui->title_widget->onLeftMenu_recentlyPlayed_clicked(); ///< 切换到最近播放页面
}

/**
 * @brief 全部音乐按钮点击槽函数
 * @note 触发标题栏全部音乐页面切换
 */
void KuGouClient::on_all_music_toolButton_clicked() {
    ui->title_widget->onLeftMenu_allMusic_clicked(); ///< 切换到全部音乐页面
}

/**
* @brief 处理suggestBox选中项槽函数
 * @note 切换搜索结果界面
 */
void KuGouClient::handleSuggestBoxSuggestionClicked(const QString &suggestText, const QVariantMap &suggestData) {
    qDebug() << "选中：" << suggestText << " 附带数据：" << suggestData;
    ui->stackedWidget->setCurrentWidget(this->m_searchResultWidget.get()); ///< 切换到搜索结果界面
    auto topLab = m_searchResultWidget->findChild<QLabel *>("searchResultTopLabel");
    if (topLab) {
        if (!suggestText.trimmed().isEmpty()){
            QString htmlText = QString(
                       R"(<span style="color:gray;">搜索到 </span><span style="color:red;">%1</span><span style="color:gray;"> 的相关歌曲</span>)")
                   .arg(suggestText);
            topLab->setText(htmlText);
        }
        else {
            const auto htmlText = QString(R"(<span style="color:gray;">搜索到今日推荐歌曲</span>)");
            topLab->setText(htmlText);
        }
    }
    ///< 清空容器
    for (MusicItemWidget *item: m_searchMusicItemVector) {
        item->setParent(nullptr); // 可选：脱离原父对象
        delete item; // 释放堆内存
    }
    m_searchMusicItemVector.clear(); // 清空 QVector 中的所有元素
    this->m_refreshMask->keepLoading();

    // 启动异步任务从服务端获取搜索结果
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this, suggestText] {
        return m_libHttp.UrlRequestGet(
            QString("http://127.0.0.1:8080/api/searchSong"),
            "keyword=" + QUrl::toPercentEncoding(suggestText),
            3000 // 3秒超时
        );
    });

    // 结果回调（在主线程执行）
    Async::onResultReady(future, this, [this](const QString &responseData) {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(responseData.toUtf8(), &err);

        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning() << "搜索响应解析失败:" << err.errorString();
            this->m_refreshMask->hideLoading("响应失败");
            return;
        }

        QJsonObject obj = doc.object();
        if (obj["status"].toString() != "success") {
            qWarning() << "搜索失败:" << obj["message"].toString();
            return;
        }

        QList<SongInfor> songs;
        QJsonArray songsArray = obj["data"].toArray();

        for (const auto &item : songsArray) {
            QJsonObject songObj = item.toObject();
            SongInfor song;

            song.hash = songObj["hash"].toString();
            song.songName = songObj["songName"].toString();
            song.singer = songObj["singer"].toString();
            song.album = songObj["album"].toString();
            song.duration = songObj["duration"].toString();
            song.coverUrl = songObj["coverUrl"].toString();
            song.netUrl = songObj["netUrl"].toString();
            song.cover = song.coverUrl.isEmpty() ? QPixmap(":/Res/tablisticon/pix4.png") : song.cover;
            songs.append(song);
        }

        // 更新UI（与原始代码相同）
        auto scrollWidget = m_searchResultWidget->findChild<QWidget*>("searchResultWidgetScrollWidget");
        if (!scrollWidget) {
            qWarning() << "未找到滚动窗口部件";
            return;
        }

        auto layout = qobject_cast<QVBoxLayout*>(scrollWidget->layout());
        if (!layout) {
            qWarning() << "滚动窗口布局无效";
            return;
        }

        // 清空现有结果
        for (MusicItemWidget *item : m_searchMusicItemVector) {
            item->setParent(nullptr);
            delete item;
        }
        m_searchMusicItemVector.clear();

        // 使用定时器逐项添加
        int currentIndex = 0;
        QTimer* addTimer = new QTimer(this);

        connect(addTimer, &QTimer::timeout, this, [=]() mutable {
            if (currentIndex >= songs.size()) {
                addTimer->deleteLater();
                this->m_refreshMask->hideLoading("加载完成");
                return;
            }

            auto &song = songs[currentIndex];
            auto item = new MusicItemWidget(song, this);
            item->setPopular(6 - currentIndex);
            item->setIndexText(currentIndex + 1);
            initSearchResultMusicItem(item);

            layout->insertWidget(layout->count() - 1, item);
            m_searchMusicItemVector.append(item);

            // 异步加载封面
            if (!song.coverUrl.isEmpty()) {
                loadCoverAsync(item, song.coverUrl);
            }

            currentIndex++;
        });

        addTimer->start(100);
    });
}

/**
 * @brief 更新播放进度
 * @note 根据进度条位置调整播放器进度
 */
void KuGouClient::updateProcess() {
    qint64 position = ui->progressSlider->value() * this->m_player->getTotalTime() / ui->progressSlider->maximum();
    ///< 计算播放位置
    // @note 未使用，保留用于调试
    // qDebug() << "position : " << position;
    this->m_player->pause(); ///< 暂停播放
    this->m_player->seek(position); ///< 跳转到指定位置
    this->m_player->play(); ///< 继续播放
}

/**
 * @brief 更新进度条范围
 * @param duration 总时长（毫秒）
 * @note 设置进度条最大值和时长标签
 */
void KuGouClient::updateSliderRange(const qint64 &duration) {
    ui->progressSlider->setMaximum(static_cast<int>(duration)); ///< 设置进度条最大值
    // @note 未使用，保留用于调试
    // qDebug() << "改变总时长为：" << duration;
    ui->duration_label->setText(QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("mm:ss"));
    ///< 更新时长标签
}

/**
 * @brief 空格键快捷键槽函数
 * @note 切换播放/暂停状态
 */
void KuGouClient::onKeyPause() {
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
void KuGouClient::onKeyLeft() {
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
void KuGouClient::onKeyRight() {
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
void KuGouClient::onTitleCurrentStackChange(const int &index, const bool &slide) {
    if (ui->stackedWidget->currentIndex() == index) return;
    this->m_refreshMask->hide(); ///< 隐藏刷新遮罩
    this->m_snackbar->hide(); ///< 隐藏消息提示条
    if (slide) {
        enableButton(false); ///< 禁用按钮
        ui->stackedWidget->slideInIdx(index); ///< 滑动切换页面
    } else {
        ui->stackedWidget->setCurrentIndex(index); ///< 直接切换页面
    }
    switch (index) {
        case 3: ui->recommend_you_toolButton->setChecked(true); break; ///< 推荐页面
        case 4: ui->music_repository_toolButton->setChecked(true); break; ///< 音乐库页面
        case 5: ui->channel_toolButton->setChecked(true); break;  ///< 频道页面
        case 6: ui->video_toolButton->setChecked(true); break;    ///< 视频页面
        case 7: ui->ai_chat_toolButton->setChecked(true); break;  ///< AI 聊天页面
        case 8: ui->song_list_toolButton->setChecked(true); break; ///< 歌单页面
        case 9: ui->daily_recommend_toolButton->setChecked(true); break; ///< 每日推荐页面
        case 10: ui->my_collection_toolButton->setChecked(true); break; ///< 收藏页面
        case 11: ui->local_download_toolButton->setChecked(true); break; ///< 本地下载页面
        case 12: ui->music_cloud_disk_toolButton->setChecked(true); break; ///< 云盘页面
        case 13: ui->purchased_music_toolButton->setChecked(true); break; ///< 已购音乐页面
        case 14: ui->recently_played_toolButton->setChecked(true); break; ///< 最近播放页面
        case 15: ui->all_music_toolButton->setChecked(true); break; ///< 全部音乐页面
        default: break;
    }
    updateSize(); ///< 更新窗口大小
}

/**
 * @brief 左侧菜单显示槽函数
 * @param flag 是否显示
 * @note 显示或隐藏菜单滚动区域
 */
void KuGouClient::onLeftMenuShow(const bool &flag) const {
    if (flag) {
        ui->menu_scrollArea->show(); ///< 显示菜单
    } else {
        ui->menu_scrollArea->hide(); ///< 隐藏菜单
    }
}

/**
 * @brief 标题栏最大化槽函数
 * @note 切换最大化/正常状态并执行动画
 */
void KuGouClient::onTitleMaxScreen() {
    // @note 未使用，保留用于调试
    // STREAM_INFO() << "最大化窗口";
    if (m_isMaxScreen) {
        this->m_isMaxScreen = false; ///< 设置正常状态
        m_endGeometry = m_startGeometry; ///< 记录正常几何形状
        m_startGeometry = this->screen()->availableGeometry(); ///< 设置最大化几何形状
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url(":/Res/titlebar/maximize-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })"; ///< 设置最大化按钮样式
        ui->title_widget->max_toolButton()->setMyIcon(QIcon(":/Res/titlebar/maximize-black.svg")); ///< 设置最大化图标
        this->m_animation->setDuration(500); ///< 设置动画时长
    } else {
        this->m_normalGeometry = this->geometry(); ///< 记录正常几何形状
        this->m_isMaxScreen = true; ///< 设置最大化状态
        m_startGeometry = this->m_normalGeometry; ///< 设置起始几何形状
        m_endGeometry = this->screen()->availableGeometry(); ///< 设置目标几何形状
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url(":/Res/titlebar/resume-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })"; ///< 设置还原按钮样式
        ui->title_widget->max_toolButton()->setMyIcon(QIcon(":/Res/titlebar/resume-black.svg")); ///< 设置还原图标
        this->m_animation->setDuration(400); ///< 设置动画时长
    }
    this->m_animation->setStartValue(m_startGeometry); ///< 设置动画起始值
    this->m_animation->setEndValue(m_endGeometry); ///< 设置动画结束值
    this->m_animation->setEasingCurve(QEasingCurve::InOutQuad); ///< 设置缓动曲线

    this->m_isTransForming = true; ///< 禁用交互
    this->m_animation->start(); ///< 开始动画
    connect(this->m_animation.get(), &QPropertyAnimation::finished, this, [this] {
        QTimer::singleShot(100, this, [this] {
            this->m_isTransForming = false; ///< 启用交互
        });
        if (this->m_endGeometry == this->screen()->availableGeometry()) emit maxScreen(); ///< 发射最大化信号
    }); ///< 连接动画结束信号

    ui->title_widget->max_toolButton()->setStyleSheet(this->m_maxBtnStyle); ///< 更新按钮样式
}

/**
 * @brief 播放本地音乐
 * @param localPath 本地音乐路径
 * @note 检查文件存在并启动播放
 */
void KuGouClient::onPlayLocalMusic(const QString &localPath) {
    // @note 未使用，保留用于调试
    // qDebug() << "播放：" << localPath;
    if (!QFile::exists(localPath)) {
        // @note 未使用，保留用于调试
        // qDebug() << "File does not exist:" << localPath;
        return;
    }
    if (!m_player->startPlay(localPath.toStdString())) {
        ElaMessageBar::error(ElaMessageBarType::BottomRight, "Error", "Failed to start playback", 2000,
                             this->window()); ///< 显示播放失败提示
    }
}

/**
 * @brief 播放/暂停按钮点击槽函数
 * @note 切换播放/暂停状态
 */
void KuGouClient::on_play_or_pause_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }

    if (this->m_player->state() == VideoPlayer::State::Pause) {
        this->m_player->play(); ///< 继续播放
        // @note 未使用，保留用于调试
        // qDebug() << "播放";
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg"))); ///< 设置播放图标
    } else if (this->m_player->state() == VideoPlayer::State::Playing) {
        this->m_player->pause(); ///< 暂停播放
        // @note 未使用，保留用于调试
        // qDebug() << "暂停";
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置暂停图标
    } else if (this->m_player->state() == VideoPlayer::State::Stop) {
        // @note 未使用，保留用于调试
        // qDebug() << "当前状态：Stop,开始重新播放。。。";
        this->m_player->replay(true); ///< 重新播放
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg"))); ///< 设置播放图标
    }
}

/**
 * @brief 收藏按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_love_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 下载按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_download_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 评论按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_comment_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 分享按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_share_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 更多按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_more_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
    }
}

/**
 * @brief 循环播放按钮点击槽函数
 * @note 切换单曲循环状态
 */
void KuGouClient::on_circle_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    m_isSingleCircle = !m_isSingleCircle; ///< 切换循环状态
    if (m_isSingleCircle) {
        // @note 未使用，保留用于调试
        // qDebug() << "单曲循环";
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url(':/Res/playbar/single-list-loop-gray.svg');}
               QToolButton:hover{border-image:url(':/Res/playbar/single-list-loop-blue.svg');})"); ///< 设置单曲循环样式
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection); ///< 断开原有连接
            mediaStatusConnection = connect(this->m_player, &VideoPlayer::audioFinish, this, [this] {
                // @note 未使用，保留用于调试
                // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
                ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置暂停图标
                // @note 未使用，保留用于调试
                // qDebug() << __LINE__ << " 重新播放";
                this->m_player->replay(true); ///< 重新播放
            }); ///< 连接单曲循环信号
        } else {
            // @note 未使用，保留用于调试
            // qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty"; ///< 记录警告日志
        }
    } else {
        // @note 未使用，保留用于调试
        // qDebug() << "播放一次";
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection); ///< 断开原有连接
            mediaStatusConnection = connect(this->m_player, &VideoPlayer::audioFinish, this, [this] {
                // @note 未使用，保留用于调试
                // qDebug() << __LINE__ << " ***** " << this->m_player->getMusicPath() << "播放结束。。。";
                ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg"))); ///< 设置暂停图标
                // @note 未使用，保留用于调试
                // qDebug() << "正常结束";
                this->m_localDownload->audioFinished(); ///< 通知本地下载组件
            }); ///< 连接正常播放结束信号
        } else {
            // @note 未使用，保留用于调试
            // qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty"; ///< 记录警告日志
        }
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url(':/Res/playbar/list-loop-gray.svg');}
               QToolButton:hover{border-image:url(':/Res/playbar/list-loop-blue.svg');})"); ///< 设置列表循环样式
    }
}

/**
 * @brief 上一首按钮点击槽函数
 * @note 播放上一首本地歌曲
 */
void KuGouClient::on_pre_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    this->m_localDownload->playLocalSongPrevSong(); ///< 播放上一首
}

/**
 * @brief 下一首按钮点击槽函数
 * @note 播放下一首本地歌曲
 */
void KuGouClient::on_next_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
    this->m_localDownload->playLocalSongNextSong(); ///< 播放下一首
}

/**
 * @brief 速度选择按钮点击槽函数
 * @note 显示速度选择界面
 */
void KuGouClient::on_speed_pushButton_clicked() {
    /// 弹出速度相关界面，并且在隐藏的时候销毁
    auto speedDialog = new SpeedDialog(this);

    // 连接关闭信号以保存状态
    connect(speedDialog, &SpeedDialog::aboutToClose, [this, speedDialog]() {
        // 获取当前状态并保存到文件
        m_speedDialogState = speedDialog->getState();
        m_speedDialogState.save();
    });

    // 从文件加载并恢复状态
    speedDialog->setState(m_speedDialogState);
    // @note 未使用，保留用于调试
    /*
        connect(speedDialog, &QObject::destroyed, this, [] {
            qDebug() << "SpeedDialog destroyed";
        });
    */
    auto tmpPos = ui->speed_pushButton->mapToGlobal(QPoint(ui->speed_pushButton->width() / 2,
                                                           ui->speed_pushButton->height() / 2));
    tmpPos.setX(tmpPos.x() - speedDialog->width() / 2);
    tmpPos.setY(tmpPos.y() - speedDialog->height() - 15);
    auto pos = mapFromGlobal(tmpPos);
    speedDialog->move(pos);
    speedDialog->show();

    /// 连接槽
    connect(speedDialog, &SpeedDialog::btnTextChanged, this, [this](const QString &text) {
        ui->speed_pushButton->setText(text);
        if (text == "倍速") {
            ui->speed_pushButton->setStyleSheet("background-color: transparent;");
        } else {
            ui->speed_pushButton->setStyleSheet(
                "background-color: qlineargradient(spread:pad,x1:0, y1:0,x2:1, y2:0,stop:0 rgb(105, 225, 255), stop:1 rgba(255, 182, 193, 255));");
        }
    });

    connect(speedDialog, &SpeedDialog::speedChanged, this, [this](const float &speed) {
        // @note 调试用 qDebug() << "设置速度为：" << speed;
        if (this->m_player)this->m_player->setSpeed(speed);
    });
}

/**
 * @brief 音质选择按钮点击槽函数
 * @note 显示未实现提示
 */
void KuGouClient::on_stander_pushButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", QStringLiteral("音质选择功能 暂未实现 敬请期待"), 1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}

/**
 * @brief 音效按钮点击槽函数
 * @note 显示未实现提示
 */
void KuGouClient::on_acoustics_pushButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", QStringLiteral("音效功能 暂未实现 敬请期待"), 1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}

/**
 * @brief 一起听按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_erji_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
}

/**
 * @brief 歌词按钮点击槽函数
 * @note 检查音乐路径并显示提示
 */
void KuGouClient::on_lyrics_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000,
                               this->window()); ///< 显示无音乐提示
        return;
    }
}

/**
 * @brief 播放队列按钮点击槽函数
 * @note 显示未实现提示
 */
void KuGouClient::on_song_queue_toolButton_clicked() {
    if (this->m_player->getMusicPath().isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", QStringLiteral("播放队列功能 暂未实现 敬请期待"), 1000,
                                   this->window()); ///< 显示未实现提示
        return;
    }
}
