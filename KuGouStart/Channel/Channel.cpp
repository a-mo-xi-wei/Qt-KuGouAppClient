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

#include <QFile>
#include <QButtonGroup>
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
    : QWidget(parent)
    , ui(new Ui::Channel)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_parent(this->window())
    , m_recommendWidget(std::make_unique<PartWidget>(this))
    , m_djWidget(std::make_unique<PartWidget>(this))
    , m_languageWidget(std::make_unique<PartWidget>(this))
    , m_themeWidget(std::make_unique<PartWidget>(this))
    , m_sceneWidget(std::make_unique<PartWidget>(this))
    , m_moodWidget(std::make_unique<PartWidget>(this))
    , m_styleWidget(std::make_unique<PartWidget>(this))
    , m_crowdWidget(std::make_unique<PartWidget>(this))
    , m_childrenWidget(std::make_unique<PartWidget>(this))
    , m_musicalInstrumentWidget(std::make_unique<PartWidget>(this))
    , m_labelWidget(std::make_unique<PartWidget>(this))
    , m_varietyWidget(std::make_unique<PartWidget>(this))
    , m_nationalCustomsWidget(std::make_unique<PartWidget>(this))
    , m_sportsWidget(std::make_unique<PartWidget>(this))
    , m_vScrollBar(nullptr)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channel.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
        ui->title_label->setStyleSheet(QString("border:none;border-image: url('%1');").arg(":/Res/window/music-channel.png")); ///< 设置标题图片
    }
    initButtonGroup();                                   ///< 初始化按钮组
    initTotalWidget();                                   ///< 初始化分区控件
    initVector();                                        ///< 初始化向量
    initUi();                                            ///< 初始化界面
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
    this->m_buttonGroup->addButton(ui->recommend_pushButton);   ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->DJ_pushButton);          ///< 添加 DJ 按钮
    this->m_buttonGroup->addButton(ui->language_pushButton);    ///< 添加语言按钮
    this->m_buttonGroup->addButton(ui->theme_pushButton);       ///< 添加主题按钮
    this->m_buttonGroup->addButton(ui->scene_pushButton);       ///< 添加场景按钮
    this->m_buttonGroup->addButton(ui->mood_pushButton);        ///< 添加心情按钮
    this->m_buttonGroup->addButton(ui->style_pushButton);       ///< 添加风格按钮
    this->m_buttonGroup->addButton(ui->crowd_pushButton);       ///< 添加人群按钮
    this->m_buttonGroup->addButton(ui->children_pushButton);    ///< 添加儿童按钮
    this->m_buttonGroup->addButton(ui->musical_instrument_pushButton); ///< 添加乐器按钮
    this->m_buttonGroup->addButton(ui->label_pushButton);       ///< 添加厂牌按钮
    this->m_buttonGroup->addButton(ui->variety_pushButton);     ///< 添加综艺按钮
    this->m_buttonGroup->addButton(ui->national_customs_pushButton); ///< 添加国风按钮
    this->m_buttonGroup->addButton(ui->sports_pushButton);      ///< 添加运动按钮
    this->m_buttonGroup->setExclusive(true);                    ///< 设置按钮组互斥
}

/**
 * @brief 初始化分区控件
 */
void Channel::initTotalWidget() const
{
    this->m_recommendWidget->setTitleName("推荐");       ///< 设置推荐分区标题
    this->m_djWidget->setTitleName("DJ");               ///< 设置 DJ 分区标题
    this->m_languageWidget->setTitleName("语言");        ///< 设置语言分区标题
    this->m_themeWidget->setTitleName("主题");           ///< 设置主题分区标题
    this->m_sceneWidget->setTitleName("场景");           ///< 设置场景分区标题
    this->m_moodWidget->setTitleName("心情");            ///< 设置心情分区标题
    this->m_styleWidget->setTitleName("风格");           ///< 设置风格分区标题
    this->m_crowdWidget->setTitleName("人群");           ///< 设置人群分区标题
    this->m_childrenWidget->setTitleName("儿童");        ///< 设置儿童分区标题
    this->m_musicalInstrumentWidget->setTitleName("乐器"); ///< 设置乐器分区标题
    this->m_labelWidget->setTitleName("厂牌");           ///< 设置厂牌分区标题
    this->m_varietyWidget->setTitleName("综艺");         ///< 设置综艺分区标题
    this->m_nationalCustomsWidget->setTitleName("国风"); ///< 设置国风分区标题
    this->m_sportsWidget->setTitleName("运动");          ///< 设置运动分区标题
}

