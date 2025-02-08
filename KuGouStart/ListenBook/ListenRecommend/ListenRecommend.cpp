//
// Created by WeiWang on 25-2-2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListenRecommend.h" resolved

#include "ListenRecommend.h"
#include "ui_ListenRecommend.h"
#include "MyMenu.h"

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
    const auto audioNovel = new ListenTableWidget(ui->table_widgets);
    audioNovel->setCnt(1);
    audioNovel->setTitle("有声小说");
    initOtherGalleryWidget("audioNovel",audioNovel);

    lay->addWidget(audioNovel);
}

void ListenRecommend::initDailyRecommendGalleryWidget() {
    //解析json文件
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/jsonFiles/dailyRecommend.json"));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading dailyRecommend.json";
            return;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr) {
            QString title = item.toObject().value("title").toString();
            QString playCount = item.toObject().value("play_count").toString();
            this->m_galleryVector[0].emplace_back(title,playCount);
        }
        file.close();
    }
    const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。";
    for (int i = 0 ; i < 10 ; ++i)
    {
        const auto it = new GalleryPhotoWidget(ui->daily_recommend_widget->getGalleryWidget());
        it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(20 + idx[0]));
        it->setTitleText(this->m_galleryVector[0][idx[0]].first);
        it->setPopularText(this->m_galleryVector[0][idx[0]].second);
        it->setDescribeText(subTitle);
        ui->daily_recommend_widget->getGalleryWidget()->addData(it);
        idx[0] = ++idx[0] % static_cast<int>(this->m_galleryVector[0].size());
    }

}

void ListenRecommend::initOtherGalleryWidget(const QString &jsonFileName, const ListenTableWidget *gallery) {
    const auto cnt = gallery->getCnt();
    //解析json文件
    {
        QFile file(GET_CURRENT_DIR + QString("/jsonFiles/%1.json").arg(jsonFileName));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("Could not open file for reading %1.json").arg(jsonFileName);
            return;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr) {
            QString title = item.toObject().value("desc").toString();
            QString playCount = item.toObject().value("people").toString();
            this->m_galleryVector[cnt].emplace_back(title,playCount);
        }
        file.close();
    }
    const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。";
    for (int i = 0 ; i < 10 ; ++i)
    {
        const auto it = new GalleryPhotoWidget(gallery->getGalleryWidget());
        it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg((cnt+1)*20 + idx[cnt]));
        it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first);
        it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second);
        it->setDescribeText(subTitle);
        gallery->getGalleryWidget()->addData(it);
        idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size());
    }

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

void ListenRecommend::onToolButtonClicked(const int& cnt) {
    // 启动定时器，延迟处理
    if (!this->m_refreshTimer->isActive()) {
        this->m_refreshTimer->start(500); // 500ms 延迟，避免过于频繁地触发
    }
    refreshObj = qobject_cast<ListenTableWidget*>(sender());
}

void ListenRecommend::onRefreshTimeout() {
//qDebug()<<"刷新一下";
    const auto cnt = refreshObj->getCnt();
    for (const auto& it : refreshObj->getGalleryWidget()->getWidgets())
    {
        it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg((cnt +1)*20 + idx[cnt]));
        it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first);
        it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second);
        it->update();
        idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size());

    }
}

