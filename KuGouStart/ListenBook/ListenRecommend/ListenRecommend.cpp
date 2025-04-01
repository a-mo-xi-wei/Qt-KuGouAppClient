//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecommend.h" resolved

#include "ListenRecommend.h"
#include "ui_ListenRecommend.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "Async.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

static int idx[20] = {0};
static ListenTableWidget* refreshObj{};

ListenRecommend::ListenRecommend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecommend)
    , m_refreshTimer(new QTimer(this))

{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
    auto menu = new MyMenu(MyMenu::MenuKind::ListenOption,this);
    m_menu = menu->getMenu<ListenOptionMenu>();

    connect(ui->daily_recommend_widget,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    connect(this->m_refreshTimer, &QTimer::timeout, this, &ListenRecommend::onRefreshTimeout);

}

ListenRecommend::~ListenRecommend() {
    delete ui;
}

void ListenRecommend::initUi() {
    //设置全部分类按钮
    ui->all_classify_toolButton->setHoverFontColor(QColor(QStringLiteral("#26A1FF")));
    ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg")));
    ui->all_classify_toolButton->setIconSize(QSize(10, 10));
    ui->all_classify_toolButton->setApproach(true);

    //设置定时器只启动一次
    this->m_refreshTimer->setSingleShot(true);

    //设置每日推荐画廊
    initDailyRecommendGalleryWidget();

    //初始化其他画廊
    initTableWidgets();
}

void ListenRecommend::initTableWidgets() {
    const auto lay = new QVBoxLayout(ui->table_widgets);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    //有声小说
    const auto audioNovel = new ListenTableWidget(ui->table_widgets);
    audioNovel->setCnt(1);
    audioNovel->setTitle("有声小说");
    connect(audioNovel,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("audioNovel",audioNovel);
    //儿童天地
    const auto childrenWorld = new ListenTableWidget(ui->table_widgets);
    childrenWorld->setCnt(2);
    childrenWorld->setTitle("儿童天地");
    connect(childrenWorld,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("childrenWorld",childrenWorld);
    //评书
    const auto commentBook = new ListenTableWidget(ui->table_widgets);
    commentBook->setCnt(3);
    commentBook->setTitle("评书");
    connect(commentBook,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("commentBook",commentBook);
    //助眠解压
    const auto helpSleep = new ListenTableWidget(ui->table_widgets);
    helpSleep->setCnt(4);
    helpSleep->setTitle("助眠解压");
    connect(helpSleep,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("sleepHelp",helpSleep);
    //人文
    const auto humanities = new ListenTableWidget(ui->table_widgets);
    humanities->setCnt(5);
    humanities->setTitle("人文");
    connect(humanities,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("humanity",humanities);
    //自我充电
    const auto selfCharge = new ListenTableWidget(ui->table_widgets);
    selfCharge->setCnt(6);
    selfCharge->setTitle("自我充电");
    connect(selfCharge,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("chongdian",selfCharge);
    //相声曲艺
    const auto voiceArt = new ListenTableWidget(ui->table_widgets);
    voiceArt->setCnt(7);
    voiceArt->setTitle("相声曲艺");
    connect(voiceArt,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("xiangsheng",voiceArt);
    //情感生活
    const auto emotionLife = new ListenTableWidget(ui->table_widgets);
    emotionLife->setCnt(8);
    emotionLife->setTitle("情感生活");
    connect(emotionLife,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("qinggan",emotionLife);
    //广播剧
    const auto radioDrama = new ListenTableWidget(ui->table_widgets);
    radioDrama->setCnt(9);
    radioDrama->setTitle("广播剧");
    connect(radioDrama,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("guangboju",radioDrama);
    //娱乐段子
    const auto entertainmentJoke = new ListenTableWidget(ui->table_widgets);
    entertainmentJoke->setCnt(10);
    entertainmentJoke->setTitle("娱乐段子");
    connect(entertainmentJoke,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("yule",entertainmentJoke);
    //二次元
    const auto anime = new ListenTableWidget(ui->table_widgets);
    anime->setCnt(11);
    anime->setTitle("二次元");
    connect(anime,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("erciyuan",anime);
    //播客
    const auto podcast = new ListenTableWidget(ui->table_widgets);
    podcast->setCnt(12);
    podcast->setTitle("播客");
    connect(podcast,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("boke",podcast);
    //粤语
    const auto yue = new ListenTableWidget(ui->table_widgets);
    yue->setCnt(13);
    yue->setTitle("粤语");
    connect(yue,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("yueyu",yue);
    //外语
    const auto foreignLanguage = new ListenTableWidget(ui->table_widgets);
    foreignLanguage->setCnt(14);
    foreignLanguage->setTitle("外语");
    connect(foreignLanguage,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("waiyu",foreignLanguage);
    //创作翻唱
    const auto creativeSing = new ListenTableWidget(ui->table_widgets);
    creativeSing->setCnt(15);
    creativeSing->setTitle("创作翻唱");
    connect(creativeSing,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("createCover",creativeSing);
    //DJ电音
    const auto djElectronic = new ListenTableWidget(ui->table_widgets);
    djElectronic->setCnt(16);
    djElectronic->setTitle("DJ电音");
    connect(djElectronic,&ListenTableWidget::toolBtnClicked,this,&ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("djElectronic",djElectronic);


    lay->addWidget(audioNovel);
    lay->addWidget(childrenWorld);
    lay->addWidget(commentBook);
    lay->addWidget(helpSleep);
    lay->addWidget(humanities);
    lay->addWidget(selfCharge);
    lay->addWidget(voiceArt);
    lay->addWidget(emotionLife);
    lay->addWidget(radioDrama);
    lay->addWidget(entertainmentJoke);
    lay->addWidget(anime);
    lay->addWidget(podcast);
    lay->addWidget(yue);
    lay->addWidget(foreignLanguage);
    lay->addWidget(creativeSing);
    lay->addWidget(djElectronic);

}

void ListenRecommend::initDailyRecommendGalleryWidget() {
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/jsonFiles/dailyRecommend.json");
    // 异步解析 JSON文件
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [jsonPath] {
        QList<QPair<QString, QString>> result;
        QFile file(jsonPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading dailyRecommend.json";
            STREAM_WARN() << "Failed to open JSON file:" << jsonPath.toStdString();
            return result;
        }

        QJsonParseError parseError;
        const auto doc = QJsonDocument::fromJson(file.readAll(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error:" << parseError.errorString();
            STREAM_WARN() << "JSON parse error:" << parseError.errorString().toStdString();
            return result;
        }

        const auto arr = doc.array();
        for (const auto& item : arr) {
            const auto obj = item.toObject();
            result.append(qMakePair(
                obj.value("title").toString(),
                obj.value("play_count").toString()
            ));
        }
        file.close();
        return result;
    });
    // 异步结果处理
    Async::onResultReady(future, this, [this](const QList<QPair<QString, QString>>& data) {
        if (data.isEmpty()) {
            qWarning() << "Daily recommend data is empty or failed to parse";
            STREAM_WARN() << "Daily recommend data is empty or failed to parse";
            return;
        }
        // 将 QList 转换为 QVector
        this->m_galleryVector[0] = std::vector(data.cbegin(), data.cend());

        const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。";
        for (int i = 0 ; i < 10 ; ++i)
        {
            const auto it = new GalleryPhotoWidget(ui->daily_recommend_widget->getGalleryWidget());
            it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + idx[0]));
            it->setTitleText(this->m_galleryVector[0][idx[0]].first);
            it->setPopularText(this->m_galleryVector[0][idx[0]].second);
            it->setDescribeText(subTitle);
            ui->daily_recommend_widget->getGalleryWidget()->addData(it);
            idx[0] = ++idx[0] % static_cast<int>(this->m_galleryVector[0].size());
        }
    });

}

void ListenRecommend::initOtherGalleryWidget(const QString &jsonFileName, const ListenTableWidget *gallery) {
    const auto cnt = gallery->getCnt();
    // 异步解析 JSON 文件
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [jsonFileName, cnt]() {
        QList<QPair<QString, QString>> result;
        QFile file(GET_CURRENT_DIR + QString("/jsonFiles/%1.json").arg(jsonFileName));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("Could not open file for reading %1.json").arg(jsonFileName);
            STREAM_WARN() << QString("Could not open file for reading %1.json").arg(jsonFileName).toStdString();
            return result;
        }
        const auto doc = QJsonDocument::fromJson(file.readAll());
        auto arr = doc.array();
        for (const auto &item : arr) {
            QString title = item.toObject().value("desc").toString();
            QString playCount = item.toObject().value("people").toString();
            result.append(qMakePair(title, playCount));
        }
        file.close();
        return result;
    });

    // 异步结果处理
    Async::onResultReady(future, this, [this, cnt, gallery](const QList<QPair<QString, QString>>& data) {
        if (data.isEmpty()) {
            qWarning() << QString("%1.json is empty or failed to parse").arg(gallery->objectName());
            STREAM_WARN() << QString("%1.json is empty or failed to parse").arg(gallery->objectName()).toStdString();
            return;
        }
        // 将 QList 转换为 QVector
        this->m_galleryVector[cnt] = std::vector(data.cbegin(), data.cend());

        const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。";
        for (int i = 0; i < 10; ++i) {
            const auto it = new GalleryPhotoWidget(gallery->getGalleryWidget());
            it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + cnt * 40 + idx[cnt]));
            it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first);
            it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second);
            it->setDescribeText(subTitle);
            gallery->getGalleryWidget()->addData(it);
            idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size());
        }
    });
}

void ListenRecommend::on_all_classify_toolButton_clicked() {
    //qDebug()<<"点击了全部分类按钮";
    if (ui->all_classify_toolButton->isChecked()){
        // 更新按钮图标为向上图标
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));

        // 设置菜单的位置
        const QPoint globalPos = ui->all_classify_toolButton->mapToGlobal(
            QPoint(ui->all_classify_toolButton->width() - m_menu->width(),
                   ui->all_classify_toolButton->height() + 10));

        // 设置菜单属性，不夺取焦点
        //m_menu->setWindowFlags(static_cast<Qt::WindowFlags>(m_menu->windowFlags() | Qt::NoFocus));
        // 菜单显示为非阻塞模式
        //m_menu->setWindowFlags(Qt::Popup );
        m_menu->setFocusPolicy(Qt::NoFocus);
        m_menu->setAttribute(Qt::WA_TransparentForMouseEvents, false);

        // 连接菜单的隐藏信号到按钮状态更新槽
        connect(m_menu, &QMenu::aboutToHide, this, [this]() {
            ui->all_classify_toolButton->setChecked(false);
            ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
            ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
            ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        });

        // 显示菜单
        m_menu->exec(globalPos);
    }
    else {
        // 更新按钮图标为向下图标
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    }
}

void ListenRecommend::onToolButtonClicked() {
    // 启动定时器，延迟处理
    if (!this->m_refreshTimer->isActive()) {
        //qDebug()<<"启动定时器";
        this->m_refreshTimer->start(500); // 500ms 延迟，避免过于频繁地触发
    }
    refreshObj = qobject_cast<ListenTableWidget*>(sender());
}

void ListenRecommend::onRefreshTimeout() {
    //qDebug()<<"刷新一下";
    const auto cnt = refreshObj->getCnt();
    //qDebug()<<"当前刷新第："<<cnt<<" 个tableWidget";
    for (const auto& it : refreshObj->getGalleryWidget()->getWidgets())
    {
        it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + cnt*40 + idx[cnt] % 40));
        it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first);
        it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second);
        it->update();
        idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size());
    }
    ElaMessageBar::success(ElaMessageBarType::BottomRight,"Success",
            refreshObj->getTitle() + " 换一批成功",2000,this->window());

}

