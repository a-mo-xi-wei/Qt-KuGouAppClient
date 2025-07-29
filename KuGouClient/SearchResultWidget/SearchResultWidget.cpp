//
// Created by WeiWang on 25-7-29.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SearchResultWidget.h" resolved

#include "SearchResultWidget.h"
#include "ElaMessageBar.h"
#include "MyScrollArea.h"
#include "Async.h"
#include "MusicItemWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

SearchResultWidget::SearchResultWidget(QWidget *parent)
    : QWidget(parent)
    , m_refreshMask(std::make_unique<RefreshMask>(this))               ///< 初始化刷新遮罩
{
    setObjectName("SearchResultWidget"); ///< 设置对象名称，便于样式管理
    QFile file(GET_CURRENT_DIR + QStringLiteral("/result.css"));   ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());                       ///< 应用样式表
    } else {
        // @note 未使用，保留用于调试
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    connect(this->m_refreshMask.get(), &RefreshMask::loadingFinished, [this](const QString& message) {
        if (message == "响应失败") {
            ElaMessageBar::error(ElaMessageBarType::BottomRight, "Error",
                             QString("加载失败"),
                             1000, this->window());
        }
        else if (message == "加载完成") {
            ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success",
                             QString("成功加载%1首歌曲").arg(m_searchMusicItemVector.size()),
                             1000, this->window()); ///< 显示提示信息
        }
    });
}

void SearchResultWidget::handleSuggestion(const QString &suggestText) {
    auto topLab = this->findChild<QLabel *>("searchResultTopLabel");
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
            song.fileSize = songObj["fileSize"].toInt();
            song.format = songObj["format"].toString();
            song.issueDate = QDateTime::fromString(songObj["issueDate"].toString(), "yyyy-MM-dd hh:mm:ss");
            song.cover = song.coverUrl.isEmpty() ? QPixmap(":/Res/tablisticon/pix4.png") : song.cover;
            songs.append(song);
        }

        // 更新UI（与原始代码相同）
        auto scrollWidget = this->findChild<QWidget*>("SearchResultWidgetScrollWidget");
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
            item->setFillColor(QColor(QStringLiteral("#B0EDF6"))); ///< 设置高亮颜色
            item->setRadius(12); ///< 设置圆角
            item->setInterval(1); ///< 设置间隔

            layout->insertWidget(layout->count() - 1, item);
            m_searchMusicItemVector.append(item);

            // 异步加载封面
            if (!song.coverUrl.isEmpty()) {
                loadCoverAsync(item, song.coverUrl);
            }
            // 异步加载歌曲网络路径
            if (!song.hash.isEmpty()) {
                loadSongUrlAsync(item,song.hash);
            }
            currentIndex++;
        });

        addTimer->start(100);
    });
}

