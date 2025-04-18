#include "KuGouApp.h"
#include "ui_KuGouApp.h"
#include "logger.hpp"
#include "RippleButton.h"
#include "RefreshMask.h"
#include "qtmaterialsnackbar.h"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPainterPath>
#include <QPainter>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QPoint>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QFontDatabase>
#include <QSizeGrip>
#include <QPropertyAnimation>
#include <QShortcut>
#include <QTimer>

QPixmap roundedPixmap(const QPixmap &src, QSize size, int radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}

KuGouApp::KuGouApp(MainWindow *parent)
    : MainWindow(parent)
    , ui(new Ui::KuGouApp)
    , m_player(std::make_unique<QMediaPlayer>(this))
    , m_audioOutput(std::make_unique<QAudioOutput>(this))
    , m_menuBtnGroup(std::make_unique<QButtonGroup>(this))
    , m_sizeGrip(std::make_unique<QSizeGrip>(this))
    , m_animation(std::make_unique<QPropertyAnimation>(this, "geometry"))
    , m_refreshMask(std::make_unique<RefreshMask>())
    , m_snackbar(std::make_unique<QtMaterialSnackbar>())
{
    ui->setupUi(this);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/kugou.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }

    initUi();

    //动画结束，恢复可交互
    connect(ui->stackedWidget,&SlidingStackedWidget::animationFinished,[this]{enableButton(true);});
    enableButton(true);
    ui->stackedWidget->setVerticalMode(true);
    //默认为你推荐
    ui->recommend_you_toolButton->clicked();
}

KuGouApp::~KuGouApp() {
    // call before spdlog static variables destroy
    mylog::logger::get().shutdown();
    delete ui;
}

