/**
 * @file VideoChannelWidget.cpp
 * @brief 实现 VideoChannelWidget 类，提供视频频道分类界面功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "VideoChannelWidget.h"
#include "ui_VideoChannelWidget.h"
#include "logger.hpp"
#include "Async.h"
#include "RefreshMask.h"

#include <QFile>
#include <QButtonGroup>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化视频频道界面
 * @param parent 父控件指针，默认为 nullptr
 */
VideoChannelWidget::VideoChannelWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoChannelWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_refreshMask(std::make_unique<RefreshMask>(this))                 ///< 初始化刷新遮罩
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channelwidget.css"));
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());                  ///< 加载样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initButtonGroup();                                            ///< 初始化按钮组
    initTotalWidget();                                            ///< 初始化分类部件
    initUi();                                                     ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
VideoChannelWidget::~VideoChannelWidget()
{
    delete ui;
}

/**
 * @brief 初始化按钮组
 */
void VideoChannelWidget::initButtonGroup() const
{
    this->m_buttonGroup->addButton(ui->popular_pushButton);       ///< 添加热门按钮
    this->m_buttonGroup->addButton(ui->children_pushButton);      ///< 添加儿童按钮
    this->m_buttonGroup->addButton(ui->theme_pushButton);         ///< 添加主题按钮
    this->m_buttonGroup->addButton(ui->film_pushButton);          ///< 添加影视按钮
    this->m_buttonGroup->addButton(ui->variety_pushButton);       ///< 添加综艺按钮
    this->m_buttonGroup->addButton(ui->ACGN_pushButton);          ///< 添加二次元按钮
    this->m_buttonGroup->addButton(ui->scene_pushButton);         ///< 添加场景按钮
    this->m_buttonGroup->addButton(ui->language_pushButton);      ///< 添加语言按钮
    this->m_buttonGroup->addButton(ui->dance_pushButton);         ///< 添加舞蹈按钮
    this->m_buttonGroup->addButton(ui->site_pushButton);          ///< 添加现场按钮
    this->m_buttonGroup->addButton(ui->singer_pushButton);        ///< 添加歌手按钮
    this->m_buttonGroup->setExclusive(true);                      ///< 设置互斥
}

/**
 * @brief 初始化分类部件
 */
void VideoChannelWidget::initTotalWidget()
{
    m_popularWidget     = std::make_unique<VideoChannelPartWidget>(this);
    m_childrenWidget    = std::make_unique<VideoChannelPartWidget>(this);
    m_themeWidget       = std::make_unique<VideoChannelPartWidget>(this);
    m_filmWidget        = std::make_unique<VideoChannelPartWidget>(this);
    m_varietyWidget     = std::make_unique<VideoChannelPartWidget>(this);
    m_ACGNWidget        = std::make_unique<VideoChannelPartWidget>(this);
    m_sceneWidget       = std::make_unique<VideoChannelPartWidget>(this);
    m_languageWidget    = std::make_unique<VideoChannelPartWidget>(this);
    m_danceWidget       = std::make_unique<VideoChannelPartWidget>(this);
    m_siteWidget        = std::make_unique<VideoChannelPartWidget>(this);
    m_singerWidget      = std::make_unique<VideoChannelPartWidget>(this);

    this->m_popularWidget->setTitleName("热门");                  ///< 设置热门标题
    this->m_childrenWidget->setTitleName("儿童");                 ///< 设置儿童标题
    this->m_themeWidget->setTitleName("主题");                    ///< 设置主题标题
    this->m_filmWidget->setTitleName("影视");                     ///< 设置影视标题
    this->m_varietyWidget->setTitleName("综艺");                  ///< 设置综艺标题
    this->m_ACGNWidget->setTitleName("二次元");                   ///< 设置二次元标题
    this->m_sceneWidget->setTitleName("场景");                    ///< 设置场景标题
    this->m_languageWidget->setTitleName("语言");                 ///< 设置语言标题
    this->m_danceWidget->setTitleName("舞蹈");                    ///< 设置舞蹈标题
    this->m_siteWidget->setTitleName("现场");                     ///< 设置现场标题
    this->m_singerWidget->setTitleName("歌手");                   ///< 设置歌手标题
}