void SearchResultWidget::initUi() {
    // 创建顶部水平布局，显示搜索结果标题
    auto hlay1 = new QHBoxLayout; ///< 搜索结果顶部水平布局
    {
        auto topLab = new QLabel("搜索到相关歌曲");
        topLab->setObjectName("searchResultTopLabel"); ///< 设置标签对象名称
        hlay1->addSpacing(15); ///< 添加左侧间距
        hlay1->addWidget(topLab); ///< 添加标题标签
        hlay1->addStretch(); ///< 添加弹性空间，推右对齐
    }

    // 创建中间水平布局，包含功能按钮
    auto hlay2 = new QHBoxLayout; ///< 搜索结果中间水平布局
    {
        hlay2->setSpacing(15); ///< 设置按钮间距
        // 创建“播放全部”按钮
        auto playAllBtn = new QToolButton;
        playAllBtn->setObjectName("SearchResultWidget-playAllBtn"); ///< 设置对象名称
        playAllBtn->setCursor(Qt::PointingHandCursor); ///< 设置鼠标悬停为手型
        playAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置图标+文本样式
        playAllBtn->setFixedSize(100, 30); ///< 设置固定大小
        playAllBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg"))); ///< 设置播放图标
        playAllBtn->setText("播放全部");

        // 创建“高潮试听”按钮
        auto highListenBtn = new QToolButton;
        highListenBtn->setObjectName("SearchResultWidget-highListenBtn"); ///< 设置对象名称
        highListenBtn->setCursor(Qt::PointingHandCursor); ///< 设置鼠标悬停为手型
        highListenBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置图标+文本样式
        highListenBtn->setFixedSize(100, 30); ///< 设置固定大小
        highListenBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/highListen-white.svg"))); ///< 设置高音质图标
        highListenBtn->setText("高潮试听");

        // 创建“下载全部”按钮
        auto downloadAllBtn = new QToolButton;
        downloadAllBtn->setObjectName("SearchResultWidget-downloadAllBtn"); ///< 设置对象名称
        downloadAllBtn->setCursor(Qt::PointingHandCursor); ///< 设置鼠标悬停为手型
        downloadAllBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置图标+文本样式
        downloadAllBtn->setFixedSize(100, 30); ///< 设置固定大小
        downloadAllBtn->setIcon(QIcon(QStringLiteral(":/Res/window/download.svg"))); ///< 设置下载图标
        downloadAllBtn->setText("下载全部");

        // 创建“批量操作”按钮
        auto batchOperationBtn = new QToolButton;
        batchOperationBtn->setObjectName("SearchResultWidget-batchOperationBtn"); ///< 设置对象名称
        batchOperationBtn->setCursor(Qt::PointingHandCursor); ///< 设置鼠标悬停为手型
        batchOperationBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置图标+文本样式
        batchOperationBtn->setFixedSize(100, 30); ///< 设置固定大小
        batchOperationBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/batch-operation-black.svg"))); ///< 设置批量操作图标
        batchOperationBtn->setText("批量操作");

        // 将按钮添加到水平布局
        hlay2->addSpacing(15); ///< 添加左侧间距
        hlay2->addWidget(playAllBtn);
        hlay2->addWidget(highListenBtn);
        hlay2->addWidget(downloadAllBtn);
        hlay2->addWidget(batchOperationBtn);
        hlay2->addStretch(); ///< 添加弹性空间，推右对齐

        // 连接按钮点击信号，显示功能未实现的提示
        connect(playAllBtn, &QToolButton::clicked, [this, playAllBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(playAllBtn->text()),
                             1000, this->window()); ///< 显示提示信息
        });
        connect(highListenBtn, &QToolButton::clicked, [this, highListenBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(highListenBtn->text()),
                             1000, this->window()); ///< 显示提示信息
        });
        connect(downloadAllBtn, &QToolButton::clicked, [this, downloadAllBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(downloadAllBtn->text()),
                             1000, this->window()); ///< 显示提示信息
        });
        connect(batchOperationBtn, &QToolButton::clicked, [this, batchOperationBtn] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                             QString("%1 功能暂未实现 敬请期待").arg(batchOperationBtn->text()),
                             1000, this->window()); ///< 显示提示信息
        });
    }

    // 创建滚动区域
    auto scrollArea = new MyScrollArea;
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置滚动区域扩展策略
    scrollArea->setObjectName("SearchResultWidgetScrollArea"); ///< 设置对象名称
    scrollArea->setFrameShape(QFrame::NoFrame); ///< 设置无边框样式
    auto scrollWidget = new QWidget;
    scrollWidget->setObjectName("SearchResultWidgetScrollWidget"); ///< 设置滚动内容区域对象名称
    scrollWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    scrollWidget->setAttribute(Qt::WA_TranslucentBackground); ///< 设置滚动内容区域透明
    scrollWidget->setAutoFillBackground(false); ///< 禁用自动填充背景
    auto scrollWidgetVLay = new QVBoxLayout(scrollWidget); ///< 创建滚动内容垂直布局
    scrollWidgetVLay->addStretch(); ///< 添加弹性空间，确保内容顶部对齐
    scrollArea->setWidget(scrollWidget); ///< 将内容区域设置为滚动区域的子控件
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); ///< 始终显示垂直滚动条
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); ///< 隐藏水平滚动条

    // 创建主垂直布局，组合所有子布局和控件
    auto vlay = new QVBoxLayout(this); ///< 主垂直布局
    vlay->setContentsMargins(8, 3, 3, 5);
    vlay->setSpacing(10); ///< 设置子控件间距
    vlay->addLayout(hlay1); ///< 添加顶部水平布局
    vlay->addSpacing(5); ///< 添加额外间距
    vlay->addLayout(hlay2); ///< 添加中间按钮布局
    vlay->addWidget(scrollArea); ///< 添加滚动区域
    vlay->addSpacerItem(new QSpacerItem(QSizePolicy::Preferred, QSizePolicy::Preferred)); ///< 添加底部弹性空间
}