void KuGouApp::initFontRes() {
    auto fontId = QFontDatabase::addApplicationFont(":/Res/font/dialog.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    //auto families = QFontDatabase::applicationFontFamilies(fontId);
    //qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //AaSongLiuKaiTi

    fontId = QFontDatabase::addApplicationFont( ":/Res/font/ElaAwesome.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    //families = QFontDatabase::applicationFontFamilies(fontId);
    //qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //ElaAwesome

    fontId = QFontDatabase::addApplicationFont(":/Res/font/qing-ning-you-yuan.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    //families = QFontDatabase::applicationFontFamilies(fontId);
    //qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //YouYuan

    fontId = QFontDatabase::addApplicationFont(":/Res/font/JetBrainsMonoNerdFont-Bold.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    //families = QFontDatabase::applicationFontFamilies(fontId);
    //qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //JetBrainsMono NF

    fontId = QFontDatabase::addApplicationFont(":/Res/font/chinese-simplify.ttf");
    if (fontId == -1) {
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";
        return;
    }
    //auto families = QFontDatabase::applicationFontFamilies(fontId);
    //qDebug() << "Loaded font families:" << families; // 输出实际字体名称    //dingliehuobanfont
}

void KuGouApp::initUi() {
    //初始化字体资源
    initFontRes();

    this->setWindowIcon(QIcon(QStringLiteral(":/Res/window/windowIcon.png")));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    //移动窗口到合适的地方
    move(QGuiApplication::primaryScreen()->geometry().width() / 2 - this->width() / 2, 100);

    //设置鼠标追踪
    this->setMouseTracking(true);
    ui->title_widget->setMouseTracking(true);
    ui->center_widget->setMouseTracking(true);
    ui->play_widget->setMouseTracking(true);
    //设置窗口属性
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setAttribute(Qt::WA_Hover, true);

    //隐藏刷新遮罩
    m_refreshMask->hide();
    this->m_refreshMask->setParent(ui->stackedWidget);
    connect(this->m_refreshMask.get(),&RefreshMask::loadingFinished,[this] {
        m_snackbar->addMessage("加载完成");
        m_snackbar->show();
    });
    //短暂显示的消息提示
    m_snackbar->setParent(ui->stackedWidget);
    m_snackbar->setAutoHideDuration(1500);
    m_snackbar->setBackgroundColor(QColor(132, 202, 192, 200));
    m_snackbar->setStyleSheet("border-radius: 10px;");

    initTitleWidget();
    initStackedWidget();
    initPlayWidget();
    initMenu();
    initCornerWidget();
}

void KuGouApp::initStackedWidget() {
    // 使用模板函数统一初始化所有组件
    initComponent(m_live,0);
    initComponent(m_listenBook,1);
    initComponent(m_search,2);
    initComponent(m_recommendForYou,3);
    initComponent(m_musicRepository,4);
    initComponent(m_channel,5);
    initComponent(m_video,6);
    initComponent(m_aiChat,7);
    initComponent(m_songList,8);
    initComponent(m_dailyRecommend,9);
    initComponent(m_collection,10);
    initComponent(m_localDownload,11);
    initComponent(m_musicCloudDisk,12);
    initComponent(m_purchasedMusic,13);
    initComponent(m_recentlyPlayed,14);
    initComponent(m_allMusic,15);

    //响应相关跳转
    connect(this->m_collection.get(),&MyCollection::find_more_music,[this]{on_music_repository_toolButton_clicked();});
    connect(this->m_localDownload.get(),&LocalDownload::find_more_music,[this]{on_music_repository_toolButton_clicked();});
    connect(this->m_musicCloudDisk.get(),&MusicCloudDisk::find_more_music,[this]{on_music_repository_toolButton_clicked();});
    connect(this->m_purchasedMusic.get(),&PurchasedMusic::find_more_music,[this]{on_music_repository_toolButton_clicked();});
    connect(this->m_recentlyPlayed.get(),&RecentlyPlayed::find_more_music,[this]{on_music_repository_toolButton_clicked();});
    connect(this->m_recentlyPlayed.get(),&RecentlyPlayed::find_more_channel,[this]{on_channel_toolButton_clicked();});
    connect(this->m_allMusic.get(),&AllMusic::find_more_music,[this]{on_music_repository_toolButton_clicked();});

    //localDownload
    connect(this->m_localDownload.get(), &LocalDownload::playMusic, this, &KuGouApp::onPlayMusic);
    connect(this->m_localDownload.get(), &LocalDownload::startPlay, this, &KuGouApp::onStartPlay);
    connect(this->m_localDownload.get(), &LocalDownload::addSongInfo, this, &KuGouApp::onAddSongInfo);
    connect(this->m_localDownload.get(), &LocalDownload::subSongInfo, this, &KuGouApp::onSubSongInfo);

    connect(this, &KuGouApp::setPlayIndex, this->m_localDownload.get(), &LocalDownload::setPlayIndex);
    connect(this, &KuGouApp::maxScreen, this->m_localDownload.get(), &LocalDownload::onMaxScreenHandle);

    //专门处理重排
    connect(this->m_localDownload.get(),&LocalDownload::syncSongInfo,this,&KuGouApp::onSyncSongInfoVector);

}

void KuGouApp::initTitleWidget() {
    //响应左侧菜栏的显示与否
    connect(ui->title_widget,&TitleWidget::leftMenuShow,this,&KuGouApp::onLeftMenuShow);
    //响应堆栈窗口切换
    connect(ui->title_widget,&TitleWidget::currentStackChange,this,&KuGouApp::onTitleCurrentStackChange);
    //响应最大化窗口
    connect(ui->title_widget,&TitleWidget::maxScreen,this,&KuGouApp::onTitleMaxScreen);
    //响应关于对话框的显示
    connect(ui->title_widget, &TitleWidget::showAboutDialog, this, [this] {
        MainWindow::onShowAboutDialog(true);
    });
    //响应刷新窗口的显示与否
    connect(ui->title_widget, &TitleWidget::refresh, this, [this] {
        updateSize();
        this->m_refreshMask->showLoading();
        this->m_refreshMask->raise();
    });
    //向titleWidget 发送
    connect(this,&MainWindow::exit,ui->title_widget,&TitleWidget::on_close_toolButton_clicked);
}

void KuGouApp::initPlayWidget() {
    ui->love_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/collect.svg")));
    ui->download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/download.svg")));
    ui->comment_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/comment.svg")));
    ui->share_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/share.svg")));
    ui->more_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/more.svg")));
    ui->pre_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/previous-song.svg")));
    ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg")));
    ui->next_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/next-song.svg")));
    ui->erji_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/together.svg")));
    ui->lyrics_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/song-words.svg")));
    ui->song_queue_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play-list.svg")));

    // 设置快捷键
    new QShortcut(QKeySequence("Space"), this, SLOT(onKeyPause())); // 空格键暂停/播放
    new QShortcut(QKeySequence("Right"), this, SLOT(onKeyRight())); // 右箭头快进
    new QShortcut(QKeySequence("Left"), this, SLOT(onKeyLeft())); // 左箭头快退

    //设置tooltip
    MainWindow::setElaToolTip(ui->love_toolButton, "我喜欢");
    MainWindow::setElaToolTip(ui->download_toolButton, "下载");
    MainWindow::setElaToolTip(ui->comment_toolButton, "评论");
    MainWindow::setElaToolTip(ui->share_toolButton, "分享");
    MainWindow::setElaToolTip(ui->more_toolButton, "更多");
    MainWindow::setElaToolTip(ui->circle_toolButton, "循环播放");
    MainWindow::setElaToolTip(ui->pre_toolButton, "上一首");
    MainWindow::setElaToolTip(ui->play_or_pause_toolButton, "播放/暂停");
    MainWindow::setElaToolTip(ui->next_toolButton, "下一首");
    MainWindow::setElaToolTip(ui->volume_toolButton, "调节音量");
    MainWindow::setElaToolTip(ui->stander_pushButton, "音质选择");
    MainWindow::setElaToolTip(ui->acoustics_pushButton, "音效");
    MainWindow::setElaToolTip(ui->erji_toolButton, "邀请好友一起听");
    MainWindow::setElaToolTip(ui->lyrics_toolButton, "打开桌面歌词");
    MainWindow::setElaToolTip(ui->song_queue_toolButton, "播放队列");
    //MainWindow::setElaToolTip(ui->song_name_text,"未知歌曲");
    //MainWindow::setElaToolTip(ui->singer_text,"未知歌手");
    auto song_name_text_toolTip = new ElaToolTip(ui->song_name_text);
    song_name_text_toolTip->setToolTip("未知歌曲");
    auto singer_text_toolTip = new ElaToolTip(ui->singer_text);
    singer_text_toolTip->setToolTip("未知歌手");

    auto font = QFont("AaSongLiuKaiTi");
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    ui->song_name_text->setFont(font);
    ui->singer_text->setFont(font);

    connect(this, &KuGouApp::curPlaySongNameChange, [this, song_name_text_toolTip](const QString& songName) {
         song_name_text_toolTip->setToolTip(songName);
         // 强制布局更新以确保获取最新宽度
         ui->song_name_text->updateGeometry();
         const QFontMetrics fm(ui->song_name_text->font());
         QString elidedText = fm.elidedText(songName, Qt::ElideRight, ui->song_name_text->width());
         ui->song_name_text->setText(elidedText);
         song_name_text_toolTip->adjustSize(); // 调整ToolTip尺寸
     });

    connect(this, &KuGouApp::curPlaySingerChange, [this, singer_text_toolTip](const QString& singerName) {
        singer_text_toolTip->setToolTip(singerName);
        ui->singer_text->updateGeometry();
        const QFontMetrics fm(ui->singer_text->font());
        ui->singer_text->setText(fm.elidedText(singerName, Qt::ElideRight, ui->singer_text->width()));
        singer_text_toolTip->adjustSize();
    });


    this->m_player->setAudioOutput(this->m_audioOutput.get());
    this->m_audioOutput->setVolume(0.2);
    connect(ui->volume_toolButton, &VolumeToolBtn::volumeChange, this, [this](const int value) {
        const float volume = static_cast<float>(value) / 100; // 将值转换为0.0到1.0之间
        this->m_audioOutput->setVolume(volume); // 设置音量
    });
    connect(this,&MainWindow::fromTray_noVolume,this,[this](const bool& flag) {
        STREAM_INFO()<<"KuGouApp 托盘图标点击: "<<(flag?"静音":"开启声音");
        if ((flag && ui->volume_toolButton->getVolumeValue()) || (!flag && !ui->volume_toolButton->getVolumeValue())) {
            // 立即触发 Enter
            QCoreApplication::sendEvent(ui->volume_toolButton, new QEvent(QEvent::Enter));
            ui->volume_toolButton->clicked();
            //qDebug()<<"当前音量："<<ui->volume_toolButton->getVolumeValue();
        }
    });

    connect(this->m_player.get(), &QMediaPlayer::positionChanged, this, [this](int position) {
        if (ui->progressSlider->isSliderDown())return;
        //qDebug()<<"position "<<position;
        ui->progressSlider->setValue(position);
        ui->position_label->setText(QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss"));
    });
    connect(this->m_player.get(), &QMediaPlayer::durationChanged, this, &KuGouApp::updateSliderRange);
    connect(this->m_player.get(), &QMediaPlayer::metaDataChanged, this, [this] {
        //qDebug() << "metaDataChanged";
        update_cover_singer_song_HLayout();
    });
    connect(this->m_player.get(), &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::PlayingState)this->m_isPlaying = true;
        else this->m_isPlaying = false;
        if (this->m_isPlaying) {
            ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg")));
        } else {
            ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg")));
        }
    });
    mediaStatusConnection = connect(this->m_player.get(), &QMediaPlayer::mediaStatusChanged, this,
                                    [=](const QMediaPlayer::MediaStatus &status) {
                                        if (status == QMediaPlayer::EndOfMedia) {
                                            if (this->m_isOrderPlay) {
                                                qDebug()<<"结束，开始播放下一首";
                                                STREAM_INFO() << "结束，开始播放下一首";
                                                addOrderIndex();
                                            }
                                        }
                                    });
    ui->progressSlider->installEventFilter(this);
    connect(ui->progressSlider, &QSlider::sliderReleased, this, &KuGouApp::updateProcess);
    connect(ui->progressSlider, &QSlider::sliderMoved, this, &KuGouApp::updateProcess);
    connect(ui->play_widget, &PlayWidget::doubleClicked, this, &KuGouApp::onTitleMaxScreen);
}

void KuGouApp::initMenu() {
    //发现音乐
    this->m_menuBtnGroup->setParent(ui->center_menu_widget);

    ui->recommend_you_toolButton    ->setIcon(QIcon(QStringLiteral(":/Res/window/recommend.svg")));
    ui->music_repository_toolButton ->setIcon(QIcon(QStringLiteral(":/Res/window/music-library.svg")));
    ui->channel_toolButton          ->setIcon(QIcon(QStringLiteral(":/Res/window/my-channel.svg")));
    ui->video_toolButton            ->setIcon(QIcon(QStringLiteral(":/Res/window/video.svg")));
    ui->live_toolButton             ->setIcon(QIcon(QStringLiteral(":/Res/window/live.svg")));
    ui->ai_chat_toolButton          ->setIcon(QIcon(QStringLiteral(":/Res/window/ai-chat.svg")));
    ui->song_list_toolButton        ->setIcon(QIcon(QStringLiteral(":/Res/window/song-list.svg")));
    ui->daily_recommend_toolButton  ->setIcon(QIcon(QStringLiteral(":/Res/window/daily.svg")));
    //我的音乐
    ui->my_collection_toolButton    ->setIcon(QIcon(QStringLiteral(":/Res/window/collect.svg")));
    ui->local_download_toolButton   ->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg")));
    ui->music_cloud_disk_toolButton ->setIcon(QIcon(QStringLiteral(":/Res/window/cloud.svg")));
    ui->purchased_music_toolButton  ->setIcon(QIcon(QStringLiteral(":/Res/window/bought.svg")));
    ui->recently_played_toolButton  ->setIcon(QIcon(QStringLiteral(":/Res/window/history.svg")));
    ui->all_music_toolButton        ->setIcon(QIcon(QStringLiteral(":/Res/titlebar/menu-black.svg")));
    //互斥
    m_menuBtnGroup->addButton(ui->recommend_you_toolButton);
    m_menuBtnGroup->addButton(ui->music_repository_toolButton);
    m_menuBtnGroup->addButton(ui->channel_toolButton);
    m_menuBtnGroup->addButton(ui->video_toolButton);
    m_menuBtnGroup->addButton(ui->live_toolButton);
    m_menuBtnGroup->addButton(ui->ai_chat_toolButton);
    m_menuBtnGroup->addButton(ui->song_list_toolButton);
    m_menuBtnGroup->addButton(ui->daily_recommend_toolButton);

    m_menuBtnGroup->addButton(ui->my_collection_toolButton);
    m_menuBtnGroup->addButton(ui->local_download_toolButton);
    m_menuBtnGroup->addButton(ui->music_cloud_disk_toolButton);
    m_menuBtnGroup->addButton(ui->purchased_music_toolButton);
    m_menuBtnGroup->addButton(ui->recently_played_toolButton);
    m_menuBtnGroup->addButton(ui->all_music_toolButton);
    m_menuBtnGroup->setExclusive(true);
}

void KuGouApp::initCornerWidget() {
    this->m_sizeGrip->setFixedSize(11, 11);
    this->m_sizeGrip->setObjectName(QStringLiteral("sizegrip"));
}

int KuGouApp::getCurrentIndex(int index) {
    if (index < 0 || index > static_cast<int>(this->m_lastSongInfoVector.size())||this->m_lastSongInfoVector.isEmpty()) {
        //qDebug() << "获取下标错误";
        return -1;
    }
    if(index == this->m_lastSongInfoVector.size())--index;
    //直接默认从旧的排序中找
    //qDebug()<<"旧排序中，在 "<<index;
    SongInfor temp;
    temp.songName = this->m_lastSongInfoVector[index].songName;
    temp.singer = this->m_lastSongInfoVector[index].singer;
    temp.duration = this->m_lastSongInfoVector[index].duration;
    auto index1 = std::find(m_songInfoVector.begin(), m_songInfoVector.end(), temp);

    if(index1 == m_songInfoVector.end()) {//删除的是自己本身
        //qDebug()<<"找到了 ======== "<<index-1;
        return index-1;
    }
    index = static_cast<int>(index1 - m_songInfoVector.begin());
    //qDebug()<<"找到，现在是 ======== "<<index;
    return index;
}

void KuGouApp::update_cover_singer_song_HLayout() {
    QString song_name;
    QString singer;
    if (this->m_isOrderPlay) {
        ui->cover_label->setPixmap(roundedPixmap(this->m_songInfoVector[this->m_orderIndex].cover,
                                                 ui->cover_label->size(), 8));
        song_name = this->m_songInfoVector[this->m_orderIndex].songName;
        singer = this->m_songInfoVector[this->m_orderIndex].singer;
    }
    else {
        ui->cover_label->setPixmap(roundedPixmap(this->m_songInfoVector[this->m_songIndex].cover,
                                                 ui->cover_label->size(), 8));
        song_name= this->m_songInfoVector[this->m_songIndex].songName;
        singer= this->m_songInfoVector[this->m_songIndex].singer;
    }
    emit curPlaySongNameChange(song_name);
    emit curPlaySingerChange(singer);
    const QFontMetrics fm(ui->song_name_text->font());
    ui->song_name_text->setText(fm.elidedText(song_name,Qt::ElideRight,ui->song_name_text->width()));
    ui->singer_text->setText(fm.elidedText(singer,Qt::ElideRight,ui->singer_text->width()));
}

void KuGouApp::updateSize() {
    QResizeEvent resizeEvent(this->size(), this->size());
    QApplication::sendEvent(this, &resizeEvent);
    //this->resize(this->size().width()+1,this->size().height()+1);
}

void KuGouApp::enableButton(const bool &flag) {
    ui->recommend_you_toolButton    ->setEnabled(flag);
    ui->music_repository_toolButton ->setEnabled(flag);
    ui->song_list_toolButton        ->setEnabled(flag);
    ui->channel_toolButton          ->setEnabled(flag);
    ui->video_toolButton            ->setEnabled(flag);
    ui->live_toolButton             ->setEnabled(flag);
    ui->ai_chat_toolButton          ->setEnabled(flag);
    ui->daily_recommend_toolButton  ->setEnabled(flag);
    ui->my_collection_toolButton    ->setEnabled(flag);
    ui->local_download_toolButton   ->setEnabled(flag);
    ui->music_cloud_disk_toolButton ->setEnabled(flag);
    ui->purchased_music_toolButton  ->setEnabled(flag);
    ui->recently_played_toolButton  ->setEnabled(flag);
    ui->all_music_toolButton        ->setEnabled(flag);
    ui->title_widget->setEnableChange(flag);
}

void KuGouApp::addOrderIndex() {
    this->m_orderIndex = (this->m_orderIndex + 1) % static_cast<int>(this->m_songInfoVector.size());
    this->m_songIndex = this->m_orderIndex;
    setPlayMusic(this->m_orderIndex);
}

void KuGouApp::subOrderIndex() {
    int s = static_cast<int>(this->m_songInfoVector.size());
    this->m_orderIndex = (this->m_orderIndex + s - 1) % s;
    this->m_songIndex = this->m_orderIndex;
    setPlayMusic(this->m_orderIndex);
}

void KuGouApp::addSongIndex() {
    //qDebug()<<"之前正在播放第 "<<this->m_songIndex<<" 首歌";
    this->m_songIndex = (this->m_songIndex + 1) % static_cast<int>(this->m_songInfoVector.size());;
    //qDebug()<<"现在播放第 "<<this->m_songIndex<<" 首歌";
    setPlayMusic(this->m_songIndex);
    //qDebug()<<"设置高亮成功";
}

void KuGouApp::subSongIndex() {
    int s = static_cast<int>(this->m_songInfoVector.size());
    this->m_songIndex = (this->m_songIndex + s - 1) % s;;
    setPlayMusic(this->m_songIndex);
}

void KuGouApp::mousePressEvent(QMouseEvent *ev) {
    MainWindow::mousePressEvent(ev);
    if (this->m_isTransForming)return;
    if (ev->button() == Qt::LeftButton) {
        this->m_pressPos = ev->pos();
        // 获取鼠标在那个区域
    }
}

void KuGouApp::mouseMoveEvent(QMouseEvent *event) {
    MainWindow::mouseMoveEvent(event);
    if (this->m_isTransForming)return;
    // 计算的鼠标移动偏移量, 就是鼠标全局坐标 - 减去点击时鼠标坐标
    point_offset = event->globalPosition().toPoint() - mousePs;

    if (isPress) {
        if (mouse_press_region == kMousePositionMid) {
            if(ui->title_widget->geometry().contains(this->m_pressPos)) {
                if (this->m_isMaxScreen == true) //如果是最大化拖动，则还原大小再拖动
                {
                    this->m_startGeometry.setX(event->pos().x() - this->m_normalGeometry.width() / 2);
                    this->m_startGeometry.setY(event->pos().y() - 20);
                    this->m_startGeometry.setWidth( this->m_normalGeometry.width());
                    this->m_startGeometry.setHeight( this->m_normalGeometry.height());

                    ui->title_widget->max_toolButton()->clicked();
                    return;
                }
                move(windowsLastPs + point_offset);
            }
            if(ui->play_widget->geometry().contains(this->m_pressPos)) {
                if (this->m_isMaxScreen == true) //如果是最大化拖动，则还原大小再拖动
                {
                    this->m_startGeometry.setX(event->pos().x() - this->m_normalGeometry.width() / 2);
                    this->m_startGeometry.setY(event->pos().y() - this->m_normalGeometry.height()+20);
                    this->m_startGeometry.setWidth( this->m_normalGeometry.width());
                    this->m_startGeometry.setHeight( this->m_normalGeometry.height());
                    ui->title_widget->max_toolButton()->clicked();
                    return;
                }
                move(windowsLastPs + point_offset);
            }
        }
    }
}

void KuGouApp::paintEvent(QPaintEvent *ev) {
    MainWindow::paintEvent(ev);
}

void KuGouApp::resizeEvent(QResizeEvent *event) {
    MainWindow::resizeEvent(event);
    //qDebug()<<"KuGouApp::resizeEvent , size = "<<event->size();
    if(this->geometry() != this->screen()->availableGeometry()) {
        //一旦拖动边框改变大小，就不再是全屏
        //qDebug()<<"全屏";
        this->m_isMaxScreen = false;
    }
    else this->m_isMaxScreen = true;
    //角标移动
    this->m_sizeGrip->move(this->width() - this->m_sizeGrip->width() - 8,
                           this->height() - this->m_sizeGrip->height() - 8);
    this->m_sizeGrip->raise();
    this->m_sizeGrip->setVisible(true);
    //song_info_widget适度延展
    ui->song_info_widget->setFixedWidth(this->width()/8+20);
    //更新文字数量
    if(!this->m_player->source().isEmpty() && !this->m_songInfoVector.isEmpty())update_cover_singer_song_HLayout();
    //刷新遮罩大小同步
    auto rect = ui->stackedWidget->geometry();
    rect.setLeft(5);
    rect.setRight(rect.width()-6);
    this->m_refreshMask->setGeometry(rect);
}

bool KuGouApp::event(QEvent *event) {
    if (QEvent::HoverMove == event->type()) //鼠标移动
    {
        //auto ev = dynamic_cast<QMouseEvent*>(event);
        //if (ev) {
        //    this->mouseMoveEvent(ev);
        //}

        // 此处最好用static_cast
        auto ev = static_cast<QMouseEvent *>(event);
        this->mouseMoveEvent(ev);
        return true;
    }

     return MainWindow::event(event);
 }

bool KuGouApp::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->progressSlider) {
        // 禁用进度条拖拽，当播放器没有加载资源时
        // 如果播放器没有加载资源，禁用进度条的鼠标事件
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove) {
            if (this->m_player->source().isEmpty()) {
                return true; // 只拦截与鼠标有关的事件
            }
        }
        if (event->type() == QEvent::MouseButtonPress) //判断类型
        {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
                //qDebug() << "触发点击";
                qint64 value = QStyle::sliderValueFromPosition(ui->progressSlider->minimum(),
                                                               ui->progressSlider->maximum(), mouseEvent->pos().x(),
                                                               ui->progressSlider->width());
                {
                    //水平进度条动态地划到点击位置
                    //auto ani = new QPropertyAnimation(ui->progressSlider,"sliderPosition");
                    // 设置动画持续时间，单位是毫秒，这里设置为100毫秒
                    //ani->setDuration(100);
                    // 动画开始值为滑块的当前位置
                    //ani->setStartValue(ui->progressSlider->value());
                    // 动画结束值为你希望的目标值
                    //ani->setEndValue(value);
                    // 设置动画的曲线类型，比如缓入缓出的效果
                    //ani->setEasingCurve(QEasingCurve::Linear);
                    // 启动动画
                    //ani->start(QAbstractAnimation::DeleteWhenStopped);
                }

                this->m_player->setPosition(value);
                if (!this->m_isPlaying)ui->play_or_pause_toolButton->clicked();
            }
        }
    }
    return MainWindow::eventFilter(watched, event);
}