/**
 * @brief 初始化界面
 */
void VideoChannelWidget::initUi()
{
    m_refreshMask->keepLoading();
    auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取垂直布局
    lay->setSpacing(0);                                           ///< 设置间距
    if (!lay)
    {
        qWarning() << "布局不存在";
        STREAM_WARN() << "布局不存在";
        return;
    }
    lay->insertWidget(lay->count(), m_popularWidget.get());   ///< 插入热门部件
    lay->insertWidget(lay->count(), m_childrenWidget.get());  ///< 插入儿童部件
    lay->insertWidget(lay->count(), m_themeWidget.get());     ///< 插入主题部件
    lay->insertWidget(lay->count(), m_filmWidget.get());      ///< 插入影视部件
    lay->insertWidget(lay->count(), m_varietyWidget.get());   ///< 插入综艺部件
    lay->insertWidget(lay->count(), m_ACGNWidget.get());      ///< 插入二次元部件
    lay->insertWidget(lay->count(), m_sceneWidget.get());     ///< 插入场景部件
    lay->insertWidget(lay->count(), m_languageWidget.get());  ///< 插入语言部件
    lay->insertWidget(lay->count(), m_danceWidget.get());     ///< 插入舞蹈部件
    lay->insertWidget(lay->count(), m_siteWidget.get());      ///< 插入现场部件
    lay->insertWidget(lay->count(), m_singerWidget.get());    ///< 插入歌手部件

    auto vScrollBar = ui->scrollArea->verticalScrollBar();     ///< 获取垂直滚动条
    auto connectButton = [this](const QPushButton *button, QWidget *targetWidget) {
        connect(button, &QPushButton::clicked, this, [this, targetWidget] {
            ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y()); ///< 平滑滚动
        });
    };
    connectButton(ui->popular_pushButton, this->m_popularWidget.get());   ///< 连接热门按钮
    connectButton(ui->children_pushButton, this->m_childrenWidget.get()); ///< 连接儿童按钮
    connectButton(ui->theme_pushButton, this->m_themeWidget.get());       ///< 连接主题按钮
    connectButton(ui->film_pushButton, this->m_filmWidget.get());         ///< 连接影视按钮
    connectButton(ui->variety_pushButton, this->m_varietyWidget.get());   ///< 连接综艺按钮
    connectButton(ui->ACGN_pushButton, this->m_ACGNWidget.get());         ///< 连接二次元按钮
    connectButton(ui->scene_pushButton, this->m_sceneWidget.get());       ///< 连接场景按钮
    connectButton(ui->language_pushButton, this->m_languageWidget.get()); ///< 连接语言按钮
    connectButton(ui->dance_pushButton, this->m_danceWidget.get());       ///< 连接舞蹈按钮
    connectButton(ui->site_pushButton, this->m_siteWidget.get());         ///< 连接现场按钮
    connectButton(ui->singer_pushButton, this->m_singerWidget.get());     ///< 连接歌手按钮

    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &VideoChannelWidget::handleWheelValue); ///< 连接滚动信号
    connect(vScrollBar, &QScrollBar::valueChanged, this, &VideoChannelWidget::handleWheelValue); ///< 连接滚动条信号

    // 异步加载 JSON 文本
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this] {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/videochannel.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly)) {
          qWarning() << "Could not open file for reading videochannel.json";
          STREAM_WARN() << "Could not open file for reading videochannel.json"; ///< 记录警告日志
          return true;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr       = obj.array();
        for (const auto &item: arr) {
          auto obj = item.toObject();
          this->m_coverTextVector.emplace_back(obj.value(QStringLiteral("coverText")).toString());
        }
        file.close();

        for (int i = 1; i <= 120; ++i)
        {
            this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i)); ///< 添加图片路径
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取随机种子
        std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed)); ///< 随机打乱
        std::shuffle(this->m_coverTextVector.begin(), this->m_coverTextVector.end(), std::default_random_engine(seed));
        return true;
    });

    Async::onResultReady(future, this, [this](bool flag) {
        QTimer::singleShot(0, this, [this] {loadSectionBlocks(m_popularWidget.get(),10,0);});
        QTimer::singleShot(100, this, [this] { loadSectionBlocks(m_childrenWidget.get(),14, 10); });
        QTimer::singleShot(200, this, [this] { loadSectionBlocks(m_themeWidget.get(), 10, 24); });
        QTimer::singleShot(300, this, [this] { loadSectionBlocks(m_filmWidget.get(), 7, 34); });
        QTimer::singleShot(400, this, [this] { loadSectionBlocks(m_varietyWidget.get(), 1, 41); });
        QTimer::singleShot(500, this, [this] { loadSectionBlocks(m_ACGNWidget.get(), 6, 42); });
        QTimer::singleShot(600, this, [this] { loadSectionBlocks(m_sceneWidget.get(), 3, 48); });
        QTimer::singleShot(700, this, [this] { loadSectionBlocks(m_languageWidget.get(), 9, 51); });
        QTimer::singleShot(800, this, [this] { loadSectionBlocks(m_danceWidget.get(), 3, 60); });
        QTimer::singleShot(900, this, [this] { loadSectionBlocks(m_siteWidget.get(), 14, 63); });
        QTimer::singleShot(1000, this, [this] {
            loadSectionBlocks(m_singerWidget.get(), 26, 77);
            m_refreshMask->hideLoading("");
        });

    });
}

