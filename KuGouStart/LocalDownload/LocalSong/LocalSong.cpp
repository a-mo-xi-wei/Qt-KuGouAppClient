//
// Created by WeiWang on 25-1-27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LocalSong.h" resolved

#include "LocalSong.h"
#include "ui_LocalSong.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "Async.h"
#include "ElaToolTip.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QScreen>
#include <QStandardPaths>
#include <QTimer>
#include <QBuffer>


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
    initUi();
    // 延迟调用 fetchAndSyncServerSongList，确保对象构造完成
    QTimer::singleShot(0, this, &LocalSong::fetchAndSyncServerSongList);
}

LocalSong::~LocalSong() {
    delete ui;
}

void LocalSong::playNextSong() {
    this->m_curPlayIndex = (m_curPlayIndex + 1) % static_cast<int>(this->m_locationMusicVector.size());
    auto item = this->m_musicItemVector[m_curPlayIndex];
    emit playMusic(item->m_information.mediaPath);
    setPlayItemHighlight(item);
}

void LocalSong::playPrevSong() {
    auto s = static_cast<int>(this->m_locationMusicVector.size());
    this->m_curPlayIndex = (m_curPlayIndex + s - 1) % s;
    auto item = this->m_musicItemVector[m_curPlayIndex];
    emit playMusic(item->m_information.mediaPath);
    setPlayItemHighlight(item);
}

