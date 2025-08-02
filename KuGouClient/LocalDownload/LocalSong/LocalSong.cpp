/**
 * @file LocalSong.cpp
 * @brief 实现 LocalSong 类，提供本地歌曲管理界面功能
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#include "LocalSong.h"
#include "ui_LocalSong.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "Async.h"
#include "ElaToolTip.h"
#include "MySearchLineEdit.h"
#include "RefreshMask.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTimer>
#include <QBuffer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 匹配非乱码字符的正则表达式 */
static QRegularExpression re(QStringLiteral("^[A-Za-z0-9\\p{Han}\\\\/\\-_\\*]+$"));

static bool firstShow = true;
/**
 * @brief 构造函数，初始化本地歌曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
LocalSong::LocalSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocalSong)
    , m_player(std::make_unique<QMediaPlayer>(this))
    , m_searchAction(new QAction(this))
    , m_refreshMask(std::make_unique<RefreshMask>(this))               ///< 初始化刷新遮罩
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/localsong.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    getMetaData();                                       ///< 获取元数据
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this); ///< 创建排序菜单
    m_sortOptMenu = menu->getMenu<SortOptionMenu>();     ///< 获取排序选项菜单
    connect(m_sortOptMenu, &SortOptionMenu::selected, this, [this] {
        ui->local_sort_toolButton->setStyleSheet("QToolButton{border-image:url(':/Res/titlebar/sort-blue.svg');}"); ///< 设置选中样式
    });
    connect(m_sortOptMenu, &SortOptionMenu::deselected, this, [this] {
        ui->local_sort_toolButton->setStyleSheet(R"(
                QToolButton{border-image:url(':/Res/titlebar/sort-gray.svg');}
                QToolButton:hover{border-image:url(':/Res/titlebar/sort-blue.svg');})"); ///< 设置未选中样式
    });
    initUi();                                            ///< 初始化界面
    //QTimer::singleShot(0, this, &LocalSong::fetchAndSyncServerSongList); ///< 延迟同步服务器歌曲
    firstShow = true;
}

/**
 * @brief 析构函数，清理资源
 */
LocalSong::~LocalSong()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 播放下一首歌曲
 */
void LocalSong::playNextSong()
{
    qDebug() << "播放下一首歌曲";
    if (this->m_musicItemVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000, this->window()); ///< 显示无音乐提示
        return;
    }
    if (this->m_deleteSelf)
    {
        const auto item = this->m_musicItemVector[m_curPlayIndex]; ///< 获取当前歌曲
        emit playMusic(item->m_information.mediaPath);   ///< 播放当前歌曲
        setPlayItemHighlight(item);                      ///< 设置高亮
        this->m_deleteSelf = false;                      ///< 重置删除标志
        return;
    }
    this->m_curPlayIndex = (m_curPlayIndex + 1) % static_cast<int>(this->m_locationMusicVector.size()); ///< 更新索引
    const auto item = this->m_musicItemVector[m_curPlayIndex]; ///< 获取下一首歌曲
    emit playMusic(item->m_information.mediaPath);       ///< 播放下一首
    setPlayItemHighlight(item);                          ///< 设置高亮
}

/**
 * @brief 播放上一首歌曲
 */
void LocalSong::playPrevSong()
{
    qDebug() << "播放上一首歌曲";
    if (this->m_musicItemVector.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000, this->window()); ///< 显示无音乐提示
        return;
    }
    if (this->m_deleteSelf)
    {
        const auto item = this->m_musicItemVector[m_curPlayIndex]; ///< 获取当前歌曲
        emit playMusic(item->m_information.mediaPath);   ///< 播放当前歌曲
        setPlayItemHighlight(item);                      ///< 设置高亮
        this->m_deleteSelf = false;                      ///< 重置删除标志
        return;
    }
    const auto s = static_cast<int>(this->m_locationMusicVector.size()); ///< 获取歌曲总数
    this->m_curPlayIndex = (m_curPlayIndex + s - 1) % s; ///< 更新索引
    const auto item = this->m_musicItemVector[m_curPlayIndex]; ///< 获取上一首歌曲
    emit playMusic(item->m_information.mediaPath);       ///< 播放上一首
    setPlayItemHighlight(item);                          ///< 设置高亮
}

/**
 * @brief 初始化界面
 * @note 设置工具提示、图标、搜索动作和布局
 */