void KuGouApp::showEvent(QShowEvent *event) {
    MainWindow::showEvent(event);
    updateSize();
}

void KuGouApp::on_recommend_you_toolButton_clicked() {
    ui->title_widget->onLeftMenu_recommend_clicked();
}

void KuGouApp::on_music_repository_toolButton_clicked() {
    ui->title_widget->onLeftMenu_musicRepository_clicked();
}

void KuGouApp::on_channel_toolButton_clicked() {
    ui->title_widget->onLeftMenu_channel_clicked();
}

void KuGouApp::on_video_toolButton_clicked() {
    ui->title_widget->onLeftMenu_video_clicked();
}

void KuGouApp::on_live_toolButton_clicked() {
    ui->title_widget->onLeftMenu_live_clicked();
}

void KuGouApp::on_ai_chat_toolButton_clicked() {
    ui->title_widget->onLeftMenu_ai_chat_clicked();
}

void KuGouApp::on_song_list_toolButton_clicked() {
    ui->title_widget->onLeftMenu_songList_clicked();
}

void KuGouApp::on_daily_recommend_toolButton_clicked() {
    ui->title_widget->onLeftMenu_dailyRecommend_clicked();
}

void KuGouApp::on_my_collection_toolButton_clicked() {
    ui->title_widget->onLeftMenu_collection_clicked();
}

