/**
 * @file VideoWidget.cpp
 * @brief 实现 VideoWidget 类，管理视频界面
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "VideoWidget.h"
#include "ui_VideoWidget.h"
#include "logger.hpp"
#include "Async.h"
#include "RefreshMask.h"
#include "QPushButton.h"

#include <QFile>
#include <QButtonGroup>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QQueue>
#include <QScrollBar>
#include <QTimer>
#include <random>

#include "ElaFlowLayout.h"


/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 滚动区域垂直偏移量 */
int offY = 297;

/**
 * @brief 构造函数，初始化视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
VideoWidget::VideoWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::VideoWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this)) ///< 初始化按钮组
    , m_refreshMask(std::make_unique<RefreshMask>(this))  ///< 初始化刷新遮罩
{
    ui->setupUi(this);                                          ///< 设置 UI 布局
    QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll()); ///< 应用样式表
    }
    else
    {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
        return;
    }
    initButtonGroup(); ///< 初始化按钮组
    initTotalWidget(); ///< 初始化视频分区
    initUi();          ///< 初始化界面
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
VideoWidget::~VideoWidget()
{
    delete ui; ///< 释放 UI 界面
}

/**
 * @brief 初始化按钮组
 * @note 设置互斥按钮
 */
void VideoWidget::initButtonGroup()
{
    // 初始化按钮
    m_recommend_pushButton        = new QPushButton(tr("推荐"));
    m_video_rank_pushButton       = new QPushButton(tr("视频榜"));
    m_MV_pushButton               = new QPushButton(tr("MV"));
    m_site_pushButton             = new QPushButton(tr("现场"));
    m_cover_pushButton            = new QPushButton(tr("翻唱"));
    m_dance_pushButton            = new QPushButton(tr("舞蹈"));
    m_children_pushButton         = new QPushButton(tr("儿童"));
    m_live_pushButton             = new QPushButton(tr("TME LIVE"));
    m_first_concert_pushButton    = new QPushButton(tr("首唱会"));
    m_chinese_language_pushButton = new QPushButton(tr("华语"));
    m_South_Korea_pushButton      = new QPushButton(tr("韩国"));
    m_Japan_pushButton            = new QPushButton(tr("日本"));
    m_American_pushButton         = new QPushButton(tr("欧美"));

    // 设置按钮通用属性
    QList<QPushButton*> buttons =
    {
        m_recommend_pushButton, m_video_rank_pushButton, m_MV_pushButton,
        m_site_pushButton, m_cover_pushButton, m_dance_pushButton,
        m_children_pushButton, m_live_pushButton, m_first_concert_pushButton,
        m_chinese_language_pushButton, m_South_Korea_pushButton,
        m_Japan_pushButton, m_American_pushButton
    };

    for (auto btn : buttons)
    {
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        btn->setFixedSize(80, 30);
        m_buttonGroup->addButton(btn); // 添加到按钮组
    }
    m_recommend_pushButton->setChecked(true);

    m_buttonGroup->setExclusive(true);

    // 创建并设置流式布局
    auto lay = new ElaFlowLayout(ui->title_widget, 5, 8, 6);
    lay->setIsAnimation(true);

    // 将所有按钮加入布局
    for (auto btn : buttons)
    {
        lay->addWidget(btn);
    }

    ui->title_widget->setLayout(lay);
    ui->title_widget->setStyleSheet("font-family: 'TaiwanPearl';font-size: 14px;");
}

/**
 * @brief 初始化视频分区
 * @note 设置 13 个分区的标题
 */