void LocalSong::initUi()
{
    auto upload_toolButton_toolTip = new ElaToolTip(ui->upload_toolButton); ///< 创建上传按钮工具提示
    upload_toolButton_toolTip->setToolTip(QStringLiteral("上传未备份音乐到音乐云盘")); ///< 设置上传提示
    auto local_share_toolButton_toolTip = new ElaToolTip(ui->local_share_toolButton); ///< 创建分享按钮工具提示
    local_share_toolButton_toolTip->setToolTip(QStringLiteral("分享")); ///< 设置分享提示
    auto local_album_toolButton_toolTip = new ElaToolTip(ui->local_album_toolButton); ///< 创建专辑按钮工具提示
    local_album_toolButton_toolTip->setToolTip(QStringLiteral("专辑")); ///< 设置专辑提示
    auto local_sort_toolButton_toolTip = new ElaToolTip(ui->local_sort_toolButton); ///< 创建排序按钮工具提示
    local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 设置默认排序提示
    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        Q_UNUSED(down);
        onDefaultSort();                                 ///< 调用默认排序
        local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onAddTimeSort(down);                             ///< 调用添加时间排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onSongNameSort(down);                            ///< 调用歌曲名称排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onSingerSort(down);                              ///< 调用歌手排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onDurationSort(down);                            ///< 调用时长排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onPlayCountSort(down);                           ///< 调用播放次数排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, local_sort_toolButton_toolTip] {
        onRandomSort();                                  ///< 调用随机排序
        local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机")); ///< 更新提示
        local_sort_toolButton_toolTip->adjustSize();     ///< 调整提示大小
    });
    auto local_batch_toolButton_toolTip = new ElaToolTip(ui->local_batch_toolButton); ///< 创建批量操作按钮工具提示
    local_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作")); ///< 设置批量操作提示

    const auto layout = ui->local_song_list_widget->layout(); ///< 获取歌曲列表布局
    layout->setSpacing(2);                               ///< 设置间距
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding)); ///< 添加扩展填充
    layout->setContentsMargins(0, 0, 0, 0);             ///< 设置边距

    ui->local_all_play_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    ui->local_add_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/add-gray.svg"))); ///< 设置添加按钮图标
    ui->upload_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/upload-cloud-gray.svg"))); ///< 设置上传按钮图标

    auto searchLineEdit = new MySearchLineEdit();
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false);   ///< 仅显示图标
    searchLineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作
    searchLineEdit->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    searchLineEdit->setFixedWidth(30);
    searchLineEdit->setMaxWidth(200);               ///< 设置搜索框最大宽度
    searchLineEdit->setBorderRadius(10);
    auto font = QFont("AaSongLiuKaiTi"); ///< 设置字体
    font.setWeight(QFont::Bold);
    font.setPointSize(12);                    ///< 设置粗体
    searchLineEdit->setFont(font);            ///< 应用字体
    ui->local_search_suggest_box->setMinimumWidth(0);
    ui->local_search_suggest_box->setLineEdit(searchLineEdit);
    ui->local_search_suggest_box->removeDefaultTrailAction();
    searchLineEdit->setPlaceholderText("");
    QToolButton *searchButton = nullptr;                 ///< 搜索按钮
    foreach (QToolButton *btn, searchLineEdit->findChildren<QToolButton *>())
    {
        if (btn->defaultAction() == this->m_searchAction)
        {
            searchButton = btn;                          ///< 找到搜索按钮
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建搜索按钮工具提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索")); ///< 设置搜索提示
            break;
        }
    }
    if (searchButton)
    {
        searchButton->installEventFilter(this);          ///< 安装事件过滤器
    }

    connect(ui->local_search_suggest_box,&ElaSuggestBox::suggestionClicked,this,&LocalSong::handleSuggestBoxSuggestionClicked);
}

/**
 * @brief 获取媒体元数据
 * @note 提取标题、歌手、封面、时长并上传至服务器
 */