void KuGouApp::on_local_download_toolButton_clicked() {
    ui->title_widget->onLeftMenu_localDownload_clicked();
}

void KuGouApp::on_music_cloud_disk_toolButton_clicked() {
    ui->title_widget->onLeftMenu_musicCloudDisk_clicked();
}

void KuGouApp::on_purchased_music_toolButton_clicked() {
    ui->title_widget->onLeftMenu_purchasedMusic_clicked();
}

void KuGouApp::on_recently_played_toolButton_clicked() {
    ui->title_widget->onLeftMenu_recentlyPlayed_clicked();
}

void KuGouApp::on_all_music_toolButton_clicked() {
    ui->title_widget->onLeftMenu_allMusic_clicked();
}

void KuGouApp::setPlayMusic(int &index) {
    //qDebug()<<"设置第 "<<index<<" 首高亮";
    emit setPlayIndex(index);
    this->m_player->stop();
    this->m_player->setSource(QUrl(this->m_songInfoVector[index].mediaPath));
    this->m_player->play();
}

void KuGouApp::updateProcess() {
    //qDebug()<<"sliderMoved / sliderReleased : "<<ui->progressSlider->value();
    qint64 position = ui->progressSlider->value() * this->m_player->duration() / ui->progressSlider->maximum();
    this->m_player->setPosition(position);
    this->m_player->play();
}