void LocalSong::initUi() {

    //设置toolTip
    {
        //upload_toolButton
        auto upload_toolButton_toolTip = new ElaToolTip(ui->upload_toolButton);
        upload_toolButton_toolTip->setToolTip(QStringLiteral("上传未备份音乐到音乐云盘"));

        // 设置 local_search_lineEdit 的 tooltip
        //auto local_search_lineEdit_toolTip = new ElaToolTip(ui->local_search_lineEdit);
        //local_search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));

        // 设置 local_share_toolButton 的 tooltip
        auto local_share_toolButton_toolTip = new ElaToolTip(ui->local_share_toolButton);
        local_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        // 设置 local_album_toolButton 的 tooltip
        auto local_album_toolButton_toolTip = new ElaToolTip(ui->local_album_toolButton);
        local_album_toolButton_toolTip->setToolTip(QStringLiteral("专辑"));

        // 设置 local_sort_toolButton 的 tooltip
        auto local_sort_toolButton_toolTip = new ElaToolTip(ui->local_sort_toolButton);
        local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
        {
            // 排序相关
            connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                Q_UNUSED(down);
                onDefaultSort();  // 调用原有的排序槽函数
                local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                onAddTimeSort(down);  // 调用原有的排序槽函数
                if (down)
                    local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：添加时间降序"));
                else
                    local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：添加时间升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                onSongNameSort(down);  // 调用原本的排序槽
                local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                onSingerSort(down);
                local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                onDurationSort(down);
                local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, local_sort_toolButton_toolTip](const bool& down) {
                onPlayCountSort(down);
                local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, local_sort_toolButton_toolTip] {
                onRandomSort();
                local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机"));
            });

        }

        // 设置 local_batch_toolButton 的 tooltip
        auto local_batch_toolButton_toolTip = new ElaToolTip(ui->local_batch_toolButton);
        local_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }

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
    ui->local_search_lineEdit->setMaxWidth(150);
    auto font = QFont("AaSongLiuKaiTi");
    font.setWeight(QFont::Bold);
    ui->local_search_lineEdit->setFont(font);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->local_search_lineEdit->findChildren<QToolButton*>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton);
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));
            break;
        }
    }

    // 安装事件过滤器
    if (searchButton) {
        searchButton->installEventFilter(this);
    }

    /*//先直接往里面嵌入两首歌
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
            disconnect(*connection);
            delete connection;
        },
        Qt::QueuedConnection // 确保异步执行
    );*/

    //向服务器发送请求歌曲列表  //一定不能放在此处 !!! 原因： 此处还是处于构造函数的范围！！！尚未构造完成！！！
    //fetchAndSyncServerSongList();

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
            if (it == this->m_locationMusicVector.end()) {
                this->m_locationMusicVector.emplace_back(tempInformation);
                //加载相关信息
                auto item = new MusicItemWidget(tempInformation, this);
                //初始化item
                initMusicItem(item);
                //插入Item
                this->m_musicItemVector.emplace_back(item);
                const auto layout = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
                if (!layout)return;
                layout->insertWidget(layout->count() - 2, item);
                ui->widget->hide();
                qDebug()<<"成功添加歌曲 ："<<item->m_information.mediaPath;
                STREAM_INFO()<<"成功添加歌曲 ："<<item->m_information.mediaPath.toStdString();
                ElaMessageBar::success(ElaMessageBarType::BottomRight,"Success",
                    QString("成功添加音乐 : %1").arg(item->m_information.songName),1000,this->window());

                //ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
                emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size()));

                //加载下一首歌
                loadNextSong();
                // 将QPixmap转换为QImage并保存为PNG格式的字节流
                QByteArray imageData;
                QBuffer buffer(&imageData);
                buffer.open(QIODevice::WriteOnly);
                tempInformation.cover.toImage().save(&buffer, "PNG"); // 也可以使用"JPG"
                buffer.close();

                // 转换为Base64字符串
                QString base64Image = QString::fromLatin1(imageData.toBase64().data());
                auto postJson = QJsonObject{
                    {"index", tempInformation.index},
                    {"cover", base64Image},
                    {"songName", tempInformation.songName},
                    {"singer", tempInformation.singer},
                    {"duration", tempInformation.duration},
                    {"mediaPath", tempInformation.mediaPath},
                    {"addTime", tempInformation.addTime.toString("yyyy-MM-dd hh:mm:ss")},
                    {"playCount", tempInformation.playCount}
                };
                QJsonDocument doc(postJson);
                QString jsonString = doc.toJson(QJsonDocument::Compact); // 紧凑格式无换行
                //向服务器发送post请求 添加歌曲信息
                m_libHttp.UrlRequestPost(QStringLiteral("http://127.0.0.1:8080/api/addSong"),jsonString);
            }
            else {
                STREAM_INFO()<<title.toStdString()<<" 已存在，请勿重复插入";
                qDebug()<<title<<" 已存在，请勿重复插入";
                //加载下一首歌
                loadNextSong();
                return;
            }
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
            this->m_sortOptMenu->btnClickAgain();//新增歌曲，同时要符合当前排序
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
    if (this->m_lastLocationMusicVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
            QString("暂无音乐"),1000,this->window());
        return;
    }
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
    std::sort(this->m_musicItemVector.begin(), this->m_musicItemVector.end(), std::move(comparator));
    // 重新添加
    this->m_locationMusicVector.clear();
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
    int index = -1;
    for (const auto &val: this->m_musicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 2, val);
        this->m_locationMusicVector.emplace_back(val->m_information);
    }
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
    connect(item, &MusicItemWidget::play, this, [item, this] {
        //qDebug()<<"播放歌曲下标："<<item->m_information.index<<"===================";
        emit playMusic(item->m_information.mediaPath);
        this->m_isOrderPlay = false;
        setPlayItemHighlight(item);
    });
    connect(item, &MusicItemWidget::deleteSong, this, &LocalSong::onItemDeleteSong);
}