/**
 * @brief 初始化界面
 * @note 添加分区控件到布局，连接按钮信号，优化滚动
 */
void Channel::initUi()
{
    {
        auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取表格布局
        if (!lay)
        {
            qWarning() << "布局不存在";
            STREAM_WARN() << "布局不存在";               ///< 记录警告日志
            return;
        }
        lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get()); ///< 添加推荐分区
        lay->insertWidget(lay->count() - 1, this->m_djWidget.get()); ///< 添加 DJ 分区
        lay->insertWidget(lay->count() - 1, this->m_languageWidget.get()); ///< 添加语言分区
        lay->insertWidget(lay->count() - 1, this->m_themeWidget.get()); ///< 添加主题分区
        lay->insertWidget(lay->count() - 1, this->m_sceneWidget.get()); ///< 添加场景分区
        lay->insertWidget(lay->count() - 1, this->m_moodWidget.get()); ///< 添加心情分区
        lay->insertWidget(lay->count() - 1, this->m_styleWidget.get()); ///< 添加风格分区
        lay->insertWidget(lay->count() - 1, this->m_crowdWidget.get()); ///< 添加人群分区
        lay->insertWidget(lay->count() - 1, this->m_childrenWidget.get()); ///< 添加儿童分区
        lay->insertWidget(lay->count() - 1, this->m_musicalInstrumentWidget.get()); ///< 添加乐器分区
        lay->insertWidget(lay->count() - 1, this->m_labelWidget.get()); ///< 添加厂牌分区
        lay->insertWidget(lay->count() - 1, this->m_varietyWidget.get()); ///< 添加综艺分区
        lay->insertWidget(lay->count() - 1, this->m_nationalCustomsWidget.get()); ///< 添加国风分区
        lay->insertWidget(lay->count() - 1, this->m_sportsWidget.get()); ///< 添加运动分区
    }
    {
        this->m_vScrollBar = ui->scrollArea->verticalScrollBar(); ///< 获取垂直滚动条
        auto connectButton = [this](const QPushButton *button, QWidget *targetWidget) {
            connect(button, &QPushButton::clicked, this, [this, targetWidget] {
                ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y()); ///< 平滑滚动到目标位置
            });
        };                                               ///< 批量连接按钮信号
        connectButton(ui->recommend_pushButton, m_recommendWidget.get()); ///< 连接推荐按钮
        connectButton(ui->DJ_pushButton, m_djWidget.get()); ///< 连接 DJ 按钮
        connectButton(ui->language_pushButton, m_languageWidget.get()); ///< 连接语言按钮
        connectButton(ui->theme_pushButton, m_themeWidget.get()); ///< 连接主题按钮
        connectButton(ui->scene_pushButton, m_sceneWidget.get()); ///< 连接场景按钮
        connectButton(ui->mood_pushButton, m_moodWidget.get()); ///< 连接心情按钮
        connectButton(ui->style_pushButton, m_styleWidget.get()); ///< 连接风格按钮
        connectButton(ui->crowd_pushButton, m_crowdWidget.get()); ///< 连接人群按钮
        connectButton(ui->children_pushButton, m_childrenWidget.get()); ///< 连接儿童按钮
        connectButton(ui->musical_instrument_pushButton, m_musicalInstrumentWidget.get()); ///< 连接乐器按钮
        connectButton(ui->label_pushButton, m_labelWidget.get()); ///< 连接厂牌按钮
        connectButton(ui->variety_pushButton, m_varietyWidget.get()); ///< 连接综艺按钮
        connectButton(ui->national_customs_pushButton, m_nationalCustomsWidget.get()); ///< 连接国风按钮
        connectButton(ui->sports_pushButton, m_sportsWidget.get()); ///< 连接运动按钮
        connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Channel::handleWheelValue); ///< 连接滚动区域信号
        connect(this->m_vScrollBar, &QScrollBar::valueChanged, this, &Channel::handleWheelValue); ///< 连接滚动条信号
    }
    auto cur = 0;                                        ///< 音乐块索引
    {
        const QString title[] = {
            "", "国语", "聚会暖场", "抖音热门歌", "中文经典", "中文DJ", "怀旧粤语", "KTV必点曲",
            "90后", "抖音最火DJ", "老情歌", "网络红歌", "最爱成名曲", "车载舞曲", "那年MP3里的歌",
            "百听不厌英文歌", "伤感网络情歌", "轻音乐"
        };                                               ///< 推荐分区标题
        for (int i = 1; i <= 17; ++i)
        {
            auto block = new ChannelBlock(this);          ///< 创建音乐块
            block->setCoverPix(this->m_pixPathVector[i]); ///< 设置封面
            block->setTitleText(title[i]);               ///< 设置标题
            block->setSingerSongText(this->m_songAndsinger[i].first + " - " + this->m_songAndsinger[i].second); ///< 设置歌手歌曲
            this->m_recommendWidget->addBlockWidget(block); ///< 添加到推荐分区
        }
    }
    cur += 17;                                           ///< 更新索引
    {
        const QString title[] = {
            "", "中文DJ", "抖音最火DJ", "情歌DJ", "车载舞曲", "重低音", "EDM热歌",
            "串烧舞曲", "电音", "电子纯音", "慢摇舞曲", "复古迪士高", "外文舞曲", "House舞曲", "超嗨舞曲"
        };                                               ///< DJ 分区标题
        for (int i = 1; i <= 14; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_djWidget->addBlockWidget(block);
        }
    }
    cur += 14;
    {
        const QString title[] = {
            "", "百听不厌英文歌", "怀旧粤语", "粤语", "欧美", "怀旧华语", "闽南语",
            "韩语", "国语", "日语", "法语", "泰语", "印度语", "俄语", "德语", "意大利语",
            "客家语", "西班牙语"
        };                                               ///< 语言分区标题
        for (int i = 1; i <= 17; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_languageWidget->addBlockWidget(block);
        }
    }
    cur += 17;
    {
        const QString title[] = {
            "", "0.8x慢速", "KTV必点曲", "网络红歌", "中文经典", "伤感网络情歌",
            "最爱成名曲", "酷狗热歌", "那年MP3里的歌", "店铺", "老情歌", "热评10W+歌曲",
            "世界级热门单曲", "情歌对唱", "酷狗新歌", "由你音乐榜", "国创ACG", "神曲",
            "经典影视原声", "最热影视歌曲", "发烧女声", "励志", "日本ACG", "蝰蛇全景声",
            "圣诞必听", "中国新乡村音乐", "热门动漫", "抖音热歌榜", "二次元精选"
        };                                               ///< 主题分区标题
        for (int i = 1; i <= 28; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_themeWidget->addBlockWidget(block);
        }
    }
    cur += 28;
    {
        const QString title[] = {
            "", "广场舞", "咖啡厅", "一个人", "工作加油曲", "睡前",
            "学习", "散步", "清吧", "在路上", "驾驶", "打游戏", "聚会暖场", "婚礼", "午休",
            "起床", "蹦迪", "自然", "冥想"
        };                                               ///< 场景分区标题
        for (int i = 1; i <= 18; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_sceneWidget->addBlockWidget(block);
        }
    }
    cur += 18;
    {
        const QString title[] = {"", "安静", "轻松", "伤感", "寂寞", "甜蜜", "兴奋", "思念", "快乐"}; ///< 心情分区标题
        for (int i = 1; i <= 8; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_moodWidget->addBlockWidget(block);
        }
    }
    cur += 8;
    {
        const QString title[] = {
            "", "轻音乐", "草原风", "流行", "民歌", "乡村音乐",
            "R&B", "摇滚", "爵士", "蒸汽波", "民谣", "说唱", "古典音乐", "布鲁斯", "金属"
        };                                               ///< 风格分区标题
        for (int i = 1; i <= 14; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_styleWidget->addBlockWidget(block);
        }
    }
    cur += 14;
    {
        const QString title[] = {"", "90后", "80后", "70后", "00后"}; ///< 人群分区标题
        for (int i = 1; i <= 4; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_crowdWidget->addBlockWidget(block);
        }
    }
    cur += 4;
    {
        const QString title[] = {
            "", "儿童故事", "0-1岁儿歌", "1-3岁儿歌",
            "3-6岁儿歌", "7-12岁儿歌", "英文儿歌", "胎教", "教育科普", "中文儿歌",
            "睡眠故事", "国学启蒙", "动画儿歌"
        };                                               ///< 儿童分区标题
        for (int i = 1; i <= 12; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_childrenWidget->addBlockWidget(block);
        }
    }
    cur += 12;
    {
        const QString title[] = {
            "", "钢琴", "古筝", "萨克斯", "八音盒", "吉他", "尤克里里",
            "葫芦丝", "二胡", "笛子", "小提琴", "唢呐"
        };                                               ///< 乐器分区标题
        for (int i = 1; i <= 11; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_musicalInstrumentWidget->addBlockWidget(block);
        }
    }
    cur += 11;
    {
        const QString title[] = {
            "", "滚石唱片推荐", "华纳唱片", "JYP", "SACRA MUSIC",
            "Liquid State", "摩登天空"
        };                                               ///< 厂牌分区标题
        for (int i = 1; i <= 6; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_labelWidget->addBlockWidget(block);
        }
    }
    cur += 6;
    {
        const QString title[] = {
            "", "声生不息-宝岛季", "来看我们的演唱会", "声生不息-港乐季",
            "明日之子乐团季", "乘风破浪的姐姐", "新声请指教", "中国梦之声·我们的歌", "音浪合伙人",
            "明日之子", "创造营2024", "这！就是原创", "中国新说唱", "创造101", "歌手",
            "经典咏流传", "天赐的声音第三季", "跨次元新星", "我们的歌第二季", "中国好声音", "我是歌手",
            "厉害了!我的歌", "中国好歌曲", "盖世英雄", "乐队的夏天", "诗画中国", "天赐的声音第二季", "蒙面唱将第五季"
        };                                               ///< 综艺分区标题
        for (int i = 1; i <= 27; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_varietyWidget->addBlockWidget(block);
        }
    }
    cur += 27;
    {
        const QString title[] = {"", "中国风精选", "古风好歌", "伤感国风", "国风新歌", "国风经典", "热血国风"}; ///< 国风分区标题
        for (int i = 1; i <= 6; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_nationalCustomsWidget->addBlockWidget(block);
        }
    }
    cur += 6;
    {
        const QString title[] = {"", "健身房", "跑步", "动感单车", "热身", "HIIT", "力量训练", "瑜伽"}; ///< 运动分区标题
        for (int i = 1; i <= 7; ++i)
        {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setTitleText(title[i]);
            block->setSingerSongText(this->m_songAndsinger[i + cur].first + " - " + this->m_songAndsinger[i + cur].second);
            this->m_sportsWidget->addBlockWidget(block);
        }
    }
}