void KuGouApp::updateSliderRange(const qint64 &duration) {
    ui->progressSlider->setMaximum(static_cast<int>(duration));
    //qDebug()<<"改变总时长";
    ui->duration_label->setText(QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("mm:ss"));
}

void KuGouApp::onPlayMusic(int index) {
    //index = getCurrentIndex(index-1);
    this->m_isOrderPlay = false;
    this->m_songIndex = index;
    setPlayMusic(this->m_songIndex);
}

void KuGouApp::onStartPlay() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
    if(this->m_isSingleCircle)ui->circle_toolButton->clicked();
    if(this->m_songInfoVector.isEmpty())return;
    this->m_isOrderPlay = true;
    this->m_orderIndex = 0;
    //直接从第一首开始播放
    setPlayMusic(this->m_orderIndex);
}

void KuGouApp::onAddSongInfo(const SongInfor &info) {
    this->m_songInfoVector.emplace_back(info);
    this->m_lastSongInfoVector.emplace_back(info);
}

void KuGouApp::onSubSongInfo(const SongInfor &info) {
    auto it = std::find(this->m_songInfoVector.begin(),this->m_songInfoVector.end(),info);
    this->m_songInfoVector.erase(it);
    //auto it2 = std::find(this->m_lastSongInfoVector.begin(),this->m_lastSongInfoVector.end(),info);
    //this->m_lastSongInfoVector.erase(it2);

    //更新下标
    int index = -1;
    for(auto& val : this->m_songInfoVector) {
        val.index = ++index;
    }
    //如果之前设置了循环，则改为初始状态
    if(this->m_songInfoVector.isEmpty()) {
        this->m_isOrderPlay = false;
        this->m_isSingleCircle = false;
        ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url(':/Res/playbar/list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url(':/Res/playbar/list-loop-blue.svg');})");
    }
    //更新当前播放下标
    this->m_songIndex = getCurrentIndex(this->m_songIndex);
    this->m_orderIndex = getCurrentIndex(this->m_orderIndex);

    this->m_lastSongInfoVector =  this->m_songInfoVector;
}

void KuGouApp::onKeyPause() {
    if (this->m_player->playbackState() == QMediaPlayer::PlaybackState::PlayingState) {
        this->m_player->pause();
    } else {
        this->m_player->play();
    }
}

void KuGouApp::onKeyLeft() {
    qint64 pos = this->m_player->position() - 5000;
    this->m_player->setPosition(pos < 0 ? 0 : pos);
}

void KuGouApp::onKeyRight() {
    qint64 pos = this->m_player->position() + 5000;
    this->m_player->setPosition(pos > this->m_player->duration() ? this->m_player->duration() : pos);
}

void KuGouApp::onTitleCurrentStackChange(const int &index,const bool& slide) {
    if (ui->stackedWidget->currentIndex() == index)return;
    this->m_refreshMask->hide();
    this->m_snackbar->hide();
    if (slide) enableButton(false),ui->stackedWidget->slideInIdx(index);
    else ui->stackedWidget->setCurrentIndex(index);
    switch (index) {
        case 3:
            ui->recommend_you_toolButton->setChecked(true);
            break;
        case 4:
            ui->music_repository_toolButton->setChecked(true);
            break;
        case 5:
            ui->channel_toolButton->setChecked(true);
            break;
        case 6:
            ui->video_toolButton->setChecked(true);
            break;
        case 7:
            ui->ai_chat_toolButton->setChecked(true);
            break;
        case 8:
            ui->song_list_toolButton->setChecked(true);
            break;
        case 9:
            ui->daily_recommend_toolButton->setChecked(true);
            break;
        case 10:
            ui->my_collection_toolButton->setChecked(true);
            break;
        case 11:
            ui->local_download_toolButton->setChecked(true);
            break;
        case 12:
            ui->music_cloud_disk_toolButton->setChecked(true);
            break;
        case 13:
            ui->purchased_music_toolButton->setChecked(true);
            break;
        case 14:
            ui->recently_played_toolButton->setChecked(true);
            break;
        case 15:
            ui->all_music_toolButton->setChecked(true);
            break;
        default: break;
    }
    updateSize();
}

void KuGouApp::onLeftMenuShow(const bool &flag) const {
    if (flag) {
        ui->menu_scrollArea->show();
    } else {
        ui->menu_scrollArea->hide();
    }
}

void KuGouApp::onTitleMaxScreen() {
    STREAM_INFO()<<"最大化窗口";
    if (m_isMaxScreen) {
        this->m_isMaxScreen = false;//现在是正常化
        m_endGeometry = m_startGeometry; // 获取普通尺寸时的几何形状
        m_startGeometry = this->screen()->availableGeometry();
        //this->setGeometry(m_startGeometry); // 恢复前，我们先把它设置回最大化尺寸
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url(":/Res/titlebar/maximize-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })";
        ui->title_widget->max_toolButton()->setMyIcon(QIcon(":/Res/titlebar/maximize-black.svg"));
        this->m_animation->setDuration(500); // 设置动画持续时间
    }
    else {
        this->m_normalGeometry = this->geometry();//最大化之前获取geometry
        this->m_isMaxScreen = true;//现在是最大化
        // 如果当前不是最大化状态，则目标是最大化
        m_startGeometry = this->m_normalGeometry;
        m_endGeometry = this->screen()->availableGeometry(); // 获取屏幕的最大化尺寸
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url(":/Res/titlebar/resume-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })";
        ui->title_widget->max_toolButton()->setMyIcon(QIcon(":/Res/titlebar/resume-black.svg"));
        this->m_animation->setDuration(400); // 设置动画持续时间
    }
    //qDebug()<<"start : "<<this->m_startGeometry<<" end : "<<this->m_endGeometry;
    this->m_animation->setStartValue(m_startGeometry); // 动画开始时的窗口几何
    this->m_animation->setEndValue(m_endGeometry); // 动画结束时的窗口几何
    this->m_animation->setEasingCurve(QEasingCurve::InOutQuad); // 设置动画的缓动曲线

    this->m_isTransForming = true; // 正在变形，禁用点击、拖动事件
    // 开始动画
    this->m_animation->start();
    // 在动画结束后标记动画停止
    connect(this->m_animation.get(), &QPropertyAnimation::finished, this, [this] {
        // 动画结束后，延迟 100 毫秒启用拖拽
        QTimer::singleShot(100, this, [this] {
            this->m_isTransForming = false; // 启用拖拽
        });
        if(this->m_endGeometry == this->screen()->availableGeometry())emit maxScreen();//高亮条延伸
    });

    // 更新按钮样式
    ui->title_widget->max_toolButton()->setStyleSheet(this->m_maxBtnStyle);
}

void KuGouApp::onSyncSongInfoVector(QVector<SongInfor> &vec) {
    //保留旧的
    this->m_lastSongInfoVector = this->m_songInfoVector;
    //获取同步的播放顺序
    this->m_songInfoVector = vec;
    //qDebug()<<"重排，更新为如下：";
    //for(auto& val : this->m_songInfoVector) {
    //    qDebug()<<val.songName;
    //}
    //更新当前播放下标
    this->m_songIndex = getCurrentIndex(this->m_songIndex);
    this->m_orderIndex = getCurrentIndex(this->m_orderIndex);

}

void KuGouApp::on_play_or_pause_toolButton_clicked() {
    //如果未设置播放源就return
    if (this->m_player->source().isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
    this->m_isPlaying = !this->m_isPlaying;
    if (this->m_isPlaying) {
        this->m_player->play();
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/pause.svg")));
    } else {
        this->m_player->pause();
        ui->play_or_pause_toolButton->setIcon(QIcon(QStringLiteral(":/Res/playbar/play.svg")));
    }
}

void KuGouApp::on_love_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()){
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
}

void KuGouApp::on_download_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
}

void KuGouApp::on_comment_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
}

void KuGouApp::on_share_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
}

