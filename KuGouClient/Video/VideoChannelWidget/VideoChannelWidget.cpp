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

#include <QFile>
#include <QButtonGroup>
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
    , m_popularWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_childrenWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_themeWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_filmWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_varietyWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_ACGNWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_sceneWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_languageWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_danceWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_siteWidget(std::make_unique<VideoChannelPartWidget>(this))
    , m_singerWidget(std::make_unique<VideoChannelPartWidget>(this))
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
    initVector();                                                 ///< 初始化图片路径
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
void VideoChannelWidget::initTotalWidget() const
{
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
    auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取垂直布局
    lay->setSpacing(0);                                           ///< 设置间距
    if (!lay)
    {
        qWarning() << "布局不存在";
        STREAM_WARN() << "布局不存在";
        return;
    }
    lay->insertWidget(lay->count() - 1, m_popularWidget.get());   ///< 插入热门部件
    lay->insertWidget(lay->count() - 1, m_childrenWidget.get());  ///< 插入儿童部件
    lay->insertWidget(lay->count() - 1, m_themeWidget.get());     ///< 插入主题部件
    lay->insertWidget(lay->count() - 1, m_filmWidget.get());      ///< 插入影视部件
    lay->insertWidget(lay->count() - 1, m_varietyWidget.get());   ///< 插入综艺部件
    lay->insertWidget(lay->count() - 1, m_ACGNWidget.get());      ///< 插入二次元部件
    lay->insertWidget(lay->count() - 1, m_sceneWidget.get());     ///< 插入场景部件
    lay->insertWidget(lay->count() - 1, m_languageWidget.get());  ///< 插入语言部件
    lay->insertWidget(lay->count() - 1, m_danceWidget.get());     ///< 插入舞蹈部件
    lay->insertWidget(lay->count() - 1, m_siteWidget.get());      ///< 插入现场部件
    lay->insertWidget(lay->count() - 1, m_singerWidget.get());    ///< 插入歌手部件

    this->m_vScrollBar = ui->scrollArea->verticalScrollBar();     ///< 获取垂直滚动条
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
    connect(this->m_vScrollBar, &QScrollBar::valueChanged, this, &VideoChannelWidget::handleWheelValue); ///< 连接滚动条信号

    int cur = 0;
    {
        //m_popularWidget 10 个
        const QString coverText[] = {
            "", "腾讯音乐盛典", "网络红歌", "每周推荐", "伤感网络情歌", "最热歌曲",
            "年度电影金曲", "武侠剧", "宫廷剧", "酷狗热歌", "偶像剧"
        };
        for (int i = 1; i <= 10; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i]);         ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_popularWidget->addBlockWidget(block);         ///< 添加到热门
        }
    }
    cur += 10;
    {
        //m_childrenWidget 14 个
        const QString coverText[] = {
            "", "宝宝巴士", "贝贝虎", "启蒙故事", "幼儿园", "动画片",
            "启蒙音乐剧", "幼儿唐诗", "迪士尼", "男孩最爱", "少儿才艺",
            "水果冰激凌", "歌声的翅膀", "女孩最爱", "麦杰克小镇"
        };
        for (int i = 1; i <= 14; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_childrenWidget->addBlockWidget(block);        ///< 添加到儿童
        }
    }
    cur += 14;
    {
        //m_themeWidget 10 个
        const QString coverText[] = {
            "", "网络红歌", "老情歌", "电音", "伤感网络情歌", "单曲循环音乐",
            "酷狗热歌", "新歌", "每周推荐", "音乐人", "燃爆歌曲"
        };
        for (int i = 1; i <= 10; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_themeWidget->addBlockWidget(block);           ///< 添加到主题
        }
    }
    cur += 10;
    {
        //m_filmWidget 7 个
        const QString coverText[] = {
            "", "年度电影金曲", "最热影视歌曲", "武侠剧", "宫廷剧", "偶像剧",
            "校园剧", "韩剧"
        };
        for (int i = 1; i <= 7; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_filmWidget->addBlockWidget(block);            ///< 添加到影视
        }
    }
    cur += 7;
    {
        //m_varietyWidget 1 个
        const QString coverText[] = {
            "", "炙热的我们"
        };
        for (int i = 1; i <= 1; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_varietyWidget->addBlockWidget(block);         ///< 添加到综艺
        }
    }
    cur += 1;
    {
        //m_ACGNWidget 6 个
        const QString coverText[] = {
            "", "国漫", "动漫", "王者荣耀", "古风ACG", "超福利宅舞",
            "游戏"
        };
        for (int i = 1; i <= 6; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_ACGNWidget->addBlockWidget(block);            ///< 添加到二次元
        }
    }
    cur += 6;
    {
        //m_sceneWidget 3 个
        const QString coverText[] = {
            "", "中文DJ", "派对", "清吧"
        };
        for (int i = 1; i <= 3; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_sceneWidget->addBlockWidget(block);           ///< 添加到场景
        }
    }
    cur += 3;
    {
        //m_languageWidget 9 个
        const QString coverText[] = {
            "", "怀旧国语", "怀旧粤语", "欧美", "国语", "粤语",
            "国语流行", "粤语流行", "闽南语", "日语"
        };
        for (int i = 1; i <= 9; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_languageWidget->addBlockWidget(block);        ///< 添加到语言
        }
    }
    cur += 9;
    {
        //m_danceWidget 3 个
        const QString coverText[] = {
            "", "广场舞", "舞蹈达人", "热舞"
        };
        for (int i = 1; i <= 3; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_danceWidget->addBlockWidget(block);           ///< 添加到舞蹈
        }
    }
    cur += 3;
    {
        //m_siteWidget 14 个
        const QString coverText[] = {
            "", "腾讯音乐庆典", "演唱会", "网路视听盛典", "国风经典", "韩国音乐现场",
            "央视网络春晚", "声家班", "欧美音乐现场", "韩国歌谣盛典", "蘑菇音乐节",
            "乐人现场", "翻唱现场", "Live现场", "摇滚现场"
        };
        for (int i = 1; i <= 14; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_siteWidget->addBlockWidget(block);            ///< 添加到现场
        }
    }
    cur += 14;
    {
        //m_singerWidget 26 个
        const QString coverText[] = {
            "", "周深", "薛之谦", "BLACKPINK", "TaylorSwift", "叶洛洛",
            "SM家族", "AKB48 GROUP", "TFBOYS", "凤凰传奇", "刘德华",
            "AlanWalker", "EXO", "艾薇儿", "SING女团", "少女时代",
            "鹿晗", "五月天", "防弹少年团", "BIGBANG", "邓紫棋",
            "洛天依", "庄心妍", "周杰伦", "陈奕迅", "张杰", "张国荣"
        };
        for (int i = 1; i <= 26; ++i)
        {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);   ///< 设置封面
            block->setCoverText(coverText[i]);                    ///< 设置文本
            this->m_singerWidget->addBlockWidget(block);          ///< 添加到歌手
        }
    }
}