void LocalSong::getMetaData()
{
    // 保存当前媒体路径，因为可能在异步操作中发生变化
    QString currentMediaPath = this->m_mediaPath;
    connect(m_player.get(), &QMediaPlayer::mediaStatusChanged, [=](const QMediaPlayer::MediaStatus &status) {
        // 确保处理的是当前请求的媒体
        if (this->m_mediaPath != currentMediaPath) {
            return;
        }
        if (status == QMediaPlayer::LoadedMedia)
        {
            this->m_player->stop();                      ///< 停止播放

            const QMediaMetaData data = this->m_player->metaData();                                 ///< 获取元数据

            auto title = data.value(QMediaMetaData::Title).toString();                  ///< 获取标题
            if (!re.match(title).hasMatch())
            {
                title = QUrl::fromLocalFile(this->m_mediaPath).fileName(); ///< 使用文件名
                title = title.first(title.lastIndexOf('.')); ///< 去除扩展名
            }

            auto singer = data.value(QMediaMetaData::ContributingArtist).toString();    ///< 获取歌手
            if (!re.match(singer).hasMatch()) {
                singer = QStringLiteral("网络歌手");     ///< 设置默认歌手
            }

            auto album = data.value(QMediaMetaData::AlbumTitle).toString();             ///< 获取专辑
            if (!re.match(album).hasMatch()) {
                album = QStringLiteral("网络专辑");
            }

            auto cover = data.value(QMediaMetaData::ThumbnailImage).value<QPixmap>();           ///< 获取封面
            if (cover.isNull())
            {
                cover = QPixmap(QString(":/Res/tablisticon/pix%1.png").arg(QRandomGenerator::global()->bounded(1, 11))); ///< 设置随机封面
            }

            const auto duration = data.value(QMediaMetaData::Duration).value<qint64>();///< 获取时长

            /// 获取文件信息
            QFileInfo fileInfo(this->m_mediaPath);
            int fileSize = fileInfo.exists() ? static_cast<int>(fileInfo.size()) : 0;               ///< 获取文件大小

            QString format;
            auto formatValue = data.value(QMediaMetaData::FileFormat);                 ///< 获取文件格式（后缀名）如 "MP3", "FLAC"
            if (formatValue.isValid() && formatValue.canConvert<QString>()) {
                format = formatValue.toString().toUpper(); // 尽量转成一致格式
            }

            // 如果元数据获取不到，再使用文件后缀
            if (format.isEmpty()) {
                format = fileInfo.suffix().toUpper();
            }

            QDateTime issueDate;                                                                    ///< 获取发行日期（元数据中的 Date 字段）
            auto dateValue = data.value(QMediaMetaData::Date);
            if (dateValue.canConvert<QDate>()) {
                issueDate.setDate(dateValue.toDate());
            } else if (dateValue.canConvert<QDateTime>()) {
                issueDate = dateValue.toDateTime();
            }

            SongInfor tempInformation;                   ///< 创建歌曲信息
            tempInformation.index = static_cast<int>(this->m_locationMusicVector.size()); ///< 设置索引
            tempInformation.cover = cover;               ///< 设置封面
            tempInformation.songName = title;            ///< 设置标题
            tempInformation.singer = singer;             ///< 设置歌手
            tempInformation.album = album;               ///< 设置专辑
            tempInformation.duration = QTime::fromMSecsSinceStartOfDay(static_cast<int>(duration)).toString("mm:ss"); ///< 设置时长
            tempInformation.mediaPath = this->m_mediaPath; ///< 设置路径
            tempInformation.addTime = QDateTime::currentDateTime(); ///< 设置添加时间
            tempInformation.playCount = 0;               ///< 设置播放次数
            tempInformation.fileSize = fileSize;         ///< 设置文件大小
            tempInformation.format = format;             ///< 设置文件格式
            tempInformation.issueDate = issueDate;       ///< 设置发行日期

            const auto it = std::find(this->m_locationMusicVector.begin(), this->m_locationMusicVector.end(), tempInformation); ///< 检查重复
            if (it == this->m_locationMusicVector.end())
            {
                this->m_locationMusicVector.emplace_back(tempInformation); ///< 添加歌曲信息
                auto item = new MusicItemWidget(tempInformation, this); ///< 创建音乐项
                initMusicItem(item);                     ///< 初始化音乐项
                this->m_musicItemVector.emplace_back(item); ///< 添加音乐项
                const auto layout = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout()); ///< 获取布局
                if (!layout) return;
                layout->insertWidget(layout->count() - 1, item); ///< 插入音乐项

                ///< 添加suggestion
                QJsonObject obj;
                obj["song"] = tempInformation.songName;
                obj["singer"] = tempInformation.singer;
                obj["duration"] = tempInformation.duration;
                ///< 添加媒体路径作为附加数据
                QVariantMap suggestData;
                suggestData["mediaPath"] = tempInformation.mediaPath;
                m_songSingerToKey[obj] = ui->local_search_suggest_box->addSuggestion(
                    tempInformation.songName + " - " + tempInformation.singer,
                    suggestData);

                ui->widget->hide();                      ///< 隐藏初始界面

                qDebug() << "成功添加歌曲 ：" << item->m_information.mediaPath;
                STREAM_INFO() << "成功添加歌曲 ：" << item->m_information.mediaPath.toStdString(); ///< 记录日志
                ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", QString("成功添加音乐 : %1").arg(item->m_information.songName), 500, this->window()); ///< 显示成功提示

                emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size())); ///< 更新数量标签

                QByteArray imageData;                    ///< 转换封面为字节流
                QBuffer buffer(&imageData);
                buffer.open(QIODevice::WriteOnly);
                tempInformation.cover.toImage().save(&buffer, "PNG"); ///< 保存为 PNG
                buffer.close();
                QString base64Image = QString::fromLatin1(imageData.toBase64().data()); ///< 转换为 Base64
                auto postJson = QJsonObject {             ///< 创建 JSON 数据
                    {"index", tempInformation.index},
                    {"cover", base64Image},
                    {"songName", tempInformation.songName},
                    {"singer", tempInformation.singer},
                    {"album", tempInformation.album},
                    {"duration", tempInformation.duration},
                    {"mediaPath", tempInformation.mediaPath},
                    {"addTime", tempInformation.addTime.toString("yyyy-MM-dd hh:mm:ss")},
                    {"playCount", tempInformation.playCount},
                    {"fileSize", tempInformation.fileSize},
                    {"format", tempInformation.format},
                    {"issueDate", tempInformation.issueDate.toString("yyyy-MM-dd hh:mm:ss")}
                };
                QJsonDocument doc(postJson);
                QString jsonString = doc.toJson(QJsonDocument::Compact); ///< 转换为 JSON 字符串
                m_libHttp.UrlRequestPost(QStringLiteral("http://127.0.0.1:8080/api/addSong"), jsonString); ///< 发送添加请求
            }
            else
            {
                STREAM_INFO() << title.toStdString() << " 已存在，请勿重复插入"; ///< 记录日志
                qDebug() << title << " 已存在，请勿重复插入";
                return;
            }
        }
        else if (status == QMediaPlayer::InvalidMedia) {
            qWarning() << "无效媒体文件:" << currentMediaPath;
        }

        // 无论成功失败，都断开这个连接，避免多次触发
        disconnect(m_player.get(), &QMediaPlayer::mediaStatusChanged, this, nullptr);
    });
}

