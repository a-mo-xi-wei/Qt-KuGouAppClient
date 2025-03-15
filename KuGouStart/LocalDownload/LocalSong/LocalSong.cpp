//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LocalSong.h" resolved

#include "LocalSong.h"
#include "ui_LocalSong.h"
#include "logger.hpp"

#include <QFileDialog>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QScreen>
#include <QStandardPaths>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))
//匹配是否有乱码
static QRegularExpression re(QStringLiteral("^[A-Za-z0-9\\p{Han}\\\\/\\-_\\*]+$"));

LocalSong::LocalSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocalSong)
    , m_player(std::make_unique<QMediaPlayer>(this))
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/localsong.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    getMetaData();
    initUi();
    //选中相关
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this);
    m_sortOptMenu = menu->getMenu<SortOptionMenu>();

    connect(m_sortOptMenu, &SortOptionMenu::selected, this, [this] {
        //ui->local_sort_toolButton->setStyleSheet("border-image:url(':/Res/titlebar/sort-blue.svg');");
        ui->local_sort_toolButton->setStyleSheet("QToolButton{border-image:url(':/Res/titlebar/sort-blue.svg');}");
    });
    connect(m_sortOptMenu, &SortOptionMenu::deselected, this, [this] {
        //qDebug()<<"接收到无排序状态";
        ui->local_sort_toolButton->setStyleSheet(R"(
                QToolButton{border-image:url(':/Res/titlebar/sort-gray.svg');}
                QToolButton:hover{border-image:url(':/Res/titlebar/sort-blue.svg');})");
    });
    //排序相关
    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, &LocalSong::onDefaultSort);
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, &LocalSong::onAddTimeSort);
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, &LocalSong::onSongNameSort);
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, &LocalSong::onSingerSort);
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, &LocalSong::onDurationSort);
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, &LocalSong::onPlayCountSort);
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, &LocalSong::onRandomSort);

    //获取父类指针
    this->m_parent = this->window();

    this->m_vScrollBar = ui->scrollArea->verticalScrollBar();
}

LocalSong::~LocalSong() {
    delete ui;
}

void LocalSong::initUi() {
    const auto layout = ui->local_song_list_widget->layout();
    layout->setSpacing(2);
    layout->addItem(new QSpacerItem(1, 40, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->setContentsMargins(0, 0, 0, 0);

    ui->local_all_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->local_add_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/add-gray.svg")));
    ui->upload_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/upload-cloud-gray.svg")));

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->local_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->local_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->local_search_lineEdit->findChildren<QToolButton*>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            break;
        }
    }

    // 安装事件过滤器
    if (searchButton) {
        searchButton->installEventFilter(this);
    }

    //先直接往里面嵌入两首歌
    this->m_mediaPath = QStringLiteral("qrc:/Res/audio/zi-jing-hua.mp3");
    this->m_player->setSource(QUrl(this->m_mediaPath));
    this->m_player->play(); // 触发状态改变信号，获取元数据信息
    // 使用 QMetaObject::Connection 跟踪连接
    QMetaObject::Connection* connection = new QMetaObject::Connection;

    *connection = connect(this, &LocalSong::updateCountLabel, this,
        [this, connection]() { // 捕获连接指针
            // 处理第二首歌
            this->m_mediaPath = QStringLiteral("qrc:/Res/audio/qing-hua-ci.mp3");
            this->m_player->setSource(QUrl(this->m_mediaPath));
            this->m_player->play();

            // 断开连接并清理
            QObject::disconnect(*connection);
            delete connection;
        },
        Qt::QueuedConnection // 确保异步执行
    );
}