void LocalSong::fetchAndSyncServerSongList() {
    const QString reply = m_libHttp.UrlRequestGet("http://127.0.0.1:8080/api/localSongList", "");
    const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8());

    if (!doc.isObject()) return;

    QJsonArray songs = doc.object()["data"].toArray();

    for (const auto& songVal : songs) {
        QJsonObject song = songVal.toObject();
        QString mediaPath = song["media_path"].toString();

        // 检查本地文件是否存在
        if (!QFile::exists(mediaPath)) {
            // 向服务器发送删除请求
            QJsonObject delReq;
            delReq["song"] = song["song"].toString();
            delReq["singer"] = song["singer"].toString();
            delReq["duration"] = song["duration"].toString();
            m_libHttp.UrlRequestPost("http://127.0.0.1:8080/api/delSong",QJsonDocument(delReq).toJson(QJsonDocument::Compact));
            continue;
        }

        // 创建歌曲信息对象
        SongInfor info;
        info.index = song["index"].toInt(); // 保存服务器索引
        info.mediaPath = mediaPath;

        // 解析Base64封面
        QByteArray imgData = QByteArray::fromBase64(song["cover"].toString().toLatin1());

        info.cover.loadFromData(imgData);
        info.songName = song["song"].toString();
        info.singer = song["singer"].toString();
        info.duration = song["duration"].toString();
        info.addTime = QDateTime::fromString(song["add_time"].toString(), "yyyy-MM-dd hh:mm:ss");
        info.playCount = song["play_count"].toInt();

        m_locationMusicVector.emplace_back(info);

        auto item = new MusicItemWidget(info, this);
        initMusicItem(item);
        m_musicItemVector.emplace_back(item);

        // 插入到布局
        const auto layout = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
        if (!layout)return;
        layout->insertWidget(layout->count() - 2, item);
        emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size()));

    }

    for (int i = 0; i < m_locationMusicVector.size(); ++i) {
        m_locationMusicVector[i].index = i;
        m_musicItemVector[i]->m_information.index = i;
        m_musicItemVector[i]->setIndexText(i + 1);
    }
}

void LocalSong::setPlayItemHighlight(MusicItemWidget *item) {
    if (m_locationMusicVector.isEmpty()) return;
    //设置当前播放的高亮下标
    this->m_curPlayIndex = item->m_information.index;
    item->m_information.playCount++;// 增加播放次数
    if (m_curPlayItemWidget == nullptr) {
        // 没有歌曲在播放，设置当前歌曲为播放状态
        m_curPlayItemWidget = item;
        item->setPlayState(true);
    } else {
        // 有歌曲在播放
        if (item != m_curPlayItemWidget) {
            m_curPlayItemWidget->setPlayState(false);
            // 如果选择的歌曲不是当前播放的歌曲，切换状态
            item->setPlayState(true);
            m_curPlayItemWidget = item;         //注意 m_curPlayItemWidget 在此处才被重新赋值，不要在之前多此一举
        } else {
            // 如果是同一首歌，确保播放状态为 true
            item->setPlayState(true);
        }
    }
}

void LocalSong::on_local_all_play_toolButton_clicked() {
    if (this->m_locationMusicVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
                                QStringLiteral("暂无可播放音乐"),
                                1000,this->window());
        return;
    }
    //TODO 此处需要取消循环播放

    qDebug()<<"播放歌曲："<<m_musicItemVector.front()->m_information.mediaPath<<"===================";
    this->m_isOrderPlay = true;
    this->m_curPlayIndex = 0;
    emit playMusic(m_musicItemVector.front()->m_information.mediaPath);
}

void LocalSong::on_local_add_toolButton_clicked() {
    const QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("添加音乐"), musicPath,
                                                      "Music (*.mp3 *.aac *.wav)");
    if (paths.isEmpty())return;
    //QString fileName = QUrl::fromLocalFile(path).fileName();
    //qDebug() << "插入："<<paths.size()<<"条数据";
    for (auto &path: paths) {
        this->m_songQueue.enqueue(path);
    }
    this->loadNextSong();
}

void LocalSong::on_upload_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            QString("%1 功能暂未实现 敬请期待").arg(ui->upload_toolButton->text()),
                            1000,this->window());
}

void LocalSong::on_local_share_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "分享 功能暂未实现 敬请期待", 1000,this->window());
}

void LocalSong::on_local_album_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "专辑 功能暂未实现 敬请期待", 1000,this->window());
}

void LocalSong::on_local_batch_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "批量操作 功能暂未实现 敬请期待", 1000,this->window());
}