/**
 * @brief 加载下一首歌曲
 * @note 从队列中取出并播放
 */
void LocalSong::loadNextSong()
{
    // 检查是否完成加载
    if (m_currentLoadIndex >= m_songQueue.size()) {
        finishLoading();
        return;
    }

    // 取出当前歌曲路径
    this->m_mediaPath = m_songQueue.at(m_currentLoadIndex);
    m_currentLoadIndex++;

    // 创建播放器
    this->m_player = std::make_unique<QMediaPlayer>(this);
    getMetaData();
    this->m_player->setSource(QUrl::fromLocalFile(this->m_mediaPath));
    this->m_player->play();
}

/**
 * @brief 串行加载歌曲
 */
void LocalSong::startSerialLoading()
{
    if (m_songQueue.isEmpty()) return;

    // 如果已经在加载中，停止当前加载
    if (m_loadTimer && m_loadTimer->isActive()) {
        m_loadTimer->stop();
    }

    // 重置状态
    m_currentLoadIndex = 0;
    m_isLoading = true;

    // 创建定时器
    if (!m_loadTimer) {
        m_loadTimer = new QTimer(this);
        connect(m_loadTimer, &QTimer::timeout, this, &LocalSong::loadNextSong);
    }

    // 设置加载间隔（200ms）
    m_loadTimer->start(200);

    // 立即加载第一首
    loadNextSong();
}

void LocalSong::finishLoading()
{
    if (m_loadTimer && m_loadTimer->isActive()) {
        m_loadTimer->stop();
    }

    m_songQueue.clear();
    m_isLoading = false;

    if (this->m_isSorting) {
        this->m_sortOptMenu->btnClickAgain();
    }

    // 显示完成提示
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "完成",
                          QString("成功添加 %1 首歌曲").arg(m_currentLoadIndex),
                          1500, this->window());
}

/**
 * @brief 自定义排序
 * @param comparator 比较器函数
 * @note 根据比较器重新排列歌曲
 */
void LocalSong::MySort(std::function<bool(const MusicItemWidget *, const MusicItemWidget *)> comparator)
{
    this->m_lastLocationMusicVector = this->m_locationMusicVector; ///< 备份歌曲列表
    if (this->m_lastLocationMusicVector.isEmpty())
    {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
        return;
    }
    ui->local_song_list_widget->setUpdatesEnabled(false); ///< 禁用更新
    const auto layout = ui->local_song_list_widget->layout(); ///< 获取布局
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item;                                     ///< 删除布局项
    }
    layout->setSpacing(2);                               ///< 设置间距
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding)); ///< 添加扩展填充
    layout->setContentsMargins(0, 0, 0, 0);             ///< 设置边距
    std::sort(this->m_musicItemVector.begin(), this->m_musicItemVector.end(), std::move(comparator)); ///< 排序
    this->m_locationMusicVector.clear();                 ///< 清空歌曲列表
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout()); ///< 获取布局
    int index = -1;
    for (const auto &val : this->m_musicItemVector)
    {
        val->m_information.index = ++index;              ///< 更新索引
        val->setIndexText(index + 1);                    ///< 设置索引文本
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 1, val); ///< 插入音乐项
        this->m_locationMusicVector.emplace_back(val->m_information); ///< 添加歌曲信息
    }
    ui->local_song_list_widget->setUpdatesEnabled(true); ///< 启用更新
    update();                                            ///< 更新界面
    updateCurPlayIndex();                                ///< 更新播放索引
}

/**
 * @brief 更新当前播放索引
 * @note 在排序或删除后更新播放位置
 */
void LocalSong::updateCurPlayIndex()
{
    if (m_curPlayIndex <= -1)
        return;
    // 添加空向量检查
    if (m_lastLocationMusicVector.empty()) {
        m_curPlayIndex = -1;
        m_curPlayItemWidget = nullptr;
        return;
    }
    const auto temp = this->m_lastLocationMusicVector[this->m_curPlayIndex]; ///< 获取当前歌曲
    auto it = std::find(this->m_locationMusicVector.begin(), this->m_locationMusicVector.end(), temp); ///< 查找歌曲
    m_deleteSelf = false;                                ///< 重置删除标志
    if (it == this->m_locationMusicVector.end())
    {
        // qDebug() << "删除的是自己";

        // 重点修改：空向量时重置索引
        if (m_locationMusicVector.empty()) {
            m_curPlayIndex = -1;
            m_curPlayItemWidget = nullptr;
            return;
        }

        if (this->m_curPlayIndex >= this->m_locationMusicVector.size())
        {
            this->m_curPlayIndex = 0;                    ///< 重置索引
        }
        else
        {
            qDebug() << "下标保持不变：" << this->m_curPlayIndex;
            m_deleteSelf = true;                         ///< 设置删除标志
            this->m_curPlayItemWidget->deleteLater();    ///< 删除当前控件
            this->m_curPlayItemWidget = nullptr;         ///< 清空当前控件
        }
    }
    else
    {
        this->m_curPlayIndex = static_cast<int>(it - this->m_locationMusicVector.begin()); ///< 更新索引
    }
}

