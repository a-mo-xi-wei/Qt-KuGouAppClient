/**
 * @file Channel.cpp
 * @brief 实现 Channel 类，提供音乐频道主界面功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "Channel.h"
#include "ui_Channel.h"
#include "MyScrollArea.h"
#include "ChannelBlock.h"
#include "Async.h"
#include "RefreshMask.h"

#include <QFile>
#include <QButtonGroup>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQueue>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐频道主界面
 * @param parent 父控件指针，默认为 nullptr
 */
Channel::Channel(QWidget *parent)
    : QWidget(parent), ui(new Ui::Channel), m_buttonGroup(std::make_unique<QButtonGroup>(this)),
      m_refreshMask(std::make_unique<RefreshMask>(this)) ///< 初始化刷新遮罩
{
    ui->setupUi(this); ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channel.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll()); ///< 应用样式表
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
            return;
        }
    }
    QTimer::singleShot(0,
                       this,
                       [this] {
                           initButtonGroup(); ///< 初始化按钮组
                       });
    QTimer::singleShot(10,
                       this,
                       [this] {
                           initTotalWidget(); ///< 初始化分区控件
                           initUi();          ///< 初始化界面
                       });
}

/**
 * @brief 析构函数，清理资源
 */
Channel::~Channel()
{
    delete ui;
}

/**
 * @brief 初始化按钮组
 * @note 设置按钮互斥
 */
void Channel::initButtonGroup() const
{
    this->m_buttonGroup->addButton(ui->recommend_pushButton);          ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->DJ_pushButton);                 ///< 添加 DJ 按钮
    this->m_buttonGroup->addButton(ui->language_pushButton);           ///< 添加语言按钮
    this->m_buttonGroup->addButton(ui->theme_pushButton);              ///< 添加主题按钮
    this->m_buttonGroup->addButton(ui->scene_pushButton);              ///< 添加场景按钮
    this->m_buttonGroup->addButton(ui->mood_pushButton);               ///< 添加心情按钮
    this->m_buttonGroup->addButton(ui->style_pushButton);              ///< 添加风格按钮
    this->m_buttonGroup->addButton(ui->crowd_pushButton);              ///< 添加人群按钮
    this->m_buttonGroup->addButton(ui->children_pushButton);           ///< 添加儿童按钮
    this->m_buttonGroup->addButton(ui->musical_instrument_pushButton); ///< 添加乐器按钮
    this->m_buttonGroup->addButton(ui->label_pushButton);              ///< 添加厂牌按钮
    this->m_buttonGroup->addButton(ui->variety_pushButton);            ///< 添加综艺按钮
    this->m_buttonGroup->addButton(ui->national_customs_pushButton);   ///< 添加国风按钮
    this->m_buttonGroup->addButton(ui->sports_pushButton);             ///< 添加运动按钮
    this->m_buttonGroup->setExclusive(true);                           ///< 设置按钮组互斥
}

/**
 * @brief 初始化分区控件
 */
void Channel::initTotalWidget()
{
    m_recommendWidget = std::make_unique<PartWidget>(this);
    m_djWidget = std::make_unique<PartWidget>(this);
    m_languageWidget = std::make_unique<PartWidget>(this);
    m_themeWidget = std::make_unique<PartWidget>(this);
    m_sceneWidget = std::make_unique<PartWidget>(this);
    m_moodWidget = std::make_unique<PartWidget>(this);
    m_styleWidget = std::make_unique<PartWidget>(this);
    m_crowdWidget = std::make_unique<PartWidget>(this);
    m_childrenWidget = std::make_unique<PartWidget>(this);
    m_musicalInstrumentWidget = std::make_unique<PartWidget>(this);
    m_labelWidget = std::make_unique<PartWidget>(this);
    m_varietyWidget = std::make_unique<PartWidget>(this);
    m_nationalCustomsWidget = std::make_unique<PartWidget>(this);
    m_sportsWidget = std::make_unique<PartWidget>(this);

    this->m_recommendWidget->setTitleName("推荐");         ///< 设置推荐分区标题
    this->m_djWidget->setTitleName("DJ");                ///< 设置 DJ 分区标题
    this->m_languageWidget->setTitleName("语言");          ///< 设置语言分区标题
    this->m_themeWidget->setTitleName("主题");             ///< 设置主题分区标题
    this->m_sceneWidget->setTitleName("场景");             ///< 设置场景分区标题
    this->m_moodWidget->setTitleName("心情");              ///< 设置心情分区标题
    this->m_styleWidget->setTitleName("风格");             ///< 设置风格分区标题
    this->m_crowdWidget->setTitleName("人群");             ///< 设置人群分区标题
    this->m_childrenWidget->setTitleName("儿童");          ///< 设置儿童分区标题
    this->m_musicalInstrumentWidget->setTitleName("乐器"); ///< 设置乐器分区标题
    this->m_labelWidget->setTitleName("厂牌");             ///< 设置厂牌分区标题
    this->m_varietyWidget->setTitleName("综艺");           ///< 设置综艺分区标题
    this->m_nationalCustomsWidget->setTitleName("国风");   ///< 设置国风分区标题
    this->m_sportsWidget->setTitleName("运动");            ///< 设置运动分区标题
}