void LocalSong::on_search_pushButton_clicked() {
    emit find_more_music();
}

void LocalSong::on_local_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->popup(this->m_menuPosition);
}

void LocalSong::onAudioFinished() {
    if (this->m_isOrderPlay) {
        qDebug()<<"当前为顺序播放，"<<this->m_curPlayItemWidget->m_information.mediaPath<<" 播放结束,开始播放下一首歌曲";
        playNextSong();
    }
}

void LocalSong::onMaxScreenHandle() {
    if (this->m_locationMusicVector.isEmpty())return;
    if (this->m_curPlayIndex != -1) {
        //qDebug()<<"正在播放第 : "<<this->m_curPlayIndex<<" 首歌";
        const auto widget = m_musicItemVector[this->m_curPlayIndex];
        widget->setPlayState(true);
        //qDebug()<<"收到最大化信号";
    }
}

void LocalSong::onDefaultSort() {
    //ui->local_sort_toolButton->setToolTip("当前排序方式：默认排序");
    //排序规则
    auto defaultSortItem = [](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return item1->m_information.addTime < item2->m_information.addTime;
    };
    MySort(defaultSortItem);
}

void LocalSong::onAddTimeSort(const bool &down) {
    //if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：添加时间降序");
    //else ui->local_sort_toolButton->setToolTip("当前排序方式：添加时间升序");

    //排序规则
    auto addTimeSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.addTime > item2->m_information.addTime;
        return item1->m_information.addTime < item2->m_information.addTime;
    };

    MySort(addTimeSortItem);
    this->m_isSorting = true;
}

void LocalSong::onSongNameSort(const bool &down) {
    //if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：歌曲名降序");
    //else ui->local_sort_toolButton->setToolTip("当前排序方式：歌曲名升序");
    //排序规则
    auto songNameSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.songName > item2->m_information.songName;
        return item1->m_information.songName < item2->m_information.songName;
    };
    MySort(songNameSortItem);
    this->m_isSorting = true;
}

void LocalSong::onSingerSort(const bool &down) {
    //if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：歌手降序");
    //else ui->local_sort_toolButton->setToolTip("当前排序方式：歌手升序");
    //排序规则
    auto singerSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.singer > item2->m_information.singer;
        return item1->m_information.singer < item2->m_information.singer;
    };
    MySort(singerSortItem);
    this->m_isSorting = true;
}

void LocalSong::onDurationSort(const bool &down) {
    //if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：时长降序");
    //else ui->local_sort_toolButton->setToolTip("当前排序方式：时长升序");
    //排序规则
    auto durationSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        if (down) return item1->m_information.duration > item2->m_information.duration;
        return item1->m_information.duration < item2->m_information.duration;
    };
    MySort(durationSortItem);
    this->m_isSorting = true;
}

void LocalSong::onPlayCountSort(const bool &down) {
    //if (down)ui->local_sort_toolButton->setToolTip("当前排序方式：播放次数降序");
    //else ui->local_sort_toolButton->setToolTip("当前排序方式：播放次数升序");
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
    if (this->m_lastLocationMusicVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
            QString("暂无音乐"),1000,this->window());
        return;
    }
    //初始UI
    //ui->local_sort_toolButton->setToolTip("当前排序方式：随机排序");
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
    std::shuffle(this->m_musicItemVector.begin(), this->m_musicItemVector.end(), std::default_random_engine(seed));
    int index = -1;
    //重新添加
    this->m_locationMusicVector.clear();
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout());
    for (const auto &val: this->m_musicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 2, val);
        this->m_locationMusicVector.emplace_back(val->m_information);
    }
    // 恢复更新
    ui->local_song_list_widget->setUpdatesEnabled(true);
    update();
    updateCurPlayIndex();
}

void LocalSong::onItemNextPlay() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Play next not implemented", 1000, window());
    // TODO: Add song to play queue as next item
}