/**
 * @brief 初始化音乐项
 * @param item 音乐项控件
 * @note 设置高亮颜色、圆角和信号连接
 */
void LocalSong::initMusicItem(MusicItemWidget *item)
{
    item->setFillColor(QColor(QStringLiteral("#B0EDF6"))); ///< 设置高亮颜色
    item->setRadius(12);                                 ///< 设置圆角
    item->setInterval(1);                                ///< 设置间隔
    connect(item, &MusicItemWidget::play, this, [item, this] {
        emit playMusic(item->m_information.mediaPath);   ///< 播放歌曲
        this->m_isOrderPlay = false;                     ///< 关闭顺序播放
        setPlayItemHighlight(item);                      ///< 设置高亮
    });
    connect(item, &MusicItemWidget::deleteSong, this, &LocalSong::onItemDeleteSong); ///< 连接删除信号
}

/**
 * @brief 从服务器同步歌曲列表
 * @note 获取服务器歌曲列表并更新本地
 */
void LocalSong::fetchAndSyncServerSongList()
{
    // 异步请求拉取服务器歌曲列表
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this] {
        //qDebug()<<"异步发送歌曲列表请求";
        return m_libHttp.UrlRequestGet("http://127.0.0.1:8080/api/localSongList", "");
    });
    Async::onResultReady(future, this, [this](const QString &reply) {
        // qDebug()<<"开始解析请求结果";
        const QJsonDocument doc = QJsonDocument::fromJson(reply.toUtf8()); ///< 解析 JSON
        if (!doc.isObject())
            return;
        QJsonArray songs = doc.object()["data"].toArray();   ///< 获取歌曲数组

        if (!songs.isEmpty())
            ui->widget->hide();

        handleSongsResult(songs);  // 分帧处理 UI 插入
    });
}

/**
 * @brief 设置播放高亮
 * @param item 音乐项控件
 * @note 高亮当前播放歌曲并更新播放次数
 */
void LocalSong::setPlayItemHighlight(MusicItemWidget *item)
{
    if (m_locationMusicVector.isEmpty())
        return;
    this->m_curPlayIndex = item->m_information.index;    ///< 设置当前索引
    item->m_information.playCount++;                     ///< 增加播放次数
    if (m_curPlayItemWidget == nullptr)
    {
        m_curPlayItemWidget = item;                      ///< 设置当前控件
        item->setPlayState(true);                        ///< 设置播放状态
    }
    else
    {
        if (item != m_curPlayItemWidget)
        {
            m_curPlayItemWidget->setPlayState(false);    ///< 取消旧高亮
            item->setPlayState(true);                    ///< 设置新高亮
            m_curPlayItemWidget = item;                  ///< 更新当前控件
        }
        else
        {
            item->setPlayState(true);                    ///< 确保播放状态
        }
    }
}

void LocalSong::scrollToItem(const QString &mediaPath) {
    // 查找匹配的歌曲项
    for (const auto & i : m_musicItemVector) {
        if (i->m_information.mediaPath == mediaPath) {
            MusicItemWidget* item = i;

            // 确保滚动区域可见
            //ui->scrollArea->ensureWidgetVisible(item);
            ui->scrollArea->smoothScrollTo(item->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
            // 设置临时高亮效果
            item->setHighlight(true);
            QTimer::singleShot(3000, item, [item]() {
                item->setHighlight(false);
            });

            break;
        }
    }
}

void LocalSong::handleSongsResult(const QJsonArray &songs)
{
    m_locationMusicVector.clear();
    m_musicItemVector.clear();
    m_songSingerToKey.clear();

    QVector<QJsonObject> pendingSongs;
    for (const auto &val : songs)
    {
        QJsonObject song = val.toObject();
        if (QFile::exists(song["media_path"].toString()))
            pendingSongs.append(song);  // 只保留存在的
    }

    auto it = std::make_shared<int>(0);
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);  // 每200ms处理一个,防止卡顿
    connect(timer, &QTimer::timeout, this, [=]() {
        if (*it >= pendingSongs.size()) {
            timer->stop();
            timer->deleteLater();

            // 处理完毕，更新索引
            for (int i = 0; i < m_locationMusicVector.size(); ++i) {
                m_locationMusicVector[i].index = i;
                m_musicItemVector[i]->m_information.index = i;
                m_musicItemVector[i]->setIndexText(i + 1);
            }

            m_refreshMask->hideLoading();
            return;
        }

        QJsonObject song = pendingSongs[*it];
        ++(*it);

        SongInfor info;
        info.index = song["index"].toInt();
        info.mediaPath = song["media_path"].toString();
        info.songName = song["song"].toString();
        info.singer = song["singer"].toString();
        info.duration = song["duration"].toString();
        info.addTime = QDateTime::fromString(song["add_time"].toString(), "yyyy-MM-dd hh:mm:ss");
        info.playCount = song["play_count"].toInt();
        info.fileSize = song["file_size"].toInt();
        info.format = song["format"].toString();
        info.issueDate = QDateTime::fromString(song["issueDate"].toString(), "yyyy-MM-dd hh:mm:ss");

        QByteArray imgData = QByteArray::fromBase64(song["cover"].toString().toLatin1());
        info.cover.loadFromData(imgData);

        m_locationMusicVector.push_back(info);
        auto item = new MusicItemWidget(info, this);
        initMusicItem(item);
        m_musicItemVector.push_back(item);

        if (auto layout = qobject_cast<QVBoxLayout *>(ui->local_song_list_widget->layout())) {
            layout->insertWidget(layout->count() - 1, item);
        }

        QJsonObject obj;
        obj["song"] = info.songName;
        obj["singer"] = info.singer;
        obj["duration"] = info.duration;

        QVariantMap suggestData;
        suggestData["mediaPath"] = info.mediaPath;
        m_songSingerToKey[obj] = ui->local_search_suggest_box->addSuggestion(
            info.songName + " - " + info.singer,
            suggestData);

        emit updateCountLabel(static_cast<int>(m_locationMusicVector.size()));
    });

    timer->start();  // 启动帧定时器
}