/**
 * @brief 异步加载搜索结果里面的封面图片
 * @param item 音乐项
 * @param imageUrl 封面图片的网络路径
 */
void SearchResultWidget::loadCoverAsync(MusicItemWidget *item, const QString &imageUrl) {
    auto watcher = new QFutureWatcher<QPixmap>(this);
    connect(watcher, &QFutureWatcher<QPixmap>::finished, [this,item, watcher] {
        item->setCover(watcher->result());
        connect(item, &MusicItemWidget::play, [this,item] {
            if (m_playingItem)m_playingItem->setPlayState(false);
            item->setPlayState(true);
            m_playingItem = item;
            emit playMusic(item);
        });
        watcher->deleteLater();
    });
    //qDebug()<<"客户端发出图片请求："<<imageUrl;
    watcher->setFuture(Async::runAsync([this,imageUrl] {
        // 通过服务器API获取图片数据

        const QByteArray response = m_libHttp.UrlRequestGetRaw("http://127.0.0.1:8080/api/getPicture",
            "url=" + QUrl::toPercentEncoding(imageUrl), 3000);

        // 检查响应是否有效
        if (response.isEmpty()) {
            qWarning() << "封面图片请求失败: 空响应";
            return QPixmap();
        }

        // 尝试直接加载为图片
        QPixmap cover;
        if (cover.loadFromData(response)) {
            //qDebug()<<"成功加载图片："<<cover;
            return cover;
        }

        // 如果直接加载失败，尝试解析JSON错误
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(response, &err);

        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            qWarning() << "封面图片请求失败:"
                      << obj["message"].toString()
                      << "状态码:" << obj["code"].toInt();
        } else {
            qWarning() << "封面图片请求失败: 无法解析响应";
        }

        return QPixmap();
    }));
}

/**
 * @brief 异步加载歌曲播放链接
 * @param item 音乐项控件，用于设置播放链接
 * @param songHash 歌曲的唯一标识
 */
void SearchResultWidget::loadSongUrlAsync(MusicItemWidget *item, const QString &songHash) {
    auto watcher = new QFutureWatcher<QString>(this);
    connect(watcher, &QFutureWatcher<QString>::finished, [this,item, watcher] {
        item->setNetUrl(watcher->result());
        //qDebug()<<"成功设置网络路径 :"<<watcher->result();
        watcher->deleteLater();
    });
    watcher->setFuture(Async::runAsync([this, songHash] {
        // 向服务端请求歌曲播放链接
        const QString response = m_libHttp.UrlRequestGet(
            "http://127.0.0.1:8080/api/getSongNetUrl",
            "hash=" + QUrl::toPercentEncoding(songHash),
            3000
        );

        if (response.isEmpty()) {
            qWarning() << "播放链接请求失败: 空响应";
            return QString();
        }

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qWarning() << "播放链接响应无法解析: " << err.errorString();
            return QString();
        }

        QJsonObject obj = doc.object();
        if (obj["code"].toInt() != 0) {
            qWarning() << "播放链接请求失败:" << obj["message"].toString();
            return QString();
        }

        QString netUrl = obj["data"].toObject().value("url").toString();
        return netUrl;
    }));
}

void SearchResultWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->m_refreshMask->setGeometry(rect()); ///< 设置遮罩几何形状
}

void SearchResultWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    this->m_refreshMask->setGeometry(rect()); ///< 设置遮罩几何形状
}