void VideoWidget::initTotalWidget()
{
    m_recommendWidget       = std::make_unique<VideoPartWidget>(this);
    m_videoRankWidget       = std::make_unique<VideoPartWidget>(this);
    m_MVWidget              = std::make_unique<VideoPartWidget>(this);
    m_siteWidget            = std::make_unique<VideoPartWidget>(this);
    m_coverWidget           = std::make_unique<VideoPartWidget>(this);
    m_danceWidget           = std::make_unique<VideoPartWidget>(this);
    m_childrenWidget        = std::make_unique<VideoPartWidget>(this);
    m_liveWidget            = std::make_unique<VideoPartWidget>(this);
    m_firstConcertWidget    = std::make_unique<VideoPartWidget>(this);
    m_chineseLanguageWidget = std::make_unique<VideoPartWidget>(this);
    m_southKoreaWidget      = std::make_unique<VideoPartWidget>(this);
    m_japanWidget           = std::make_unique<VideoPartWidget>(this);
    m_americanWidget        = std::make_unique<VideoPartWidget>(this);

    this->m_recommendWidget->setTitleName("推荐");       ///< 设置推荐标题
    this->m_videoRankWidget->setTitleName("视频榜");      ///< 设置视频榜标题
    this->m_MVWidget->setTitleName("MV");              ///< 设置 MV 标题
    this->m_siteWidget->setTitleName("现场");            ///< 设置现场标题
    this->m_coverWidget->setTitleName("翻唱");           ///< 设置翻唱标题
    this->m_danceWidget->setTitleName("舞蹈");           ///< 设置舞蹈标题
    this->m_childrenWidget->setTitleName("儿童");        ///< 设置儿童标题
    this->m_liveWidget->setTitleName("TME LIVE");      ///< 设置 TME LIVE 标题
    this->m_firstConcertWidget->setTitleName("首唱会");   ///< 设置首唱会标题
    this->m_chineseLanguageWidget->setTitleName("华语"); ///< 设置华语标题
    this->m_southKoreaWidget->setTitleName("韩国");      ///< 设置韩国标题
    this->m_japanWidget->setTitleName("日本");           ///< 设置日本标题
    this->m_americanWidget->setTitleName("欧美");        ///< 设置欧美标题
}

/**
 * @brief 初始化界面
 * @note 设置布局、信号和视频块
 */