void LocalSong::onItemAddToPlayQueue() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to play queue not implemented", 1000, window());
    // TODO: Append song to play queue
}

void LocalSong::onItemAddToNewSongList() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to new song list not implemented", 1000, window());
    // TODO: Create new playlist and add song
}

void LocalSong::onItemAddToLove() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to loved songs not implemented", 1000, window());
    // TODO: Mark song as favorite
}

void LocalSong::onItemAddToCollect() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to collection not implemented", 1000, window());
    // TODO: Add song to user collection
}

void LocalSong::onItemAddToPlayList() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to playlist not implemented", 1000, window());
    // TODO: Show playlist selection dialog
}

void LocalSong::onItemDownload() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Download not implemented", 1000, window());
    // TODO: Download song if applicable
}

void LocalSong::onItemShare() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Share not implemented", 1000, window());
    // TODO: Implement sharing
}

void LocalSong::onItemComment() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Comment not implemented", 1000, window());
    // TODO: Show comment dialog
}

void LocalSong::onItemSameSong() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Find similar songs not implemented", 1000, window());
    // TODO: Search for similar songs
}

void LocalSong::onItemViewSongInfo() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "View song info not implemented", 1000, window());
    // TODO: Show song details dialog
}

void LocalSong::onItemDeleteSong(const int &idx) {
    qDebug()<<"收到删除信号，删除第 "<<idx<<" 项";
    PRINT_INFO("收到删除信号，删除第 %d 项",idx);
    //三个属性组成唯一键值
    auto song = this->m_locationMusicVector[idx].songName;
    auto singer = this->m_locationMusicVector[idx].singer;
    auto duration = this->m_locationMusicVector[idx].duration;

    this->m_lastLocationMusicVector = this->m_locationMusicVector;
    auto widget = this->m_musicItemVector[idx];
    widget->deleteLater();
    this->m_locationMusicVector.erase(m_locationMusicVector.cbegin() + idx);
    this->m_musicItemVector.erase(m_musicItemVector.cbegin() + idx);
    if (this->m_musicItemVector.isEmpty())ui->widget->show();
    //ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
    emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size()));

    //更新下标
    int index = -1;
    for (auto &val: this->m_locationMusicVector) {
        val.index = ++index;
    }
    index = -1;
    for (const auto &val: this->m_musicItemVector) {
        val->m_information.index = ++index; //更换下标
        val->setIndexText(index + 1); //设置indexLab
    }
    updateCurPlayIndex();
    // 向服务器发送删除请求
    QJsonObject delReq;
    delReq["song"] = song;
    delReq["singer"] = singer;
    delReq["duration"] = duration;

    /*Async::runAsync(QThreadPool::globalInstance(),&CLibhttp::UrlRequestGet,
       m_libHttp, QString("http://127.0.0.1:8080/api/delSong"),QJsonDocument(delReq).toJson(QJsonDocument::Compact),1000);
*/
    Async::runAsync(QThreadPool::globalInstance(),
    [this,delReq] {
        return m_libHttp.UrlRequestGet(
            QString("http://127.0.0.1:8080/api/delSong"),
            QJsonDocument(delReq).toJson(QJsonDocument::Compact),
            1000
        );
    });
    //m_libHttp.UrlRequestPost("http://127.0.0.1:8080/api/delSong",QJsonDocument(delReq).toJson(QJsonDocument::Compact),1000);
    //qDebug()<<"处理删除请求完成";
    ElaMessageBar::success(ElaMessageBarType::BottomRight,"Success",
        QString("成功删除音乐 : %1").arg(song),1000,this->window());
}

void LocalSong::onItemOpenInFile() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Open in file explorer not implemented", 1000, window());
    // TODO: Open file location in explorer
}

void LocalSong::onItemSearch() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Search not implemented", 1000, window());
    // TODO: Implement search functionality
}

void LocalSong::onItemUpLoad() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Upload not implemented", 1000, window());
    // TODO: Implement upload
}

void LocalSong::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->window()->height() - 340);
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
