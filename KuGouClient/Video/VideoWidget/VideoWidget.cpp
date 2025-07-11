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

#include <QFile>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 滚动区域垂直偏移量 */
int offY = 297;

/**
 * @brief 构造函数，初始化视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))                ///< 初始化按钮组
    , m_recommendWidget(std::make_unique<VideoPartWidget>(this))         ///< 初始化推荐分区
    , m_videoRankWidget(std::make_unique<VideoPartWidget>(this))         ///< 初始化视频榜分区
    , m_MVWidget(std::make_unique<VideoPartWidget>(this))               ///< 初始化 MV 分区
    , m_siteWidget(std::make_unique<VideoPartWidget>(this))             ///< 初始化现场分区
    , m_coverWidget(std::make_unique<VideoPartWidget>(this))            ///< 初始化翻唱分区
    , m_danceWidget(std::make_unique<VideoPartWidget>(this))            ///< 初始化舞蹈分区
    , m_childrenWidget(std::make_unique<VideoPartWidget>(this))         ///< 初始化儿童分区
    , m_liveWidget(std::make_unique<VideoPartWidget>(this))             ///< 初始化 TME LIVE 分区
    , m_firstConcertWidget(std::make_unique<VideoPartWidget>(this))     ///< 初始化首唱会分区
    , m_chineseLanguageWidget(std::make_unique<VideoPartWidget>(this))  ///< 初始化华语分区
    , m_southKoreaWidget(std::make_unique<VideoPartWidget>(this))       ///< 初始化韩国分区
    , m_japanWidget(std::make_unique<VideoPartWidget>(this))            ///< 初始化日本分区
    , m_americanWidget(std::make_unique<VideoPartWidget>(this))         ///< 初始化欧美分区
{
    ui->setupUi(this);                                          ///< 设置 UI 布局
    QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());                    ///< 应用样式表
    }
    else
    {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";                 ///< 记录错误日志
        return;
    }
    initButtonGroup();                                          ///< 初始化按钮组
    initTotalWidget();                                          ///< 初始化视频分区
    initVector();                                               ///< 初始化图片路径
    initUi();                                                   ///< 初始化界面
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
VideoWidget::~VideoWidget()
{
    delete ui;                                                  ///< 释放 UI 界面
}

/**
 * @brief 初始化按钮组
 * @note 设置互斥按钮
 */
void VideoWidget::initButtonGroup() const
{
    this->m_buttonGroup->addButton(ui->recommend_pushButton);           ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->video_rank_pushButton);          ///< 添加视频榜按钮
    this->m_buttonGroup->addButton(ui->MV_pushButton);                  ///< 添加 MV 按钮
    this->m_buttonGroup->addButton(ui->site_pushButton);                ///< 添加现场按钮
    this->m_buttonGroup->addButton(ui->cover_pushButton);               ///< 添加翻唱按钮
    this->m_buttonGroup->addButton(ui->dance_pushButton);               ///< 添加舞蹈按钮
    this->m_buttonGroup->addButton(ui->children_pushButton);            ///< 添加儿童按钮
    this->m_buttonGroup->addButton(ui->live_pushButton);                ///< 添加 TME LIVE 按钮
    this->m_buttonGroup->addButton(ui->first_concert_pushButton);       ///< 添加首唱会按钮
    this->m_buttonGroup->addButton(ui->chinese_language_pushButton);    ///< 添加华语按钮
    this->m_buttonGroup->addButton(ui->South_Korea_pushButton);         ///< 添加韩国按钮
    this->m_buttonGroup->addButton(ui->Japan_pushButton);               ///< 添加日本按钮
    this->m_buttonGroup->addButton(ui->American_pushButton);            ///< 添加欧美按钮
    this->m_buttonGroup->setExclusive(true);                            ///< 设置互斥
}

/**
 * @brief 初始化视频分区
 * @note 设置 13 个分区的标题
 */
void VideoWidget::initTotalWidget() const
{
    this->m_recommendWidget->setTitleName("推荐");                     ///< 设置推荐标题
    this->m_videoRankWidget->setTitleName("视频榜");                   ///< 设置视频榜标题
    this->m_MVWidget->setTitleName("MV");                              ///< 设置 MV 标题
    this->m_siteWidget->setTitleName("现场");                          ///< 设置现场标题
    this->m_coverWidget->setTitleName("翻唱");                         ///< 设置翻唱标题
    this->m_danceWidget->setTitleName("舞蹈");                         ///< 设置舞蹈标题
    this->m_childrenWidget->setTitleName("儿童");                      ///< 设置儿童标题
    this->m_liveWidget->setTitleName("TME LIVE");                      ///< 设置 TME LIVE 标题
    this->m_firstConcertWidget->setTitleName("首唱会");                ///< 设置首唱会标题
    this->m_chineseLanguageWidget->setTitleName("华语");               ///< 设置华语标题
    this->m_southKoreaWidget->setTitleName("韩国");                    ///< 设置韩国标题
    this->m_japanWidget->setTitleName("日本");                         ///< 设置日本标题
    this->m_americanWidget->setTitleName("欧美");                      ///< 设置欧美标题
}