void LocalSong::getMetaData() {
    //经过一段时间的搜索得到的结论，如果想不调用play()就获得元数据信息是不现实的，能做到的也就只有加载完成之后立马停止
    connect(m_player.get(), &QMediaPlayer::mediaStatusChanged, [=](const QMediaPlayer::MediaStatus &status) {
        if (status == QMediaPlayer::LoadedMedia) {
            //qDebug()<<"媒体状态改变，加载完成";
            //qDebug()<<"元数据加载完成";
            const QMediaMetaData data = this->m_player->metaData();
            //for (auto val: data.keys()) {
                //qDebug()<<val<<": "<<data.value(val).toString();
            //}
            // 停止播放
            this->m_player->stop();
            //获取标题
            auto title = data.value(QMediaMetaData::Title).toString();
            if (!re.match(title).hasMatch()) {
                title = QUrl::fromLocalFile(this->m_mediaPath).fileName();
                title = title.first(title.lastIndexOf('.'));
            }
            //获取歌手
            auto singer = data.value(QMediaMetaData::ContributingArtist).toString();
            if (!re.match(singer).hasMatch())singer = QStringLiteral("网络歌手");
            //获取封面
            auto cover = data.value(QMediaMetaData::ThumbnailImage).value<QPixmap>();
            if (cover.isNull()) {
                //qDebug()<<"封面为空";
                cover = QPixmap(QString(":/Res/tablisticon/pix%1.png").arg(QRandomGenerator::global()->bounded(1, 11)));
            }
            //获取时长
            const auto duration = data.value(QMediaMetaData::Duration).value<qint64>();
            //信息赋值
            SongInfor tempInformation;
            tempInformation.index = static_cast<int>(this->m_locationMusicVector.size());
            tempInformation.cover = cover;
            tempInformation.songName = title;
            tempInformation.singer = singer;
            tempInformation.duration = QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("mm:ss");
            tempInformation.mediaPath = this->m_mediaPath;
            tempInformation.addTime = QDateTime::currentDateTime();
            tempInformation.playCount = 0;
            //判重（通过元数据信息）
            const auto it = std::find(this->m_locationMusicVector.begin(),
                                this->m_locationMusicVector.end(), tempInformation);
            if (it == this->m_locationMusicVector.end())this->m_locationMusicVector.emplace_back(tempInformation);
            else {
                //qDebug()<<title<<"已存在，请勿重复插入";
                //加载下一首歌
                loadNextSong();
                return;
            }
            //向parent发送添加MediaPath的信号
            emit addSongInfo(tempInformation);
            //加载相关信息
            auto item = new MusicItemWidget(tempInformation, this);
            //初始化item
            initMusicItem(item);
            //插入Item
            this->m_MusicItemVector.emplace_back(item);
            const auto layout = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
            if (!layout)return;
            layout->insertWidget(layout->count() - 2, item);
            //ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
            emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size()));

            //加载下一首歌
            loadNextSong();
        }
    });

}

void LocalSong::loadNextSong() {
    if (!m_songQueue.isEmpty()) {
        this->m_mediaPath = m_songQueue.dequeue(); // 取出队列中的下一首歌路径
        //qDebug()<<"取出歌曲 ： "<<this->m_mediaPath<<"=================";

        /*// 在加载新媒体前，重置媒体状态
        this->m_player->stop();
        this->m_player->setSource(QUrl());  // 清空当前媒体源
        this->m_player->play();*/ //不起效果

        // 释放并重建 QMediaPlayer 对象
        this->m_player = std::make_unique<QMediaPlayer>(this); //它会自动释放之前的对象
        getMetaData();

        this->m_player->setSource(QUrl::fromLocalFile(this->m_mediaPath));
        this->m_player->play(); // 触发状态改变信号，获取元数据信息
    }
    else {
        if (this->m_isSorting)
            this->m_sortOptMenu->btnClickAgain();
    }
}

void LocalSong::getMenuPosition(const QPoint &pos) {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    const int menuRightPos = pos.x() + m_sortOptMenu->width();
    const int menuBottomPos = pos.y() + m_sortOptMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
    // 如果菜单下侧超出屏幕下侧（不存在）
    //if(menuTopPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setY(10);
    //}
}

void LocalSong::MySort(std::function<bool(const MusicItemWidget *, const MusicItemWidget *)> comparator) {
    //记录当前的（旧的）vector
    this->m_lastLocationMusicVector = this->m_locationMusicVector;
    if (this->m_lastLocationMusicVector.isEmpty())return;
    //初始UI
    ui->local_song_list_widget->setUpdatesEnabled(false);
    const auto layout = ui->local_song_list_widget->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        //if(item->widget())item->widget()->deleteLater();//不得删除他指向的内容
        delete item;
    }
    layout->setSpacing(2);
    layout->addItem(new QSpacerItem(1, 40, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->setContentsMargins(0, 0, 0, 0);
    // 按特定条件排序
    std::sort(this->m_MusicItemVector.begin(), this->m_MusicItemVector.end(), std::move(comparator));
    // 重新添加
    this->m_locationMusicVector.clear();
    int index = -1;
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
    for (const auto &val: this->m_MusicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 2, val);
        this->m_locationMusicVector.emplace_back(val->m_information);
    }
    emit syncSongInfo(this->m_locationMusicVector);
    // 恢复更新
    ui->local_song_list_widget->setUpdatesEnabled(true);
    update();
    updateCurPlayIndex();
}