/**
 * @brief 初始化歌曲-歌手和封面图片向量
 * @note 使用随机种子打乱顺序
 */
void Channel::initVector()
{
    {
        this->m_songAndsinger.emplace_back("租购", "薛之谦"); ///< 添加歌曲-歌手配对
        this->m_songAndsinger.emplace_back("还会再相遇", "时代少年团");
        this->m_songAndsinger.emplace_back("篇章(郜一菲小号合作曲)", "王赫野");
        this->m_songAndsinger.emplace_back("先说爱的人为什么先离开", "田园");
        this->m_songAndsinger.emplace_back("勇气之歌(《海洋奇缘2》中文主题曲)", "张杰");
        this->m_songAndsinger.emplace_back("小太阳", "时代少年团");
        this->m_songAndsinger.emplace_back("山友顶峰湖有彼岸", "魏佳艺");
        this->m_songAndsinger.emplace_back("朝夕相伴两不厌", "门丽");
        this->m_songAndsinger.emplace_back("冬2025", "王靖雯");
        this->m_songAndsinger.emplace_back("繁星从远山升起", "怪阿姨、吴瑭");
        this->m_songAndsinger.emplace_back("我们的时光 + 彩虹下面(live版)", "赵雷");
        this->m_songAndsinger.emplace_back("别等啦，出发吧", "张碧晨");
        this->m_songAndsinger.emplace_back("一分钱难倒英雄汉", "王小叶");
        this->m_songAndsinger.emplace_back("久别离", "七叔 (叶泽浩)");
        this->m_songAndsinger.emplace_back("我是夜里的光", "郑畅业");
        this->m_songAndsinger.emplace_back("极度伤感", "马健涛");
        this->m_songAndsinger.emplace_back("再见白马", "杨小壮");
        this->m_songAndsinger.emplace_back("秋叶落尽大雪飞(烟嗓版)", "韩小欠");
        this->m_songAndsinger.emplace_back("心头刺", "小阿七");
        this->m_songAndsinger.emplace_back("人间数十载", "韩小欠");
        this->m_songAndsinger.emplace_back("12号", "周柏豪");
        this->m_songAndsinger.emplace_back("你在的城市下了雪吗", "苍狼");
        this->m_songAndsinger.emplace_back("我不能", "六哲");
        this->m_songAndsinger.emplace_back("风中听落雪", "小阿枫");
        this->m_songAndsinger.emplace_back("野草没有花期", "潘成 (皮卡潘)");
        this->m_songAndsinger.emplace_back("风雨中的遗憾(粤语金曲)", "阿国歌");
        this->m_songAndsinger.emplace_back("如何面对这份情", "红蔷薇");
        this->m_songAndsinger.emplace_back("花香染指尖", "蔷薇团长、笑天");
        this->m_songAndsinger.emplace_back("临安月", "冯提莫");
        this->m_songAndsinger.emplace_back("分手的意义", "贺一航");
        this->m_songAndsinger.emplace_back("你我这也不能称为我们", "大雨点儿");
        this->m_songAndsinger.emplace_back("我的人生只能自己升级", "肖雨蒙");
        this->m_songAndsinger.emplace_back("女人的妆男人的谎", "李乐乐");
        this->m_songAndsinger.emplace_back("乌兰巴托的夜晚", "石头");
        this->m_songAndsinger.emplace_back("赤裸裸的伤害", "张艺迈");
        this->m_songAndsinger.emplace_back("戏", "丫蛋");
        this->m_songAndsinger.emplace_back("心中有座城叫愁", "影子");
        this->m_songAndsinger.emplace_back("此夜定有鬼", "一只白羊");
        this->m_songAndsinger.emplace_back("又是一阵秋风过", "谈柒柒");
        this->m_songAndsinger.emplace_back("雪的来意", "于洋");
        this->m_songAndsinger.emplace_back("横滨别恋", "周慧敏");
        this->m_songAndsinger.emplace_back("我快撑不住了", "范茹");
        this->m_songAndsinger.emplace_back("TOO HARD", "米卡");
        this->m_songAndsinger.emplace_back("以后的路陪你走", "清唯");
        this->m_songAndsinger.emplace_back("恰少年", "曹怡");
        this->m_songAndsinger.emplace_back("欠你一个天下", "梦珂");
        this->m_songAndsinger.emplace_back("我若消失谁会思念", "乔玲儿");
        this->m_songAndsinger.emplace_back("你的(live)", "汪苏泷、贺仙人");
        this->m_songAndsinger.emplace_back("土坡上的狗尾草(双语版)", "卢润泽、宫八");
        this->m_songAndsinger.emplace_back("凝眸", "丁禹兮");
        this->m_songAndsinger.emplace_back("心甘情愿做你一生的知己", "基地小虎");
        this->m_songAndsinger.emplace_back("寄明月", "虞书欣、丁禹兮、祝绪丹");
        this->m_songAndsinger.emplace_back("都一样", "肖战");
        this->m_songAndsinger.emplace_back("忘了归期", "王忻辰、袁小葳");
        this->m_songAndsinger.emplace_back("踹", "尼古拉斯.四哥");
        this->m_songAndsinger.emplace_back("7月7日晴", "en");
        this->m_songAndsinger.emplace_back("白鸽乌鸦相爱的戏码", "艾辰");
        this->m_songAndsinger.emplace_back("重塑", "都智文");
        this->m_songAndsinger.emplace_back("带我到山顶(Live)", "姚晓棠、海来阿木");
        this->m_songAndsinger.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_songAndsinger.emplace_back("依然在一起", "马天宇");
        this->m_songAndsinger.emplace_back("你之于我", "丁禹兮");
        this->m_songAndsinger.emplace_back("重写宇宙", "世界之外");
        this->m_songAndsinger.emplace_back("葡萄成熟时", "陈奕迅");
        this->m_songAndsinger.emplace_back("后来遇见他", "善宇");
        this->m_songAndsinger.emplace_back("一叶知心", "巫娜");
        this->m_songAndsinger.emplace_back("好好生活", "旭言");
        this->m_songAndsinger.emplace_back("28TH", "HVOB");
        this->m_songAndsinger.emplace_back("天荒爱未老", "周慧敏");
        this->m_songAndsinger.emplace_back("一生不变", "邓瑞霞");
        this->m_songAndsinger.emplace_back("一暝", "杨静");
        this->m_songAndsinger.emplace_back("客家情怀", "谢忠良");
        this->m_songAndsinger.emplace_back("悬溺", "葛东琪");
        this->m_songAndsinger.emplace_back("还原", "肖战");
        this->m_songAndsinger.emplace_back("有一种爱叫做放手", "阿木");
        this->m_songAndsinger.emplace_back("炽火同行", "曹杨");
        this->m_songAndsinger.emplace_back("暖人", "张磊");
        this->m_songAndsinger.emplace_back("今生的爱前世的债", "相思");
        this->m_songAndsinger.emplace_back("水中花(Live)", "郁可唯");
        this->m_songAndsinger.emplace_back("主视角", "陈思斯");
        this->m_songAndsinger.emplace_back("声声慢(粤语版)", "陈艺鹏");
        this->m_songAndsinger.emplace_back("清静经", "王菲");
        this->m_songAndsinger.emplace_back("Together", "王铮亮");
        this->m_songAndsinger.emplace_back("夏夜心事", "Audrey");
        this->m_songAndsinger.emplace_back("静听流水声", "明月闲人");
        this->m_songAndsinger.emplace_back("桂花香", "七朵组合");
        this->m_songAndsinger.emplace_back("那个女孩", "宝石Gem");
        this->m_songAndsinger.emplace_back("她们唱的爱情", "曾昭玮");
        this->m_songAndsinger.emplace_back("天气预爆", "筷子兄弟");
        this->m_songAndsinger.emplace_back("碎银几两", "轩东");
        this->m_songAndsinger.emplace_back("花轿里面的人", "巨敏");
        this->m_songAndsinger.emplace_back("不可道", "祖娅纳惜");
        this->m_songAndsinger.emplace_back("渴望", "毛阿敏");
        this->m_songAndsinger.emplace_back("冬天里的一把火", "费翔");
        this->m_songAndsinger.emplace_back("说散就散(Live)", "杨耀扬");
        this->m_songAndsinger.emplace_back("如果有来生(Live)", "谭维维");
        this->m_songAndsinger.emplace_back("时光谣", "蔡健雅");
        this->m_songAndsinger.emplace_back("好风(Live)", "布衣乐队");
        this->m_songAndsinger.emplace_back("纸上雪", "许嵩");
        this->m_songAndsinger.emplace_back("红山果", "安与骑兵");
        this->m_songAndsinger.emplace_back("不羁恋人", "陈慧娴");
        this->m_songAndsinger.emplace_back("懂你", "满文军");
        this->m_songAndsinger.emplace_back("落日归山河", "张颜烁");
        this->m_songAndsinger.emplace_back("想起他们", "毛不易");
        this->m_songAndsinger.emplace_back("想念你想我", "Eric周兴哲");
        this->m_songAndsinger.emplace_back("運命ちゃん", "いきものがかり");
        this->m_songAndsinger.emplace_back("会いたい", "いきものがかり");
        this->m_songAndsinger.emplace_back("Under the Tree", "Ed Sheeran");
        this->m_songAndsinger.emplace_back("봄처럼 다가와", "송하영、이나경");
        this->m_songAndsinger.emplace_back("Contigo", "Marta Santos");
        this->m_songAndsinger.emplace_back("Ends of the Earth", "Ty Myers");
        this->m_songAndsinger.emplace_back("Dicen", "Alvaro Soler");
        this->m_songAndsinger.emplace_back("TWS (투어스) '마지막 축제' Official", "TWS");
        this->m_songAndsinger.emplace_back("IZNA", "izna");
        this->m_songAndsinger.emplace_back("情人", "LUMi");
        this->m_songAndsinger.emplace_back("마이트로(MYTRO)", "Mytro");
        this->m_songAndsinger.emplace_back("Work Hard Play Hard", "YONA YONA WEEKENDERS");
        this->m_songAndsinger.emplace_back("我想要拥有你", "小男孩乐团");
        this->m_songAndsinger.emplace_back("number one girl", "ROSÉ");
        this->m_songAndsinger.emplace_back("SHE THING（心生）", "戴萌");
        this->m_songAndsinger.emplace_back("Told You So", "Martin Garrix、Jex");
        this->m_songAndsinger.emplace_back("漂流", "肖战");
        this->m_songAndsinger.emplace_back("别Gender", "曾轶可");
        this->m_songAndsinger.emplace_back("SHUSH", "ENONE");
        this->m_songAndsinger.emplace_back("梦的烛衣", "张碧晨");
        this->m_songAndsinger.emplace_back("Nothing", "薛之谦");
        this->m_songAndsinger.emplace_back("Gang Gang Love", "弹壳");
        this->m_songAndsinger.emplace_back("青年", "刘循子墨、张本煜");
        this->m_songAndsinger.emplace_back("月夜狂想曲", "时代少年团");
        this->m_songAndsinger.emplace_back("天空之外Above The Rooftop", "曾轶可");
        this->m_songAndsinger.emplace_back("不重要", "苏醒AllenSu");
        this->m_songAndsinger.emplace_back("喚醒我", "万妮达Vinida Weng");
        this->m_songAndsinger.emplace_back("七号种子", "万妮达Vinida Weng");
        this->m_songAndsinger.emplace_back("你的温柔Your Tenderness", "曾轶可");
        this->m_songAndsinger.emplace_back("TOXIC", "MEOVV");
        this->m_songAndsinger.emplace_back("Side U (Prod. AmPm)", "茜屋日海夏");
        this->m_songAndsinger.emplace_back("Dystopia", "ONE OK ROCK");
        this->m_songAndsinger.emplace_back("NALLINA", "NEXZ");
        this->m_songAndsinger.emplace_back("알고싶어요 (I want to know )", "김수영");
        this->m_songAndsinger.emplace_back("ミチシルベ", "IMP.");
        this->m_songAndsinger.emplace_back("고칠게(I'm gonna change)", "Woody");
        this->m_songAndsinger.emplace_back("멈추지 않아", "PLAVE");
        this->m_songAndsinger.emplace_back("Latawce", "Sylwia Grzeszczak");
        this->m_songAndsinger.emplace_back("Mine", "AUDREY NUNA");
        this->m_songAndsinger.emplace_back("Your Christmas", "Tokio Hotel");
        this->m_songAndsinger.emplace_back("Não Vou Temer", "Rich Lima");
        this->m_songAndsinger.emplace_back("SHEITE", "Nicki Nicole");
        this->m_songAndsinger.emplace_back("Tragen Engel Schwerter", "Nicki Papa");
        this->m_songAndsinger.emplace_back("The Karate Kid", "Coldplay");
        this->m_songAndsinger.emplace_back("2 hands", "Tate McRae");
        this->m_songAndsinger.emplace_back("SINNER", "Zauntee");
        this->m_songAndsinger.emplace_back("Gelada", "Atitude 67、Mar.iana");
        this->m_songAndsinger.emplace_back("Be Careful", "Maiah");
        this->m_songAndsinger.emplace_back("Lover Boy", "Diamond Cafe");
        this->m_songAndsinger.emplace_back("Breathe", "ZOĒ");
        this->m_songAndsinger.emplace_back("more than a minute", "Macey");
        this->m_songAndsinger.emplace_back("Halfway To Heaven", "Midland");
        this->m_songAndsinger.emplace_back("Blah Blah Blah", "Mikay");
        this->m_songAndsinger.emplace_back("Metamorfosis", "Reyno");
        this->m_songAndsinger.emplace_back("Two Faced", "Linkin Park");
        this->m_songAndsinger.emplace_back("Tout va bien", "Dalí");
        this->m_songAndsinger.emplace_back("Daddy Raised", "Jackson Dean");
        this->m_songAndsinger.emplace_back("HYÄNE", "NGEE");
        this->m_songAndsinger.emplace_back("This Christmas", "Alive Gospel、Janice");
        this->m_songAndsinger.emplace_back("Was muss, das muss", "Mr. Hurley & Die Pulveraffen、Dämmerland");
        this->m_songAndsinger.emplace_back("AMARSI PER LAVORO", "Mazzariello");
        this->m_songAndsinger.emplace_back("Be Alright", "Joel Corry");
        this->m_songAndsinger.emplace_back("už i andělé šli spát", "Mirai");
        this->m_songAndsinger.emplace_back("Rode Draad", "Hannah Mae");
        this->m_songAndsinger.emplace_back("FEARLESS", "Twocolors、TC/TC");
        this->m_songAndsinger.emplace_back("love myself", "Lindsay Ell");
        this->m_songAndsinger.emplace_back("Snow Waltz", "Inger Lise Hope");
        this->m_songAndsinger.emplace_back("Bıraktım Yansınlar", "Uğur Bozdağ");
        this->m_songAndsinger.emplace_back("Confiance", "Oussama");
        this->m_songAndsinger.emplace_back("HO SBAGLIATO ANCORA", "chiello");
        this->m_songAndsinger.emplace_back("En lo oscurito", "Conep、Ciara Lauve");
        this->m_songAndsinger.emplace_back("Running Wild", "Jin");
        this->m_songAndsinger.emplace_back("나였으면(If It Were Me)", "HYNN");
        this->m_songAndsinger.emplace_back("빛 (Light)", "申容财");
        this->m_songAndsinger.emplace_back("아이 (The Child)", "미미크리엑스");
        this->m_songAndsinger.emplace_back("両片想い (两相思)", "柴田淳");
        this->m_songAndsinger.emplace_back("미워하다, 그리워하고(Hate and Miss you)", "柳会胜");
        this->m_songAndsinger.emplace_back("우리라는 이야기 (The story of us)", "이젤");
        this->m_songAndsinger.emplace_back("HOWLING", "XG");
        this->m_songAndsinger.emplace_back("Fighter", "HYNN、nochexintoma");
        this->m_songAndsinger.emplace_back("景色 (KESHIKI)", "EVNNE");
        this->m_songAndsinger.emplace_back("Identity", "MISAMO");
        this->m_songAndsinger.emplace_back("潜规则", "郑智化");
        this->m_songAndsinger.emplace_back("不要回答", "檀健次、朱哲琴");
        this->m_songAndsinger.emplace_back("OVERLOAD", "邹沛沛、邢益豪");
        this->m_songAndsinger.emplace_back("心悸宝贝 BIUBIU", "babyMINT");
        this->m_songAndsinger.emplace_back("紫蝴蝶", "邹沛沛");
        this->m_songAndsinger.emplace_back("从未见过的海", "告五人");
        this->m_songAndsinger.emplace_back("目标是宝可梦大师", "腾格尔");
        this->m_songAndsinger.emplace_back("忘", "张叶蕾");
        this->m_songAndsinger.emplace_back("次次", "甄济如");
        this->m_songAndsinger.emplace_back("我好想你", "苏打绿");
        this->m_songAndsinger.emplace_back("最好的朋友", "SNH48");
        this->m_songAndsinger.emplace_back("伴你", "黄绮珊");
        this->m_songAndsinger.emplace_back("炎之舞", "郑智化");
    }
    for (int i = 1; i <= 210; ++i)
    {
        this->m_pixPathVector.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i)); ///< 添加封面图片路径
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取当前时间作为随机种子
    std::shuffle(this->m_songAndsinger.begin(), this->m_songAndsinger.end(), std::default_random_engine(seed)); ///< 随机打乱歌曲-歌手
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed)); ///< 随机打乱封面图片
}