/**
 * @brief 初始化图片路径
 * @note 加载并打乱 120 张封面图片
 */
void VideoWidget::initVector()
{
    for (int i = 1; i <= 120; ++i)
    {
        this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i)); ///< 添加封面图片
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机种子
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed)); ///< 打乱图片列表
}

/**
 * @brief 初始化界面
 * @note 设置布局、信号和视频块
 */
void VideoWidget::initUi()
{
    // 加入布局
    auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取垂直布局
    lay->setSpacing(0);                                         ///< 设置间距
    lay->insertWidget(lay->count(), m_recommendWidget.get());       ///< 添加推荐分区
    lay->insertWidget(lay->count(), m_videoRankWidget.get());       ///< 添加视频榜分区
    lay->insertWidget(lay->count(), m_MVWidget.get());             ///< 添加 MV 分区
    lay->insertWidget(lay->count(), m_siteWidget.get());           ///< 添加现场分区
    lay->insertWidget(lay->count(), m_coverWidget.get());          ///< 添加翻唱分区
    lay->insertWidget(lay->count(), m_danceWidget.get());          ///< 添加舞蹈分区
    lay->insertWidget(lay->count(), m_childrenWidget.get());       ///< 添加儿童分区
    lay->insertWidget(lay->count(), m_liveWidget.get());           ///< 添加 TME LIVE 分区
    lay->insertWidget(lay->count(), m_firstConcertWidget.get());   ///< 添加首唱会分区
    lay->insertWidget(lay->count(), m_chineseLanguageWidget.get());///< 添加华语分区
    lay->insertWidget(lay->count(), m_southKoreaWidget.get());     ///< 添加韩国分区
    lay->insertWidget(lay->count(), m_japanWidget.get());          ///< 添加日本分区
    lay->insertWidget(lay->count(), m_americanWidget.get());       ///< 添加欧美分区

    // 处理信号
    this->m_vScrollBar = ui->scrollArea->verticalScrollBar();          ///< 获取垂直滚动条
    auto connectButton = [this](const QPushButton *button, QWidget *targetWidget) {
        connect(button, &QPushButton::clicked, this, [this, targetWidget] {
            ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y()); ///< 平滑滚动到目标分区
        });
    };
    connectButton(ui->recommend_pushButton, this->m_recommendWidget.get());           ///< 连接推荐按钮
    connectButton(ui->video_rank_pushButton, this->m_videoRankWidget.get());          ///< 连接视频榜按钮
    connectButton(ui->MV_pushButton, this->m_MVWidget.get());                        ///< 连接 MV 按钮
    connectButton(ui->site_pushButton, this->m_siteWidget.get());                    ///< 连接现场按钮
    connectButton(ui->cover_pushButton, this->m_coverWidget.get());                  ///< 连接翻唱按钮
    connectButton(ui->dance_pushButton, this->m_danceWidget.get());                  ///< 连接舞蹈按钮
    connectButton(ui->children_pushButton, this->m_childrenWidget.get());            ///< 连接儿童按钮
    connectButton(ui->live_pushButton, this->m_liveWidget.get());                    ///< 连接 TME LIVE 按钮
    connectButton(ui->first_concert_pushButton, this->m_firstConcertWidget.get());   ///< 连接首唱会按钮
    connectButton(ui->chinese_language_pushButton, this->m_chineseLanguageWidget.get()); ///< 连接华语按钮
    connectButton(ui->South_Korea_pushButton, this->m_southKoreaWidget.get());       ///< 连接韩国按钮
    connectButton(ui->Japan_pushButton, this->m_japanWidget.get());                  ///< 连接日本按钮
    connectButton(ui->American_pushButton, this->m_americanWidget.get());            ///< 连接欧美按钮

    // @note 未使用，保留用于调试
    /*
    connect(ui->recommend_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_recommendWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->video_rank_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_videoRankWidget->mapToParent(QPoint(0, 0)).y());
    });
        connect(ui->MV_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_MVWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->site_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_siteWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->cover_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_coverWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->dance_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_danceWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->children_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_childrenWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->live_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_liveWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->first_concert_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_firstConcertWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->chinese_language_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_chineseLanguageWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->South_Korea_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_southKoreaWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->Japan_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_japanWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->American_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_americanWidget->mapToParent(QPoint(0, 0)).y());
        });
    */

    // 连接滚轮信号
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &VideoWidget::handleWheelValue); ///< 连接滚轮信号
    connect(this->m_vScrollBar, &QScrollBar::valueChanged, this, &VideoWidget::handleWheelValue); ///< 连接滚动条值变化信号

    // 添加推荐分区视频块
    const QString tipText[] = {
        "", "音乐现场", "综艺制作形式", "舞蹈", "创意", "演奏", "舞蹈"
    };
    const QString videoName[] = {
        "", "把《恐龙抗狼》改得燃一点？", "连名带姓（live）", "花姐《一个多情又无情的人》DJ舞曲版",
        "民族舞", "二胡演奏 萨卡萨卡班班甲鱼鱼", "ROSE《APT》2024/11/29"
    };
    const QString author[] = {
        "", "武言圣", "刘天佑Tay", "网络潮流大时代", "往昔忆尽红", "雷天神", "你好我的女神"
    };
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 1; j <= 3; ++j)
        {
            auto block = new VideoBlockWidget(this);            ///< 创建视频块
            auto idx = i * 3 + j;
            block->setCoverPix(this->m_pixPathVector[idx]);      ///< 设置封面
            block->setShowTip();                               ///< 显示提示
            block->setTipText(tipText[idx]);                   ///< 设置提示文本
            block->setVideoName(videoName[idx]);               ///< 设置视频名称
            block->setAuthor(author[idx]);                     ///< 设置作者
            block->setIconPix(this->m_pixPathVector[idx]);     ///< 设置图标
            this->m_recommendWidget->addBlockWidget(i, j - 1, block); ///< 添加到推荐分区
        }
    }

    // 添加其他分区视频块
    auto cur = 6;
    {
        const QString videoName[] = {
            "", "永不言败", "无名的人", "toxic till the end",
            "茶花开了", "只字不提", "陪伴"
        };
        const QString author[] = {
            "", "周深", "张韶涵", "ROSE", "王睿卓", "于和伟", "陈慧娴"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_videoRankWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "APT.", "aespa 《Whiplash》MV", "Stray Kids \"Walkin On Water\" M/V",
            "战·永不言败", "谭维维MV上线", "\'DRIP\' MV"
        };
        const QString author[] = {
            "", "Bruno Mars", "aespa", "Stray Kids", "周深", "谭维维", "BABYMONSTER"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_MVWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "无效陪伴", "达拉崩吧", "壁上观",
            "11", "Strategy", "Warp Me In Plastic"
        };
        const QString author[] = {
            "", "汪苏泷", "周深", "张远", "队长", "TWICE", "7mm"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_siteWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "《土坡上的狗尾巴草》卢润泽", "我是奶龙，我才是奶龙！#每日一笑", "《谁》洋澜一",
            "《苹果香》安安", "离别开出花", "Empty Love"
        };
        const QString author[] = {
            "", "夏娃子", "夏娃子", "豆豆果", "豆豆果", "陆鳐LuLu", "陆鳐LuLu"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_coverWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "科目三(舞蹈)", "LIKE THAT", "DRIP",
            "梦", "Strategy", "Whiplash"
        };
        const QString author[] = {
            "", "蛋蛋", "BABYMONSTER", "BABYMONSTER", "时代少年团", "TWICE", "aespa"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_danceWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "Buster the Hero Fire Truck Saves the Day", "第二季45-船长爸爸", "Colors Rap",
            "生日快乐", "第三季01-堆肥", "奇迹无限"
        };
        const QString author[] = {
            "", "Little Baby Nursery Rhyme Friends", "Peppa Pig", "Little Baby Nursery Rhyme Friends",
            "贝瓦儿歌", "Peppa Pig", "何洛洛"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_childrenWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "《纯妹妹》单依纯-第五届腾讯音乐娱乐盛典", "男儿歌", "《璀璨冒险人》周深-第五届腾讯音乐娱乐盛典",
            "《小美满》-第五届腾讯音乐娱乐盛典", "醉", "第四届TMEA盛典 张杰《少年中国说》"
        };
        const QString author[] = {
            "", "单依纯", "时代少年团", "周深", "周深", "时代少年团", "张杰"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_liveWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "夏天的风", "暗里着迷（温柔女生版）", "孟婆的碗",
            "迟来的爱", "我是你的格桑花", "辞九门回忆"
        };
        const QString author[] = {
            "", "Uu（刘梦妤）", "童珺", "魏佳艺", "金润吉", "欣宝儿", "邓寓君(等什么君)"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_firstConcertWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "战·永不言败", "谭维维新歌MV上线", "姑娘别哭泣",
            "无效陪伴", "达拉崩吧", "壁上观"
        };
        const QString author[] = {
            "", "周深", "谭维维", "懒大王", "汪苏泷", "周深", "张远"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_chineseLanguageWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "APT.", "aespa 《Whiplash》MV", "Stray Kids \"Walkin On Water\" M/V",
            "\'DRIP\' MV", "TOMBOY", "Nxde"
        };
        const QString author[] = {
            "", "Bruno Mars", "aespa", "Stray Kids", "BABYMONSTER", "(G)I-DLE", "(G)I-DLE"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_southKoreaWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "不乱不破", "おべか", "责任集合体(无责任集合体)",
            "アイドル", "絶対敵対メチャキライャ-", "aespa《Hot Mess》MV"
        };
        const QString author[] = {
            "", "HOYO-MiX", "すりい", "マサラダ", "YOASOBI", "aespa", "Strategy"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_japanWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }
    cur += 6;
    {
        const QString videoName[] = {
            "", "Wrap Me In Plastic", "Uptown Funk", "ADDICT",
            "Come Home", "Smokin Out The Window", "Uptown Funk"
        };
        const QString author[] = {
            "", "7mm", "Mark Ronson", "Hazbin Hotel", "Thomas Day", "Bruno Mars", "Mark Ronson"
        };
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 1; j <= 3; ++j)
            {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_americanWidget->addBlockWidget(i, j - 1, block);
            }
        }
    }

    // 设置全部按钮
    ui->all_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF"))); ///< 设置悬停字体颜色
    ui->all_toolButton->setApproach(true);                    ///< 启用接近效果
    ui->all_toolButton->setIconSize(QSize(10, 10));           ///< 设置图标大小
    ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置默认图标
    ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置悬停图标
    ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置离开图标
    ui->title_widget2->hide();                                ///< 初始隐藏第二标题区域
}