void VideoChannelWidget::loadSectionBlocks(VideoChannelPartWidget *section, const int &cnt, const int &sum) {
    for (int i = 1; i <= cnt; ++i)
    {
        auto block = new VideoChannelBlock(this);
        block->setCoverPix(this->m_pixPathVector[i + sum]);   ///< 设置封面
        block->setCoverText(m_coverTextVector[i]);                    ///< 设置文本
        section->addBlockWidget(block);          ///< 添加到歌手
    }
}

void VideoChannelWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise();  // 确保遮罩在最上层
}

/**
 * @brief 处理滚动值变化
 * @param value 滚动条值
 */
void VideoChannelWidget::handleWheelValue(const int &value)
{
    const QVector<QPair<QWidget*, QPushButton*>> sectionMappings = {
        {m_popularWidget.get(),     ui->popular_pushButton},
        {m_childrenWidget.get(),    ui->children_pushButton},
        {m_themeWidget.get(),       ui->theme_pushButton},
        {m_filmWidget.get(),        ui->film_pushButton},
        {m_varietyWidget.get(),     ui->variety_pushButton},
        {m_ACGNWidget.get(),        ui->ACGN_pushButton},
        {m_sceneWidget.get(),       ui->scene_pushButton},
        {m_languageWidget.get(),    ui->language_pushButton},
        {m_danceWidget.get(),       ui->dance_pushButton},
        {m_siteWidget.get(),        ui->site_pushButton},
        {m_singerWidget.get(),      ui->singer_pushButton}
    };

    for (int i = 0; i < sectionMappings.size(); ++i) {
        QWidget* currentWidget = sectionMappings[i].first;
        QWidget* nextWidget = (i + 1 < sectionMappings.size()) ? sectionMappings[i + 1].first : nullptr;

        int currentY = currentWidget->mapToParent(QPoint(0, 0)).y();
        int nextY = nextWidget ? nextWidget->mapToParent(QPoint(0, 0)).y() : INT_MAX;

        if (value >= currentY && value < nextY) {
            sectionMappings[i].second->setChecked(true);
            break;
        }
    }
}