/**
 * @brief 处理滚动条值变化
 * @param value 滚动条值
 */
void Channel::handleWheelValue(const int &value)
{
    if (value >= this->m_recommendWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_djWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->recommend_pushButton->setChecked(true);      ///< 选中推荐按钮
    }
    else if (value >= this->m_djWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_languageWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->DJ_pushButton->setChecked(true);             ///< 选中 DJ 按钮
    }
    else if (value >= this->m_languageWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_themeWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->language_pushButton->setChecked(true);       ///< 选中语言按钮
    }
    else if (value >= this->m_themeWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_sceneWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->theme_pushButton->setChecked(true);          ///< 选中主题按钮
    }
    else if (value >= this->m_sceneWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_moodWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->scene_pushButton->setChecked(true);          ///< 选中场景按钮
    }
    else if (value >= this->m_moodWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_styleWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->mood_pushButton->setChecked(true);           ///< 选中心情按钮
    }
    else if (value >= this->m_styleWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->style_pushButton->setChecked(true);          ///< 选中风格按钮
    }
    else if (value >= this->m_crowdWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->crowd_pushButton->setChecked(true);          ///< 选中人群按钮
    }
    else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_musicalInstrumentWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->children_pushButton->setChecked(true);       ///< 选中儿童按钮
    }
    else if (value >= this->m_musicalInstrumentWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_labelWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->musical_instrument_pushButton->setChecked(true); ///< 选中乐器按钮
    }
    else if (value >= this->m_labelWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_varietyWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->label_pushButton->setChecked(true);          ///< 选中厂牌按钮
    }
    else if (value >= this->m_varietyWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_nationalCustomsWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->variety_pushButton->setChecked(true);        ///< 选中综艺按钮
    }
    else if (value >= this->m_nationalCustomsWidget->mapToParent(QPoint(0, 0)).y() &&
             value < this->m_sportsWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->national_customs_pushButton->setChecked(true); ///< 选中国风按钮
    }
    else if (value >= this->m_sportsWidget->mapToParent(QPoint(0, 0)).y())
    {
        ui->sports_pushButton->setChecked(true);         ///< 选中运动按钮
    }
}

/**
 * @brief 调整大小事件，调整滚动区域高度
 * @param event 调整大小事件
 */
void Channel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类调整大小事件
    ui->scrollArea->setFixedHeight(this->m_parent->height() - 305); ///< 设置滚动区域高度
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 */
void Channel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);                     ///< 调用父类鼠标按下事件
}