/**
 * @brief 全部按钮切换槽函数
 * @note 切换标题区域显示并调整滚动高度
 */
void VideoWidget::on_all_toolButton_toggled()
{
    if (ui->all_toolButton->isChecked())
    {
        ui->all_toolButton->setText(QStringLiteral("收起"));   ///< 设置按钮文本
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg"))); ///< 设置向上图标
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg"))); ///< 设置悬停向上图标
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg"))); ///< 设置离开向上图标
        ui->title_widget2->show();                            ///< 显示第二标题区域
        offY = 335;                                           ///< 设置偏移量
    }
    else
    {
        ui->all_toolButton->setText(QStringLiteral("全部"));   ///< 设置按钮文本
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置向下图标
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置悬停向下图标
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置离开向下图标
        ui->title_widget2->hide();                            ///< 隐藏第二标题区域
        offY = 297;                                           ///< 设置偏移量
    }
    auto parent = this->window();                             ///< 获取顶级窗口
    ui->scrollArea->setFixedHeight(parent->height() - offY);  ///< 调整滚动区域高度
    update();                                                 ///< 触发重绘
}

/**
 * @brief 处理滚轮值
 * @param value 滚动条值
 * @note 根据滚动位置切换按钮选中状态
 */
void VideoWidget::handleWheelValue(const int &value)
{
    if (value >= this->m_recommendWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_videoRankWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->recommend_pushButton->setChecked(true);           ///< 选中推荐按钮
    }
    else if (value >= this->m_videoRankWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_MVWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->video_rank_pushButton->setChecked(true);          ///< 选中视频榜按钮
    }
    else if (value >= this->m_MVWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_siteWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->MV_pushButton->setChecked(true);                  ///< 选中 MV 按钮
    }
    else if (value >= this->m_siteWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_coverWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->site_pushButton->setChecked(true);               ///< 选中现场按钮
    }
    else if (value >= this->m_coverWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_danceWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->cover_pushButton->setChecked(true);              ///< 选中翻唱按钮
    }
    else if (value >= this->m_danceWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->dance_pushButton->setChecked(true);              ///< 选中舞蹈按钮
    }
    else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_liveWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->children_pushButton->setChecked(true);           ///< 选中儿童按钮
    }
    else if (value >= this->m_liveWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_firstConcertWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->live_pushButton->setChecked(true);               ///< 选中 TME LIVE 按钮
    }
    else if (value >= this->m_firstConcertWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_chineseLanguageWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->first_concert_pushButton->setChecked(true);      ///< 选中首唱会按钮
    }
    else if (value >= this->m_chineseLanguageWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_southKoreaWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->chinese_language_pushButton->setChecked(true);   ///< 选中华语按钮
    }
    else if (value >= this->m_southKoreaWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_japanWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->South_Korea_pushButton->setChecked(true);        ///< 选中韩国按钮
    }
    else if (value >= this->m_japanWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_americanWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->Japan_pushButton->setChecked(true);              ///< 选中日本按钮
    }
    else if (value >= this->m_americanWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->American_pushButton->setChecked(true);           ///< 选中欧美按钮
    }
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 默认实现
 */
void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);                    ///< 调用父类处理
}