/**
 * @brief 全部播放按钮点击槽函数
 * @note 播放第一首歌曲并取消循环
 */
void LocalSong::on_local_all_play_toolButton_clicked()
{
    if (this->m_locationMusicVector.isEmpty())
    {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QStringLiteral("暂无可播放音乐"), 1000, this->window()); ///< 显示警告
        return;
    }
    emit cancelLoopPlay();                               ///< 取消循环播放
    qDebug() << "播放歌曲：" << m_musicItemVector.front()->m_information.mediaPath << "===================";
    this->m_isOrderPlay = true;                          ///< 启用顺序播放
    this->m_curPlayIndex = 0;                            ///< 设置索引
    setPlayItemHighlight(this->m_musicItemVector.front()); ///< 设置高亮
    emit playMusic(m_musicItemVector.front()->m_information.mediaPath); ///< 播放第一首
}

/**
 * @brief 添加歌曲按钮点击槽函数
 * @note 打开文件对话框添加歌曲
 */
void LocalSong::on_local_add_toolButton_clicked()
{
    const QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first(); ///< 获取音乐路径
    QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("添加音乐"), musicPath, "Music (*.mp3 *.aac *.wav)"); ///< 打开文件对话框
    if (paths.isEmpty())
        return;
    for (auto &path : paths)
    {
        this->m_songQueue.enqueue(path);                 ///< 添加到队列
    }
    // 启动串行加载
    startSerialLoading();                                ///< 加载下一首
}

/**
 * @brief 上传按钮点击槽函数
 * @note 显示未实现提示
 */
void LocalSong::on_upload_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", QString("%1 功能暂未实现 敬请期待").arg(ui->upload_toolButton->text()), 1000, this->window()); ///< 显示提示
}

void LocalSong::handleSuggestBoxSuggestionClicked(const QString &suggestText, const QVariantMap &suggestData) {
    qDebug()<<suggestText<<" 被点击";
    ///< 检查是否包含媒体路径
    if (suggestData.contains("mediaPath")) {
        QString mediaPath = suggestData["mediaPath"].toString();
        scrollToItem(mediaPath);
    } else {
        qWarning() << "未找到媒体路径数据：" << suggestText;
    }
}

/**
 * @brief 分享按钮点击槽函数
 * @note 显示未实现提示
 */
void LocalSong::on_local_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "分享 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 专辑按钮点击槽函数
 * @note 显示未实现提示
 */
void LocalSong::on_local_album_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "专辑 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示未实现提示
 */
void LocalSong::on_local_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void LocalSong::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 触发搜索信号
}

/**
 * @brief 排序按钮点击槽函数
 * @note 显示排序菜单
 */
void LocalSong::on_local_sort_toolButton_clicked()
{
    this->m_sortOptMenu->exec(QCursor::pos());           ///< 显示菜单
}

/**
 * @brief 音频播放结束槽函数
 * @note 上一首自然播放结束后，顺序播放时自动播放下一首
 */
void LocalSong::onAudioFinished()
{
    qDebug() << "上一首播放结束,当前m_isOrderPlay: " << this->m_isOrderPlay;
    ///< 此处存在疑点，因为：此处接收到的播放结束信号可能是自然结束也可能是外部中断结束，自然结束的话自然可以调用播放下一首但若是中断结束的话则不应该，所以应该明确该信号的发出前提
    if (this->m_isOrderPlay)
    {
        playNextSong();                                  ///< 播放下一首
    }
}

