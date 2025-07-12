/**
 * @file ListenOptionMenu.cpp
 * @brief 实现 ListenOptionMenu 类，提供听单选项菜单功能
 * @author WeiWang
 * @date 2025-02-03
 * @version 1.0
 */

#include "ListenOptionMenu.h"
#include "logger.hpp"
#include "MyScrollArea.h"

#include <QFile>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>

/** @brief 获取当前文件目录的宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化听单选项菜单
 * @param parent 父控件指针，默认为 nullptr
 */
ListenOptionMenu::ListenOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
    // 加载样式表
    QFile file(GET_CURRENT_DIR + QStringLiteral("/listenmenu.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
}

/**
 * @brief 初始化菜单布局和内容
 */
void ListenOptionMenu::initMenu() {
    this->setFixedSize(740, 370);
    this->setMouseTracking(true);

    // 滚动区域设置
    auto area = new MyScrollArea(this);
    area->move(15, 20);
    area->setFixedSize(720, 330);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setContentsMargins(0, 0, 0, 0);
    area->setFrameStyle(QFrame::NoFrame);

    // 内容窗口
    auto contentWidget = new QWidget(area);
    contentWidget->setObjectName("listenContentWidget");

    auto layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 分类及初始化函数
    const QList<QPair<QString, std::function<void(QWidget*)>>> categories = {
        {"contentWidget_novel",            [this](QWidget* w){ initAudioNovelWidget(w); }},
        {"contentWidget_children",         [this](QWidget* w){ initChildrenWorldWidget(w); }},
        {"contentWidget_comment",          [this](QWidget* w){ initCommentBookWidget(w); }},
        {"contentWidget_sleep",            [this](QWidget* w){ initSleepHelpingWidget(w); }},
        {"contentWidget_humanity",         [this](QWidget* w){ initHumanityWidget(w); }},
        {"contentWidget_self",             [this](QWidget* w){ initSelfChargingWidget(w); }},
        {"contentWidget_crosstalk",        [this](QWidget* w){ initCrosstalkWidget(w); }},
        {"contentWidget_sentimentality",   [this](QWidget* w){ initSentimentalityWidget(w); }},
        {"contentWidget_radio",            [this](QWidget* w){ initRadioDramaWidget(w); }},
        {"contentWidget_entertainment",    [this](QWidget* w){ initEntertainmentJokesWidget(w); }},
        {"contentWidget_acgn",             [this](QWidget* w){ initACGNWidget(w); }},
        {"contentWidget_podcast",          [this](QWidget* w){ initPodcastWidget(w); }},
        {"contentWidget_cantonese",        [this](QWidget* w){ initCantoneseWidget(w); }},
        {"contentWidget_foreign",          [this](QWidget* w){ initForeignLanguageWidget(w); }},
        {"contentWidget_createCover",      [this](QWidget* w){ initCreateCoverWidget(w); }},
        {"contentWidget_electronicSound",  [this](QWidget* w){ initElectronicSoundWidget(w); }}
    };

    for (int i = 0; i < categories.size(); ++i) {
        const auto& [name, initFunc] = categories[i];

        auto sectionWidget = new QWidget(contentWidget);
        sectionWidget->setObjectName(name);
        sectionWidget->setMouseTracking(true);
        sectionWidget->setContentsMargins(10, 0, 10, 0);

        initFunc(sectionWidget);
        layout->addWidget(sectionWidget);

        if (i != categories.size() - 1) {
            auto separator = new QFrame(contentWidget);
            separator->setObjectName("separator");
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            layout->addWidget(separator);
        }
    }

    contentWidget->setLayout(layout);
    area->setWidget(contentWidget);
}

/**
 * @brief 初始化有声小说分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initAudioNovelWidget(QWidget *widget) {
    widget->setFixedSize(735, 210); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0, 0, 0, 0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("有声小说 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "现代言情", "古代言情", "文学图书", "影视原著", "爆笑甜宠",
        "乡村生活", "玄幻异界", "萌宝甜宠", "科幻未来", "架空穿越",
        "出版图书", "青春校园", "历史", "都市纵横", "短剧",
        "游戏竞技", "武侠仙侠", "玄幻女强", "轻小说", "悬疑推理",
        "搞笑爽文", "豪门总裁", "关斗宅斗", "年代重生"
    };
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j >= 24) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化儿童天地分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initChildrenWorldWidget(QWidget *widget) {
    widget->setFixedSize(735, 130); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0, 0, 0, 0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("儿童天地 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "寓言童话", "早教课堂", "育儿科普", "教材教辅", "国学诗词",
        "儿童故事", "英语启蒙", "儿童歌谣", "趣味百科", "孕产胎教",
        "哄睡音乐", "卡通动画", "文学名著"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 12) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化评书分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initCommentBookWidget(QWidget *widget) {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("评书 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "评书大全", "袁阔成", "李庆丰", "田连元", "评书小说",
        "连丽如", "单田芳", "孙一"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化助眠解压分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initSleepHelpingWidget(QWidget *widget) {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("助眠解压 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "失眠必听", "放松音乐", "助眠故事", "自然疗愈", "能量颂钵",
        "情绪调节", "清新古风", "减压冥想", "脑波音乐", "耳朵按摩"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化人文分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initHumanityWidget(QWidget *widget) {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("人文 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "诗词歌赋", "文化大观", "国学经典", "艺术美学", "社科人文",
        "读书思考", "法律讲堂"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 6) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化自我充电分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initSelfChargingWidget(QWidget *widget) {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("自我充电 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "心理调节", "外语", "职场升级", "人际关系", "商业财经",
        "个人提升", "健康养生", "学科教育"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化相声曲艺分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initCrosstalkWidget(QWidget *widget) {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("相声曲艺 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "相声精选", "豫剧", "曲艺", "精选小品", "二人转",
        "黄梅戏", "越剧", "京剧"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化情感生活分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initSentimentalityWidget(QWidget *widget) {
    widget->setFixedSize(735, 130); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("情感生活 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "情话语录", "暖心治愈", "故事", "男友陪伴", "乐活消遣",
        "女声哄睡", "爱情治愈", "心理情绪", "晚安心语", "声音恋人",
        "婚恋关系", "美文伴读", "偶像暖心", "励志感悟"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 13) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化广播剧分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initRadioDramaWidget(QWidget *widget)  {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("广播剧 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "悬疑好剧", "言情", "全年龄", "影视剧场", "心动剧场",
        "漫剧场", "游戏剧场"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 6) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化娱乐段子分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initEntertainmentJokesWidget(QWidget *widget)   {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("娱乐段子 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "音乐推荐", "爱豆频道", "生活杂谈", "综艺节目", "脱口秀",
        "热剧影评", "奇闻怪谈", "搞笑段子"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化二次元分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initACGNWidget(QWidget *widget)    {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("二次元 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "次元速递", "声控福利", "游戏攻略", "ACG串烧", "ACG音声",
        "鬼畜名梗"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 5) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化播客分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initPodcastWidget(QWidget *widget)   {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("播客 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "生活观察", "商业洞察", "汽车资讯", "运动体育", "影视娱乐",
        "科技科学", "情感剧场", "音乐雷达", "人文艺术"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 8) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    //添加到contentWidget
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化粤语分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initCantoneseWidget(QWidget *widget)    {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("粤语 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "文化知识", "音乐FM", "劲歌金曲", "都市情感", "Talk Show",
        "生活资讯", "亲子故事", "粤韵风华", "粤语讲古"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 8) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化外语分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initForeignLanguageWidget(QWidget *widget)    {
    widget->setFixedSize(735, 55); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("外语 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "基础入门", "实用口语", "考试题材", "美文朗诵"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化创作翻唱分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initCreateCoverWidget(QWidget *widget)     {
    widget->setFixedSize(735, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("创作翻唱 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "纯音乐", "古风", "流行", "推荐", "其他",
        "外语"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 5) break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化DJ电音分类窗口
 * @param widget 目标控件
 */
void ListenOptionMenu::initElectronicSoundWidget(QWidget *widget)     {
    widget->setFixedSize(735, 55); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto vlay = new QVBoxLayout;
    vlay->addWidget(titleLab);
    vlay->addStretch();
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 10, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);

    // 标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("DJ|电音 >");
    titleLab->setFixedSize(80, 30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);

    // 宫格布局
    auto gridWidget = new QWidget(contentWidget);
    auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "舞曲串烧", "DJ嗨歌", "DJ舞曲"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 3; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
            label->installEventFilter(this);
            gridLayout->addWidget(label, i, j);
        }
    }
    gridWidget->setLayout(gridLayout);
    auto vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(gridWidget);
    vLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    auto contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addLayout(vLayout);
    contentLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    contentWidget->setLayout(contentLayout);
}

/**
 * @brief 初始化水平分隔线
 * @param frame 分隔线控件
 */
void ListenOptionMenu::initSeparator(QFrame *frame) {
    frame->setFixedWidth(675);
    frame->setObjectName("separator");
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);
}

/**
 * @brief 获取当前菜单对象
 * @return 当前菜单对象指针
 */
const ListenOptionMenu *ListenOptionMenu::getMenu() const {
    return this;
}

/**
 * @brief 事件过滤器，处理鼠标点击事件
 * @param watched 事件目标对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool ListenOptionMenu::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* label = qobject_cast<QLabel*>(watched);
        if (label && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
            emit clickedFuncName(label->text());
            return true;
        }
    }
    return BaseMenu::eventFilter(watched, event);
}