void VideoWidget::initUi()
{
    m_refreshMask->keepLoading();
    // 加入布局
    auto lay = dynamic_cast<QVBoxLayout*>(ui->table_widget->layout()); ///< 获取垂直布局
    lay->setSpacing(0);                                                ///< 设置间距
    lay->insertWidget(lay->count(), m_recommendWidget.get());          ///< 添加推荐分区
    lay->insertWidget(lay->count(), m_videoRankWidget.get());          ///< 添加视频榜分区
    lay->insertWidget(lay->count(), m_MVWidget.get());                 ///< 添加 MV 分区
    lay->insertWidget(lay->count(), m_siteWidget.get());               ///< 添加现场分区
    lay->insertWidget(lay->count(), m_coverWidget.get());              ///< 添加翻唱分区
    lay->insertWidget(lay->count(), m_danceWidget.get());              ///< 添加舞蹈分区
    lay->insertWidget(lay->count(), m_childrenWidget.get());           ///< 添加儿童分区
    lay->insertWidget(lay->count(), m_liveWidget.get());               ///< 添加 TME LIVE 分区
    lay->insertWidget(lay->count(), m_firstConcertWidget.get());       ///< 添加首唱会分区
    lay->insertWidget(lay->count(), m_chineseLanguageWidget.get());    ///< 添加华语分区
    lay->insertWidget(lay->count(), m_southKoreaWidget.get());         ///< 添加韩国分区
    lay->insertWidget(lay->count(), m_japanWidget.get());              ///< 添加日本分区
    lay->insertWidget(lay->count(), m_americanWidget.get());           ///< 添加欧美分区

    // 处理信号
    auto vScrollBar    = ui->scrollArea->verticalScrollBar(); ///< 获取垂直滚动条
    auto connectButton = [this](const QPushButton * button, QWidget * targetWidget)
    {
        connect(button, &QPushButton::clicked, this, [this, targetWidget]
        {
            ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y()); ///< 平滑滚动到目标分区
        });
    };

    connectButton(m_recommend_pushButton, this->m_recommendWidget.get());              ///< 连接推荐按钮
    connectButton(m_video_rank_pushButton, this->m_videoRankWidget.get());             ///< 连接视频榜按钮
    connectButton(m_MV_pushButton, this->m_MVWidget.get());                            ///< 连接 MV 按钮
    connectButton(m_site_pushButton, this->m_siteWidget.get());                        ///< 连接现场按钮
    connectButton(m_cover_pushButton, this->m_coverWidget.get());                      ///< 连接翻唱按钮
    connectButton(m_dance_pushButton, this->m_danceWidget.get());                      ///< 连接舞蹈按钮
    connectButton(m_children_pushButton, this->m_childrenWidget.get());                ///< 连接儿童按钮
    connectButton(m_live_pushButton, this->m_liveWidget.get());                        ///< 连接 TME LIVE 按钮
    connectButton(m_first_concert_pushButton, this->m_firstConcertWidget.get());       ///< 连接首唱会按钮
    connectButton(m_chinese_language_pushButton, this->m_chineseLanguageWidget.get()); ///< 连接华语按钮
    connectButton(m_South_Korea_pushButton, this->m_southKoreaWidget.get());           ///< 连接韩国按钮
    connectButton(m_Japan_pushButton, this->m_japanWidget.get());                      ///< 连接日本按钮
    connectButton(m_American_pushButton, this->m_americanWidget.get());                ///< 连接欧美按钮

    // 连接滚轮信号
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &VideoWidget::handleWheelValue); ///< 连接滚轮信号
    connect(vScrollBar, &QScrollBar::valueChanged, this, &VideoWidget::handleWheelValue);     ///< 连接滚动条值变化信号

    // 异步加载 JSON 文本
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this]
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/video.json")); ///< 加载 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading video.json";
            STREAM_WARN() << "Could not open file for reading video.json"; ///< 记录警告日志
            return true;
        }
        const auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr       = obj.array();
        for (const auto& item : arr)
        {
            auto obj = item.toObject();
            this->m_videoAuthorVector.emplace_back(obj.value(QStringLiteral("videoName")).toString(),
                                                   obj.value(QStringLiteral("author")).toString());
        }
        file.close();

        /// 初始化图片路径
        for (int i = 1; i <= 120; ++i)
        {
            this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i));
            ///< 添加封面图片
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机种子
        std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(),
                     std::default_random_engine(seed)); ///< 打乱图片列表
        std::shuffle(this->m_videoAuthorVector.begin(), this->m_videoAuthorVector.end(),
                     std::default_random_engine(seed));
        return true;
    });

    Async::onResultReady(future, this, [this](bool flag)
    {
        using Task = std::function<void()>;
        QVector<Task> tasks;

        // 1. 初始化推荐分区块（固定结构的双重循环）
        tasks << [this]
        {
            const QString tipText[] = {
                "", "音乐现场", "综艺制作形式", "舞蹈", "创意", "演奏", "舞蹈"
            };
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 1; j <= 3; ++j)
                {
                    int idx    = i * 3 + j;
                    auto block = new VideoBlockWidget(this);
                    block->setCoverPix(m_pixPathVector[idx]);
                    block->setShowTip();
                    block->setTipText(tipText[idx]);
                    block->setVideoName(m_videoAuthorVector[idx].first);
                    block->setAuthor(m_videoAuthorVector[idx].second);
                    block->setIconPix(m_pixPathVector[idx]);
                    m_recommendWidget->addBlockWidget(i, j - 1, block);
                }
            }
        };

        // 2. 添加每个分区的 loadSectionBlocks 调用
        tasks << [this] { loadSectionBlocks(m_videoRankWidget.get(), 1); };
        tasks << [this] { loadSectionBlocks(m_MVWidget.get(), 2); };
        tasks << [this] { loadSectionBlocks(m_siteWidget.get(), 3); };
        tasks << [this] { loadSectionBlocks(m_coverWidget.get(), 4); };
        tasks << [this] { loadSectionBlocks(m_danceWidget.get(), 5); };
        tasks << [this] { loadSectionBlocks(m_childrenWidget.get(), 6); };
        tasks << [this] { loadSectionBlocks(m_liveWidget.get(), 7); };
        tasks << [this] { loadSectionBlocks(m_firstConcertWidget.get(), 8); };
        tasks << [this] { loadSectionBlocks(m_chineseLanguageWidget.get(), 9); };
        tasks << [this] { loadSectionBlocks(m_southKoreaWidget.get(), 10); };
        tasks << [this] { loadSectionBlocks(m_japanWidget.get(), 11); };
        tasks << [this]
        {
            loadSectionBlocks(m_americanWidget.get(), 12);
            m_refreshMask->hideLoading(); // 所有加载完成后再隐藏遮罩
        };

        // 3. 串行执行器
        auto queue = std::make_shared<QQueue<Task>>();
        for (const auto& task : tasks)
            queue->enqueue(task);

        auto runner = std::make_shared<std::function<void()>>();
        *runner     = [queue, runner]()
        {
            if (queue->isEmpty())
                return;
            auto task = queue->dequeue();
            QTimer::singleShot(0, nullptr, [task, runner]()
            {
                task();
                (*runner)();
            });
        };

        (*runner)(); // 启动串行执行
    });


}