/**
 * @brief 最大化屏幕处理槽函数
 * @note 更新当前播放高亮
 */
void LocalSong::onMaxScreenHandle()
{
    if (this->m_locationMusicVector.isEmpty())
        return;
    if (this->m_curPlayIndex != -1)
    {
        const auto widget = m_musicItemVector[this->m_curPlayIndex]; ///< 获取当前控件
        widget->setPlayState(true);                      ///< 设置播放状态
    }
}

/**
 * @brief 默认排序
 * @note 按添加时间升序
 */
void LocalSong::onDefaultSort()
{
    auto defaultSortItem = [](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return item1->m_information.addTime < item2->m_information.addTime; ///< 比较添加时间
    };
    MySort(defaultSortItem);                             ///< 执行排序
}

/**
 * @brief 添加时间排序
 * @param down 是否降序
 */
void LocalSong::onAddTimeSort(const bool &down)
{
    auto addTimeSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return down ? item1->m_information.addTime > item2->m_information.addTime : item1->m_information.addTime < item2->m_information.addTime; ///< 比较添加时间
    };
    MySort(addTimeSortItem);                             ///< 执行排序
    this->m_isSorting = true;                            ///< 设置排序状态
}

/**
 * @brief 歌曲名称排序
 * @param down 是否降序
 */
void LocalSong::onSongNameSort(const bool &down)
{
    auto songNameSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return down ? item1->m_information.songName > item2->m_information.songName : item1->m_information.songName < item2->m_information.songName; ///< 比较歌曲名称
    };
    MySort(songNameSortItem);                            ///< 执行排序
    this->m_isSorting = true;                            ///< 设置排序状态
}

/**
 * @brief 歌手排序
 * @param down 是否降序
 */
void LocalSong::onSingerSort(const bool &down)
{
    auto singerSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return down ? item1->m_information.singer > item2->m_information.singer : item1->m_information.singer < item2->m_information.singer; ///< 比较歌手
    };
    MySort(singerSortItem);                              ///< 执行排序
    this->m_isSorting = true;                            ///< 设置排序状态
}

/**
 * @brief 时长排序
 * @param down 是否降序
 */
void LocalSong::onDurationSort(const bool &down)
{
    auto durationSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return down ? item1->m_information.duration > item2->m_information.duration : item1->m_information.duration < item2->m_information.duration; ///< 比较时长
    };
    MySort(durationSortItem);                            ///< 执行排序
    this->m_isSorting = true;                            ///< 设置排序状态
}

/**
 * @brief 播放次数排序
 * @param down 是否降序
 */
void LocalSong::onPlayCountSort(const bool &down)
{
    auto playCountSortItem = [down](const MusicItemWidget *item1, const MusicItemWidget *item2) {
        return down ? item1->m_information.playCount > item2->m_information.playCount : item1->m_information.playCount < item2->m_information.playCount; ///< 比较播放次数
    };
    MySort(playCountSortItem);                           ///< 执行排序
    this->m_isSorting = true;                            ///< 设置排序状态
}

/**
 * @brief 随机排序
 */
void LocalSong::onRandomSort()
{
    this->m_lastLocationMusicVector = this->m_locationMusicVector; ///< 备份歌曲列表
    if (this->m_lastLocationMusicVector.isEmpty())
    {
        ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
        return;
    }
    ui->local_song_list_widget->setUpdatesEnabled(false); ///< 禁用更新
    const auto layout = ui->local_song_list_widget->layout(); ///< 获取布局
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item;                                     ///< 删除布局项
    }
    layout->setSpacing(2);                               ///< 设置间距
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding)); ///< 添加扩展填充
    layout->setContentsMargins(0, 0, 0, 0);             ///< 设置边距
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取随机种子
    std::shuffle(this->m_musicItemVector.begin(), this->m_musicItemVector.end(), std::default_random_engine(seed)); ///< 随机打乱
    int index = -1;
    this->m_locationMusicVector.clear();                 ///< 清空歌曲列表
    const auto lay = dynamic_cast<QVBoxLayout *>(ui->local_song_list_widget->layout()); ///< 获取布局
    for (const auto &val : this->m_musicItemVector)
    {
        val->m_information.index = ++index;              ///< 更新索引
        val->setIndexText(index + 1);                    ///< 设置索引文本
        lay->insertWidget(ui->local_song_list_widget->layout()->count() - 1, val); ///< 插入音乐项
        this->m_locationMusicVector.emplace_back(val->m_information); ///< 添加歌曲信息
    }
    ui->local_song_list_widget->setUpdatesEnabled(true); ///< 启用更新
    update();                                            ///< 更新界面
    updateCurPlayIndex();                                ///< 更新播放索引
}