/**
 * @brief 初始化界面
 * @note 添加分区控件到布局，连接按钮信号，优化滚动
 */
void Channel::initUi()
{
    ui->guide_widget->setStyleSheet("font-family: 'TaiwanPearl';");

    m_refreshMask->keepLoading();
    {
        auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取表格布局
        if (!lay) {
            qWarning() << "布局不存在";
            STREAM_WARN() << "布局不存在"; ///< 记录警告日志
            return;
        }
        lay->insertWidget(lay->count(), this->m_recommendWidget.get());         ///< 添加推荐分区
        lay->insertWidget(lay->count(), this->m_djWidget.get());                ///< 添加 DJ 分区
        lay->insertWidget(lay->count(), this->m_languageWidget.get());          ///< 添加语言分区
        lay->insertWidget(lay->count(), this->m_themeWidget.get());             ///< 添加主题分区
        lay->insertWidget(lay->count(), this->m_sceneWidget.get());             ///< 添加场景分区
        lay->insertWidget(lay->count(), this->m_moodWidget.get());              ///< 添加心情分区
        lay->insertWidget(lay->count(), this->m_styleWidget.get());             ///< 添加风格分区
        lay->insertWidget(lay->count(), this->m_crowdWidget.get());             ///< 添加人群分区
        lay->insertWidget(lay->count(), this->m_childrenWidget.get());          ///< 添加儿童分区
        lay->insertWidget(lay->count(), this->m_musicalInstrumentWidget.get()); ///< 添加乐器分区
        lay->insertWidget(lay->count(), this->m_labelWidget.get());             ///< 添加厂牌分区
        lay->insertWidget(lay->count(), this->m_varietyWidget.get());           ///< 添加综艺分区
        lay->insertWidget(lay->count(), this->m_nationalCustomsWidget.get());   ///< 添加国风分区
        lay->insertWidget(lay->count(), this->m_sportsWidget.get());            ///< 添加运动分区
    }
    {
        auto vScrollBar = ui->scrollArea->verticalScrollBar(); ///< 获取垂直滚动条
        auto connectButton = [this](const QPushButton *button, QWidget *targetWidget) {
            connect(button,
                    &QPushButton::clicked,
                    this,
                    [this, targetWidget] {
                        ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y());
                        ///< 平滑滚动到目标位置
                    });
        };                                                                ///< 批量连接按钮信号
        connectButton(ui->recommend_pushButton, m_recommendWidget.get()); ///< 连接推荐按钮
        connectButton(ui->DJ_pushButton, m_djWidget.get());               ///< 连接 DJ 按钮
        connectButton(ui->language_pushButton, m_languageWidget.get());   ///< 连接语言按钮
        connectButton(ui->theme_pushButton, m_themeWidget.get());         ///< 连接主题按钮
        connectButton(ui->scene_pushButton, m_sceneWidget.get());         ///< 连接场景按钮
        connectButton(ui->mood_pushButton, m_moodWidget.get());           ///< 连接心情按钮
        connectButton(ui->style_pushButton, m_styleWidget.get());         ///< 连接风格按钮
        connectButton(ui->crowd_pushButton, m_crowdWidget.get());         ///< 连接人群按钮
        connectButton(ui->children_pushButton, m_childrenWidget.get());   ///< 连接儿童按钮
        connectButton(ui->musical_instrument_pushButton, m_musicalInstrumentWidget.get());
        ///< 连接乐器按钮
        connectButton(ui->label_pushButton, m_labelWidget.get());                      ///< 连接厂牌按钮
        connectButton(ui->variety_pushButton, m_varietyWidget.get());                  ///< 连接综艺按钮
        connectButton(ui->national_customs_pushButton, m_nationalCustomsWidget.get()); ///< 连接国风按钮
        connectButton(ui->sports_pushButton, m_sportsWidget.get());                    ///< 连接运动按钮
        connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Channel::handleWheelValue);
        ///< 连接滚动区域信号
        connect(vScrollBar, &QScrollBar::valueChanged, this, &Channel::handleWheelValue);
        ///< 连接滚动条信号
    }

    // 异步加载 JSON 文本
    const auto future = Async::runAsync(QThreadPool::globalInstance(),
                                        [this] {
                                            QFile file(
                                                GET_CURRENT_DIR + QStringLiteral("/title.json"));
                                            ///< 加载 JSON 文件
                                            if (!file.open(QIODevice::ReadOnly)) {
                                                qWarning() <<
                                                    "Could not open file for reading title.json";
                                                STREAM_WARN() <<
                                                    "Could not open file for reading title.json";
                                                ///< 记录警告日志
                                                return true;
                                            }
                                            const auto obj =
                                                QJsonDocument::fromJson(file.readAll());
                                            ///< 解析 JSON
                                            auto arr = obj.array();
                                            for (const auto &item : arr) {
                                                auto obj = item.toObject();
                                                this->m_titleVector.emplace_back(
                                                    obj.value(QStringLiteral("title")).toString());
                                            }
                                            file.close();
                                            file.setFileName(
                                                GET_CURRENT_DIR + QStringLiteral("/desc.json"));
                                            ///< 设置为 desc.json
                                            if (!file.open(QIODevice::ReadOnly)) {
                                                qWarning() <<
                                                    "Could not open file for reading desc.json";
                                                STREAM_WARN() <<
                                                    "Could not open file for reading desc.json";
                                                ///< 记录警告日志
                                                return true;
                                            }
                                            const auto descObj = QJsonDocument::fromJson(
                                                file.readAll()); ///< 解析 desc.json
                                            arr = descObj.array();
                                            for (const auto &item : arr) {
                                                auto obj = item.toObject();
                                                this->m_songAndsinger.emplace_back(
                                                    obj.value(QStringLiteral("song")).toString(),
                                                    obj.value(QStringLiteral("singer")).toString());
                                            }

                                            for (int i = 1; i <= 210; ++i) {
                                                this->m_pixPathVector.emplace_back(
                                                    QString(
                                                        ":/BlockCover/Res/blockcover/music-block-cover%1.jpg")
                                                    .arg(i));
                                                ///< 添加封面图片路径
                                            }

                                            unsigned seed = std::chrono::system_clock::now().
                                                            time_since_epoch().count();
                                            ///< 获取当前时间作为随机种子
                                            std::shuffle(this->m_songAndsinger.begin(),
                                                         this->m_songAndsinger.end(),
                                                         std::default_random_engine(seed));
                                            ///< 随机打乱歌曲-歌手
                                            std::shuffle(this->m_pixPathVector.begin(),
                                                         this->m_pixPathVector.end(),
                                                         std::default_random_engine(seed));
                                            ///< 随机打乱封面图片
                                            std::shuffle(this->m_titleVector.begin(),
                                                         this->m_titleVector.end(),
                                                         std::default_random_engine(seed));
                                            return true;
                                        });

    Async::onResultReady(future,
                         this,
                         [this](bool flag) {
                             if (!flag) {
                                 m_refreshMask->hideLoading("");
                                 return;
                             }

                             using Task = std::function<void()>;
                             QVector<Task> tasks;

                             tasks << [this] { loadSectionBlocks(m_recommendWidget.get(), 17, 0); };
                             tasks << [this] { loadSectionBlocks(m_djWidget.get(), 14, 17); };
                             tasks << [this] { loadSectionBlocks(m_languageWidget.get(), 17, 31); };
                             tasks << [this] { loadSectionBlocks(m_themeWidget.get(), 28, 48); };
                             tasks << [this] { loadSectionBlocks(m_sceneWidget.get(), 18, 76); };
                             tasks << [this] { loadSectionBlocks(m_moodWidget.get(), 8, 94); };
                             tasks << [this] { loadSectionBlocks(m_styleWidget.get(), 14, 102); };
                             tasks << [this] { loadSectionBlocks(m_crowdWidget.get(), 4, 116); };
                             tasks << [this] {
                                 loadSectionBlocks(m_childrenWidget.get(), 12, 120);
                             };
                             tasks << [this] {
                                 loadSectionBlocks(m_musicalInstrumentWidget.get(), 11, 132);
                             };
                             tasks << [this] { loadSectionBlocks(m_labelWidget.get(), 6, 143); };
                             tasks << [this] { loadSectionBlocks(m_varietyWidget.get(), 27, 149); };
                             tasks << [this] {
                                 loadSectionBlocks(m_nationalCustomsWidget.get(), 6, 176);
                             };
                             tasks << [this] {
                                 loadSectionBlocks(m_sportsWidget.get(), 7, 182);
                                 m_refreshMask->hideLoading(""); ///< 所有加载完成后隐藏遮罩
                                 QMetaObject::invokeMethod(this,
                                                           "emitInitialized",
                                                           Qt::QueuedConnection);
                             };

                             auto queue = std::make_shared<QQueue<Task>>();
                             for (const auto &task : tasks)
                                 queue->enqueue(task);

                             auto runner = std::make_shared<std::function<void()>>();
                             *runner = [queue, runner]() {
                                 if (queue->isEmpty())
                                     return;

                                 auto task = queue->dequeue();
                                 QTimer::singleShot(0,
                                                    nullptr,
                                                    [task, runner]() {
                                                        task();
                                                        (*runner)();
                                                    });
                             };
                             (*runner)();
                         });
}