void VideoWidget::loadSectionBlocks(VideoPartWidget* section, const int idx)
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 1; j <= 3; ++j)
        {
            int index = i * 3 + j + idx * 6;
            if (index >= m_videoAuthorVector.size() || index >= m_pixPathVector.size())
                return;

            auto block = new VideoBlockWidget(this);
            block->setCoverPix(m_pixPathVector[index]);
            block->setVideoName(m_videoAuthorVector[index].first);
            block->setAuthor(m_videoAuthorVector[index].second);
            block->setIconPix(m_pixPathVector[index]);
            block->adjustSize();
            block->updateGeometry();
            section->addBlockWidget(i, j - 1, block);
            section->updateGeometry();
            section->update();
        }
    }
}

/**
 * @brief 处理滚轮值
 * @param value 滚动条值
 * @note 根据滚动位置切换按钮选中状态
 */
void VideoWidget::handleWheelValue(const int& value)
{
    const QVector<QPair<QWidget*, QPushButton*>> sectionMappings =
    {
        {m_recommendWidget.get(), m_recommend_pushButton},
        {m_videoRankWidget.get(), m_video_rank_pushButton},
        {m_MVWidget.get(), m_MV_pushButton},
        {m_siteWidget.get(), m_site_pushButton},
        {m_coverWidget.get(), m_cover_pushButton},
        {m_danceWidget.get(), m_dance_pushButton},
        {m_childrenWidget.get(), m_children_pushButton},
        {m_liveWidget.get(), m_live_pushButton},
        {m_firstConcertWidget.get(), m_first_concert_pushButton},
        {m_chineseLanguageWidget.get(), m_chinese_language_pushButton},
        {m_southKoreaWidget.get(), m_South_Korea_pushButton},
        {m_japanWidget.get(), m_Japan_pushButton},
        {m_americanWidget.get(), m_American_pushButton}
    };

    for (int i = 0; i < sectionMappings.size(); ++i)
    {
        QWidget* currentWidget = sectionMappings[i].first;
        QWidget* nextWidget    = (i + 1 < sectionMappings.size()) ? sectionMappings[i + 1].first : nullptr;

        int currentY = currentWidget->mapToParent(QPoint(0, 0)).y();
        int nextY    = nextWidget ? nextWidget->mapToParent(QPoint(0, 0)).y() : INT_MAX;

        if (value >= currentY && value < nextY)
        {
            sectionMappings[i].second->setChecked(true);
            break;
        }
    }
}

void VideoWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise(); // 确保遮罩在最上层
}

void VideoWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // qDebug() << "当前宽度：" << width();
    m_chinese_language_pushButton->setHidden(width() <= 880);
    m_South_Korea_pushButton->setHidden(width() <= 880);
    m_Japan_pushButton->setHidden(width() <= 880);
    m_American_pushButton->setHidden(width() <= 880);
}