/**
 * @brief 下一首播放槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemNextPlay()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Play next not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 添加到播放队列槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemAddToPlayQueue()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to play queue not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 添加到新歌单槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemAddToNewSongList()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to new song list not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 添加到喜爱槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemAddToLove()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to loved songs not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 添加到收藏槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemAddToCollect()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to collection not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 添加到播放列表槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemAddToPlayList()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Add to playlist not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 下载槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemDownload()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Download not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 分享槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemShare()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Share not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 评论槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemComment()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Comment not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 相似歌曲槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemSameSong()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Find similar songs not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 查看歌曲信息槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemViewSongInfo()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "View song info not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 删除歌曲槽函数
 * @param idx 歌曲索引
 * @note 删除本地和服务器歌曲
 */
void LocalSong::onItemDeleteSong(const int &idx)
{
    qDebug() << "收到删除信号，删除第 " << idx << " 项";
    PRINT_INFO("收到删除信号，删除第 %d 项", idx);      ///< 记录日志
    auto song = this->m_locationMusicVector[idx].songName; ///< 获取歌曲名称
    auto singer = this->m_locationMusicVector[idx].singer; ///< 获取歌手
    auto duration = this->m_locationMusicVector[idx].duration; ///< 获取时长
    this->m_lastLocationMusicVector = this->m_locationMusicVector; ///< 备份歌曲列表
    auto widget = this->m_musicItemVector[idx];          ///< 获取控件
    widget->deleteLater();                               ///< 删除控件
    this->m_locationMusicVector.erase(m_locationMusicVector.cbegin() + idx); ///< 删除歌曲信息
    this->m_musicItemVector.erase(m_musicItemVector.cbegin() + idx); ///< 删除音乐项
    if (this->m_musicItemVector.isEmpty())
        ui->widget->show();                              ///< 显示初始界面
    emit updateCountLabel(static_cast<int>(this->m_locationMusicVector.size())); ///< 更新数量标签
    int index = -1;
    for (auto &val : this->m_locationMusicVector)
    {
        val.index = ++index;                             ///< 更新歌曲索引
    }
    index = -1;
    for (const auto &val : this->m_musicItemVector)
    {
        val->m_information.index = ++index;              ///< 更新控件索引
        val->setIndexText(index + 1);                    ///< 设置索引文本
    }
    updateCurPlayIndex();                                ///< 更新播放索引
    QJsonObject delReq;                                  ///< 创建删除请求
    delReq["song"] = song;
    delReq["singer"] = singer;
    delReq["duration"] = duration;

    ///< 删除suggestion
    auto it = m_songSingerToKey.find(delReq);
    if (it != m_songSingerToKey.end()) {
        ui->local_search_suggest_box->removeSuggestion(it->second);
        m_songSingerToKey.erase(it);
    }

    /*Async::runAsync(QThreadPool::globalInstance(),&CLibhttp::UrlRequestGet,
       m_libHttp, QString("http://127.0.0.1:8080/api/delSong"),QJsonDocument(delReq).toJson(QJsonDocument::Compact),1000);
    */
    const auto future = Async::runAsync(QThreadPool::globalInstance(),
    [this,delReq] {
        return m_libHttp.UrlRequestPost(
            QString("http://127.0.0.1:8080/api/delSong"),
            QJsonDocument(delReq).toJson(QJsonDocument::Compact),
            1000
        );
    }); ///< 异步删除
    //qDebug()<<"处理删除请求完成";
    // 成功后提示
    Async::onResultReady(future, this, [this, song](const QString &responseData) {
        // 可以根据返回内容判断是否删除成功（这里假设服务端返回 JSON 并有 success 字段）
        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(responseData.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning() << "删除请求失败：返回数据解析失败";
            STREAM_WARN() << "删除请求失败：返回数据解析失败";
            return;
        }
        const QJsonObject obj = doc.object();
        if (obj.value("code").toInt() == 0) {
            ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success",
                QString("成功删除音乐 : %1").arg(song), 1000, this->window());
        } else {
            ElaMessageBar::error(ElaMessageBarType::BottomRight, "Error",
                QString("删除失败 : %1").arg(obj.value("message").toString()), 2000, this->window());
        }
    });
}

/**
 * @brief 在文件资源管理器中打开槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemOpenInFile()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Open in file explorer not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 搜索槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemSearch()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Search not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 上传槽函数
 * @note 显示未实现提示
 */
void LocalSong::onItemUpLoad()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "Upload not implemented", 1000, window()); ///< 显示提示
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理搜索图标的动态切换
 */
bool LocalSong::eventFilter(QObject *watched, QEvent *event)
{
    const auto button = qobject_cast<QToolButton *>(watched); ///< 转换为工具按钮
    if (button && button->defaultAction() == this->m_searchAction)
    {
        if (event->type() == QEvent::Enter)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置蓝色图标
        }
        else if (event->type() == QEvent::Leave)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置黑色图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}

void LocalSong::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise();  // 确保遮罩在最上层
    if (firstShow) {
        firstShow = false;
        // qDebug()<<"第一次触发";
        // qDebug()<<"遮罩绘制";
        m_refreshMask->keepLoading();
        QTimer::singleShot(0, this, [this]() {
            fetchAndSyncServerSongList();  // 保证主线程中注册回调
        });
    }
}

void LocalSong::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (m_refreshMask)
        m_refreshMask->setGeometry(this->rect());
}