void LocalSong::updateCurPlayIndex() {
    //记录m_curPlayIndex;
    if (m_curPlayIndex == -1)return;
    const auto temp = this->m_lastLocationMusicVector[this->m_curPlayIndex];
    //重新赋值m_curPlayIndex
    auto it = std::find(this->m_locationMusicVector.begin(), this->m_locationMusicVector.end(), temp);
    if (it == this->m_locationMusicVector.end()) {
        //没找到，说明删除的是最后一个
        --it;
    }
    this->m_curPlayIndex = static_cast<int>(it - this->m_locationMusicVector.begin());
}

void LocalSong::initMusicItem(MusicItemWidget *item) {
    item->setFillColor(QColor(QStringLiteral("#B0EDF6")));
    item->setRadius(12);
    item->setInterval(1);
    //int index = tempInformation.index;// 捕获当前的 index
    connect(item, &MusicItemWidget::play, this, [item, this] {
        emit playMusic(item->m_information.index);
    });
    connect(item, &MusicItemWidget::deleteSong, this, &LocalSong::onItemDeleteSong);
}

void LocalSong::on_local_all_play_toolButton_clicked() {
    emit startPlay();
}

void LocalSong::on_local_add_toolButton_clicked() {
    const QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("添加音乐"), musicPath,
                                                      "Music (*.mp3 *.aac *.wav)");
    if (paths.isEmpty())return;
    //QString fileName = QUrl::fromLocalFile(path).fileName();
    //qDebug() << "插入："<<paths.size()<<"条数据";
    for (auto &path: paths) {
        qDebug()<<"添加歌曲 ："<<path;
        STREAM_INFO()<<"添加歌曲 ："<<path.toStdString();
        this->m_songQueue.enqueue(path);
    }
    this->loadNextSong();
}

void LocalSong::on_local_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->move(this->m_menuPosition);
    this->m_sortOptMenu->show();
}

void LocalSong::setPlayIndex(const int &index) {
    if (this->m_locationMusicVector.isEmpty())return;
    this->m_setPlayIndex = index;
    //qDebug()<<"开始播放第 : "<<index<<" 首歌";
    //qDebug()<<m_curPlayIndex<<"**********";
    if (this->m_curPlayIndex == -1) {
        this->m_curPlayIndex = index;
        const auto widget = m_MusicItemVector[index];
        //增加播放次数
        widget->m_information.playCount++;
        //qDebug()<<"第 "<<index<<"首歌增加次数至："<<widget->m_information.playCount;
        widget->setPlayState(true);
    } else {
        auto widget = m_MusicItemVector[this->m_setPlayIndex]; //为了好增加次数
        //增加播放次数
        widget->m_information.playCount++;
        //qDebug()<<"第 "<<index<<"首歌增加次数至："<<widget->m_information.playCount;
        if (this->m_setPlayIndex != this->m_curPlayIndex) {
            widget->setPlayState(true);
            widget = m_MusicItemVector[this->m_curPlayIndex];
            widget->setPlayState(false);
            this->m_curPlayIndex = this->m_setPlayIndex;
        } else {
            widget->setPlayState(true);
        }
    }
}

void LocalSong::onMaxScreenHandle() {
    if (this->m_locationMusicVector.isEmpty())return;
    if (this->m_curPlayIndex != -1) {
        //qDebug()<<"正在播放第 : "<<this->m_curPlayIndex<<" 首歌";
        const auto widget = m_MusicItemVector[this->m_curPlayIndex];
        widget->setPlayState(true);
        //qDebug()<<"收到最大化信号";
    }
}

void LocalSong::onDefaultSort() {
    ui->local_sort_toolButton->setToolTip("当前排序方式：默认排序");
    //排序规则
    auto defaultSortItem = [](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return item1->m_information.addTime < item2->m_information.addTime;
    };
    MySort(defaultSortItem);
}

void LocalSong::onAddTimeSort(const bool &down) {
    if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：添加时间降序");
    else ui->local_sort_toolButton->setToolTip("当前排序方式：添加时间升序");

    //排序规则
    auto addTimeSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.addTime > item2->m_information.addTime;
        return item1->m_information.addTime < item2->m_information.addTime;
    };

    MySort(addTimeSortItem);
    this->m_isSorting = true;
}