void KuGouApp::on_more_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
    }
}

void KuGouApp::on_circle_toolButton_clicked() {
    if (this->m_player->source().isEmpty()|| this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
    m_isSingleCircle = !m_isSingleCircle;
    if (m_isSingleCircle) {
        this->m_isOrderPlay = false;
        //qDebug()<<"单曲循环";
        //this->m_player->setLoops(QMediaPlayer::Loops::Infinite);
        ////怪不得，原来错在这里，我就说怎么循环播放进度条一直有问题，服了
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url(':/Res/playbar/single-list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url(':/Res/playbar/single-list-loop-blue.svg');})");
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection);
            mediaStatusConnection = connect(this->m_player.get(), &QMediaPlayer::mediaStatusChanged, this,
                                            [=](QMediaPlayer::MediaStatus status) {
                                                if (status == QMediaPlayer::EndOfMedia) {
                                                    if(this->m_songInfoVector.isEmpty())return;
                                                    //qDebug()<<"播放结束";
                                                    // 当播放结束时，重新开始播放
                                                    //qDebug()<<"循环播放 ："<<this->m_isSingleCircle;
                                                    this->m_player->stop(); // 设置到文件的开头
                                                    //this->m_player->play();
                                                    setPlayMusic(this->m_songIndex);
                                                }
                                            });
        } else {
            qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty";
        }
    } else {
        //qDebug()<<"播放一次";
        if (mediaStatusConnection) {
            disconnect(mediaStatusConnection);
            mediaStatusConnection = connect(this->m_player.get(), &QMediaPlayer::mediaStatusChanged, this,
                                            [=](QMediaPlayer::MediaStatus status) {
                                                if (status == QMediaPlayer::EndOfMedia) {
                                                    if(this->m_songInfoVector.isEmpty())return;
                                                    if (this->m_isOrderPlay) {
                                                        //qDebug()<<"结束，开始播放下一首";
                                                        addOrderIndex();
                                                    }
                                                }
                                            });
        } else {
            qDebug() << "mediaStatusConnection is empty";
            STREAM_WARN() << "mediaStatusConnection is empty";
        }
        ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url(':/Res/playbar/list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url(':/Res/playbar/list-loop-blue.svg');})");
    }
}

void KuGouApp::on_pre_toolButton_clicked() {
    if (this->m_player->source().isEmpty()|| this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
    if (this->m_isOrderPlay) subOrderIndex();
    else subSongIndex();
}

void KuGouApp::on_next_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
    if (this->m_isOrderPlay) addOrderIndex();
    else addSongIndex();
}

void KuGouApp::on_stander_pushButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                                QStringLiteral("音质选择功能 暂未实现 敬请期待"),
                                1000,this->window());
        return;
    }
}

void KuGouApp::on_acoustics_pushButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                                QStringLiteral("音效功能 暂未实现 敬请期待"),
                                1000,this->window());
        return;
    }
}

void KuGouApp::on_erji_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
}

void KuGouApp::on_lyrics_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
}

void KuGouApp::on_song_queue_toolButton_clicked() {
    if (this->m_player->source().isEmpty() || this->m_songInfoVector.isEmpty()) {
        ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                                QStringLiteral("播放队列功能 暂未实现 敬请期待"),
                                1000,this->window());
        return;
    }
}