void Channel::loadSectionBlocks(PartWidget *section, const int &cnt, const int &sum)
{
    for (int i = 1; i <= cnt; ++i) {
        auto block = new ChannelBlock(this);
        block->setCoverPix(this->m_pixPathVector[i + sum]);
        block->setTitleText(m_titleVector[i]);
        block->setSingerSongText(
            this->m_songAndsinger[i + sum].first + " - " + this->m_songAndsinger[i + sum].second);
        section->addBlockWidget(block);
    }
}

void Channel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise(); // 确保遮罩在最上层
}

void Channel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise(); // 确保遮罩在最上层
}

/**
 * @brief 处理滚动条值变化
 * @param value 滚动条值
 */
void Channel::handleWheelValue(const int &value)
{
    const QVector<QPair<QWidget *, QPushButton *>> sectionMappings = {
        {m_recommendWidget.get(), ui->recommend_pushButton},
        {m_djWidget.get(), ui->DJ_pushButton},
        {m_languageWidget.get(), ui->language_pushButton},
        {m_themeWidget.get(), ui->theme_pushButton},
        {m_sceneWidget.get(), ui->scene_pushButton},
        {m_moodWidget.get(), ui->mood_pushButton},
        {m_styleWidget.get(), ui->style_pushButton},
        {m_crowdWidget.get(), ui->crowd_pushButton},
        {m_childrenWidget.get(), ui->children_pushButton},
        {
            m_musicalInstrumentWidget.get(),
            ui->musical_instrument_pushButton
        },
        {m_labelWidget.get(), ui->label_pushButton},
        {m_varietyWidget.get(), ui->variety_pushButton},
        {
            m_nationalCustomsWidget.get(),
            ui->national_customs_pushButton
        },
        {m_sportsWidget.get(), ui->sports_pushButton}
    };
    int currentY, nextY;
    for (int i = 0; i < sectionMappings.size(); ++i) {
        QWidget *currentWidget = sectionMappings[i].first;
        QWidget *nextWidget = (i + 1 < sectionMappings.size())
                                  ? sectionMappings[i + 1].first
                                  : nullptr;

        currentY = currentWidget->mapToParent(QPoint(0, 0)).y();
        nextY = nextWidget ? nextWidget->mapToParent(QPoint(0, 0)).y() : INT_MAX;

        if (value >= currentY && value < nextY) {
            sectionMappings[i].second->setChecked(true);
            break;
        }
    }
}