void LocalSong::onSongNameSort(const bool &down) {
    if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：歌曲名降序");
    else ui->local_sort_toolButton->setToolTip("当前排序方式：歌曲名升序");
    //排序规则
    auto songNameSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.songName > item2->m_information.songName;
        return item1->m_information.songName < item2->m_information.songName;
    };
    MySort(songNameSortItem);
    this->m_isSorting = true;
}

void LocalSong::onSingerSort(const bool &down) {
    if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：歌手降序");
    else ui->local_sort_toolButton->setToolTip("当前排序方式：歌手升序");
    //排序规则
    auto singerSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.singer > item2->m_information.singer;
        return item1->m_information.singer < item2->m_information.singer;
    };
    MySort(singerSortItem);
    this->m_isSorting = true;
}

void LocalSong::onDurationSort(const bool &down) {
    if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：时长降序");
    else ui->local_sort_toolButton->setToolTip("当前排序方式：时长升序");
    //排序规则
    auto durationSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.duration > item2->m_information.duration;
        return item1->m_information.duration < item2->m_information.duration;
    };
    MySort(durationSortItem);
    this->m_isSorting = true;
}

void LocalSong::onPlayCountSort(const bool &down) {
    if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：播放次数降序");
    else ui->local_sort_toolButton->setToolTip("当前排序方式：播放次数升序");
    //排序规则
    auto playCountSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.playCount > item2->m_information.playCount;
        return item1->m_information.playCount < item2->m_information.playCount;
    };
    MySort(playCountSortItem);
    this->m_isSorting = true;
}

void LocalSong::onRandomSort() {
    //记录当前的（旧的）vector
    this->m_lastLocationMusicVector = this->m_locationMusicVector;
    if (this->m_lastLocationMusicVector.isEmpty())return;
    //初始UI
    ui->local_sort_toolButton->setToolTip("当前排序方式：随机排序");
    ui->local_song_list_widget->setUpdatesEnabled(false);
    const auto layout = ui->local_song_list_widget->layout();
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item;
    }
    layout->setSpacing(2);
    layout->addItem(new QSpacerItem(1, 40, QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout->setContentsMargins(0, 0, 0, 0);
    //排序规则(打乱)
    // 使用当前时间作为随机数种子
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_MusicItemVector.begin(), this->m_MusicItemVector.end(), std::default_random_engine(seed));
    int index = -1;
    //重新添加
    this->m_locationMusicVector.clear();
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
    for (const auto &val: this->m_MusicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 2, val);
        this->m_locationMusicVector.emplace_back(val->m_information);
    }
    emit syncSongInfo(this->m_locationMusicVector); //按相同的顺序
    // 恢复更新
    ui->local_song_list_widget->setUpdatesEnabled(true);
    update();
    updateCurPlayIndex();
}

void LocalSong::onItemNextPlay() {
}

void LocalSong::onItemAddToPlayQueue() {
}

void LocalSong::onItemAddToNewSongList() {
}

void LocalSong::onItemAddToLove() {
}

void LocalSong::onItemAddToCollect() {
}

void LocalSong::onItemAddToPlayList() {
}

void LocalSong::onItemDownload() {
}

void LocalSong::onItemShare() {
}

void LocalSong::onItemComment() {
}

void LocalSong::onItemSameSong() {
}

void LocalSong::onItemViewSongInfo() {
}

void LocalSong::onItemDeleteSong(const int &idx) {
    qDebug()<<"收到删除信号，删除第 "<<idx<<" 项";
    PRINT_INFO("收到删除信号，删除第 %d 项",idx);
    this->m_lastLocationMusicVector = this->m_locationMusicVector;
    const auto widget = this->m_MusicItemVector[idx];
    widget->deleteLater();
    emit subSongInfo(m_locationMusicVector[idx]); //向KuGou发送删除idx信号
    this->m_locationMusicVector.erase(m_locationMusicVector.cbegin() + idx);
    this->m_MusicItemVector.erase(m_MusicItemVector.cbegin() + idx);
    //ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
    emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size()));

    //更新下标
    int index = -1;
    for (auto &val: this->m_locationMusicVector) {
        val.index = ++index;
    }
    index = -1;
    for (const auto &val: this->m_MusicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
    }
    update();
    updateCurPlayIndex();
}

void LocalSong::onItemOpenInFile() {
}

void LocalSong::onItemSearch() {
}

void LocalSong::onItemUpLoad() {
}

void LocalSong::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->m_parent->height() - 340);
}

bool LocalSong::eventFilter(QObject *watched, QEvent *event) {
    const auto button = qobject_cast<QToolButton*>(watched);
    if (button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }
    return QObject::eventFilter(watched, event);
}