/**
 * @brief 初始化图片路径容器
 */
void VideoChannelWidget::initVector()
{
    for (int i = 1; i <= 120; ++i)
    {
        this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i)); ///< 添加图片路径
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取随机种子
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed)); ///< 随机打乱
}

/**
 * @brief 处理滚动值变化
 * @param value 滚动条值
 */
void VideoChannelWidget::handleWheelValue(const int &value)
{
    if (value >= this->m_popularWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->popular_pushButton->setChecked(true);                 ///< 选中热门按钮
    }
    else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_themeWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->children_pushButton->setChecked(true);                ///< 选中儿童按钮
    }
    else if (value >= this->m_themeWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_filmWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->theme_pushButton->setChecked(true);                   ///< 选中主题按钮
    }
    else if (value >= this->m_filmWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_varietyWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->film_pushButton->setChecked(true);                    ///< 选中影视按钮
    }
    else if (value >= this->m_varietyWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_ACGNWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->variety_pushButton->setChecked(true);                 ///< 选中综艺按钮
    }
    else if (value >= this->m_ACGNWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_sceneWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->ACGN_pushButton->setChecked(true);                    ///< 选中二次元按钮
    }
    else if (value >= this->m_sceneWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_languageWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->scene_pushButton->setChecked(true);                   ///< 选中场景按钮
    }
    else if (value >= this->m_languageWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_danceWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->language_pushButton->setChecked(true);                ///< 选中语言按钮
    }
    else if (value >= this->m_danceWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_siteWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->dance_pushButton->setChecked(true);                   ///< 选中舞蹈按钮
    }
    else if (value >= this->m_siteWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_singerWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->site_pushButton->setChecked(true);                    ///< 选中现场按钮
    }
    else if (value >= this->m_singerWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->singer_pushButton->setChecked(true);                  ///< 选中歌手按钮
    }
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 * @note 重写基类方法
 */
void VideoChannelWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->window()->height() - 280); ///< 调整滚动区域高度
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void VideoChannelWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}