//
// Created by WeiWang on 25-2-3.
//

#include "ListenOptionMenu.h"
#include "logger.hpp"

#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListenOptionMenu::ListenOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/listenmenu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

void ListenOptionMenu::initMenu() {
    this->setFixedSize(740,370);
    this->setMouseTracking(true);
    //设置滚动区域
    auto area = new QScrollArea(this);
    area->move(15,20);
    area->setFixedSize(720, 330);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setContentsMargins(0,0,0,0);
    area->setFrameStyle(QFrame::NoFrame);
    //创建总体窗口
    const auto contentWidget = new QWidget(area);
    contentWidget->setContentsMargins(0,0,0,0);
    auto layout = new QVBoxLayout(contentWidget); // 使用垂直布局管理菜单项
    // 创建内容窗口（可以是 QWidget 或其他自定义控件）
    {
        //有声小说
        const auto contentWidget_novel = new QWidget(contentWidget);
        initAudioNovelWidget(contentWidget_novel);
        const auto separator1 = new QFrame(contentWidget);
        initSeparator(separator1);
        //儿童天地
        const auto contentWidget_children = new QWidget(contentWidget);
        initChildrenWorldWidget(contentWidget_children);
        const auto separator2 = new QFrame(contentWidget);
        initSeparator(separator2);
        //评书
        const auto contentWidget_comment = new QWidget(contentWidget);
        initCommentBookWidget(contentWidget_comment);
        const auto separator3 = new QFrame(contentWidget);
        initSeparator(separator3);
        //助眠解压
        const auto contentWidget_sleep = new QWidget(contentWidget);
        initSleepHelpingWidget(contentWidget_sleep);
        const auto separator4 = new QFrame(contentWidget);
        initSeparator(separator4);
        //人文
        const auto contentWidget_humanity = new QWidget(contentWidget);
        initHumanityWidget(contentWidget_humanity);
        const auto separator5 = new QFrame(contentWidget);
        initSeparator(separator5);
        //自我充电
        const auto contentWidget_self = new QWidget(contentWidget);
        initSelfChargingWidget(contentWidget_self);
        const auto separator6 = new QFrame(contentWidget);
        initSeparator(separator6);
        //相声曲艺
        const auto contentWidget_crosstalk = new QWidget(contentWidget);
        initCrosstalkWidget(contentWidget_crosstalk);
        const auto separator7 = new QFrame(contentWidget);
        initSeparator(separator7);
        //情感生活
        const auto contentWidget_sentimentality = new QWidget(contentWidget);
        initSentimentalityWidget(contentWidget_sentimentality);
        const auto separator8 = new QFrame(contentWidget);
        initSeparator(separator8);
        //广播剧
        const auto contentWidget_radio = new QWidget(contentWidget);
        initRadioDramaWidget(contentWidget_radio);
        const auto separator9 = new QFrame(contentWidget);
        initSeparator(separator9);
        //娱乐段子
        const auto contentWidget_entertainment = new QWidget(contentWidget);
        initEntertainmentJokesWidget(contentWidget_entertainment);
        const auto separator10 = new QFrame(contentWidget);
        initSeparator(separator10);
        //二次元
        const auto contentWidget_acgn = new QWidget(contentWidget);
        initACGNWidget(contentWidget_acgn);
        const auto separator11 = new QFrame(contentWidget);
        initSeparator(separator11);
        //播客
        const auto contentWidget_podcast = new QWidget(contentWidget);
        initPodcastWidget(contentWidget_podcast);
        const auto separator12 = new QFrame(contentWidget);
        initSeparator(separator12);
        //粤语
        const auto contentWidget_cantonese = new QWidget(contentWidget);
        initCantoneseWidget(contentWidget_cantonese);
        const auto separator13 = new QFrame(contentWidget);
        initSeparator(separator13);
        //外语
        const auto contentWidget_foreign = new QWidget(contentWidget);
        initForeignLanguageWidget(contentWidget_foreign);
        const auto separator14 = new QFrame(contentWidget);
        initSeparator(separator14);
        //创作翻唱
        const auto contentWidget_createCover = new QWidget(contentWidget);
        initCreateCoverWidget(contentWidget_createCover);
        const auto separator15 = new QFrame(contentWidget);
        initSeparator(separator15);
        //DJ电音
        const auto contentWidget_electronicSound = new QWidget(contentWidget);
        initElectronicSoundWidget(contentWidget_electronicSound);
        // 设置滚动区域的内容窗口
        layout->addWidget(contentWidget_novel);
        layout->addWidget(separator1);
        layout->addWidget(contentWidget_children);
        layout->addWidget(separator2);
        layout->addWidget(contentWidget_comment);
        layout->addWidget(separator3);
        layout->addWidget(contentWidget_sleep);
        layout->addWidget(separator4);
        layout->addWidget(contentWidget_humanity);
        layout->addWidget(separator5);
        layout->addWidget(contentWidget_self);
        layout->addWidget(separator6);
        layout->addWidget(contentWidget_crosstalk);
        layout->addWidget(separator7);
        layout->addWidget(contentWidget_sentimentality);
        layout->addWidget(separator8);
        layout->addWidget(contentWidget_radio);
        layout->addWidget(separator9);
        layout->addWidget(contentWidget_entertainment);
        layout->addWidget(separator10);
        layout->addWidget(contentWidget_acgn);
        layout->addWidget(separator11);
        layout->addWidget(contentWidget_podcast);
        layout->addWidget(separator12);
        layout->addWidget(contentWidget_cantonese);
        layout->addWidget(separator13);
        layout->addWidget(contentWidget_foreign);
        layout->addWidget(separator14);
        layout->addWidget(contentWidget_createCover);
        layout->addWidget(separator15);
        layout->addWidget(contentWidget_electronicSound);
    }
    contentWidget->setLayout(layout);
    area->setWidget(contentWidget);
}

void ListenOptionMenu::initAudioNovelWidget(QWidget *widget) {
    widget->setFixedSize(735, 210); // 设置内容窗口大小（根据需求调整）
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
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(vlay);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("有声小说 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "现代言情" , "古代言情" , "文学图书" , "影视原著" , "爆笑甜宠" ,
        "乡村生活" , "玄幻异界" ,"萌宝甜宠" , "科幻未来" , "架空穿越" ,
        "出版图书" , "青春校园" , "历史" , "都市纵横" , "短剧" ,
        "游戏竞技" , "武侠仙侠" , "玄幻女强" , "轻小说" , "悬疑推理" ,
        "搞笑爽文" , "豪门总裁" , "关斗宅斗" , "年代重生"
    };
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j >= 24)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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

void ListenOptionMenu::initChildrenWorldWidget(QWidget *widget) {
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("儿童天地 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "寓言童话" , "早教课堂" , "育儿科普" , "教材教辅" , "国学诗词" ,
        "儿童故事" , "英语启蒙" ,"儿童歌谣" , "趣味百科" , "孕产胎教" ,
        "哄睡音乐" , "卡通动画" , "文学名著"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 12)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("评书 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "评书大全" , "袁阔成" , "李庆丰" , "田连元" , "评书小说" ,
        "连丽如" , "单田芳" ,"孙一"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("助眠解压 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "失眠必听" , "放松音乐" , "助眠故事" , "自然疗愈" , "能量颂钵" ,
        "情绪调节" , "清新古风" ,"减压冥想", "脑波音乐" , "耳朵按摩"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            //if (i * 5 + j > 7)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("人文 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "诗词歌赋" , "文化大观" , "国学经典" , "艺术美学" , "社科人文" ,
        "读书思考" , "法律讲堂"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 6)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("自我充电 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "心理调节" , "外语" , "职场升级" , "人际关系" , "商业财经" ,
        "个人提升" , "健康养生", "学科教育"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("相声曲艺 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "相声精选" , "豫剧" , "曲艺" , "精选小品" , "二人转" ,
        "黄梅戏" , "越剧", "京剧"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("情感生活 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "情话语录" , "暖心治愈" , "故事" , "男友陪伴" , "乐活消遣" ,
        "女声哄睡" , "爱情治愈" ,"心理情绪" , "晚安心语" , "声音恋人" ,
        "婚恋关系" , "美文伴读" , "偶像暖心", "励志感悟"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 13)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("广播剧 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "悬疑好剧" , "言情" , "全年龄" , "影视剧场" , "心动剧场" ,
        "漫剧场" , "游戏剧场"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 6)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("娱乐段子 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "音乐推荐" , "爱豆频道" , "生活杂谈" , "综艺节目" , "脱口秀" ,
        "热剧影评" , "奇闻怪谈", "搞笑段子"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 7)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("二次元 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "次元速递" , "声控福利" , "游戏攻略" , "ACG串烧" , "ACG音声" ,
        "鬼畜名梗"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 5)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("播客 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "生活观察" , "商业洞察" , "汽车资讯" , "运动体育" , "影视娱乐" ,
        "科技科学" , "情感剧场", "音乐雷达", "人文艺术"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 8)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("粤语 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "文化知识" , "音乐FM" , "劲歌金曲" , "都市情感" , "Talk Show" ,
        "生活咨讯" , "亲子故事", "粤韵风华", "粤语讲古"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 8)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("外语 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "基础入门" , "实用口语" , "考试题材" , "美文朗诵"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("创作翻唱 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "纯音乐" , "古风" , "流行" , "推荐" , "其他" ,
        "外语"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 5 + j > 5)break;
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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
    //标题
    titleLab->setCursor(Qt::PointingHandCursor);
    titleLab->setObjectName("titleLab");
    titleLab->setText("DJ|电音 >");
    titleLab->setFixedSize(80,30);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 5, 0, 0);
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(12);
    gridLayout->setHorizontalSpacing(12);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "舞曲串烧" , "DJ嗨歌" , "DJ舞曲"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 3; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(110, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 5 + j]);
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

void ListenOptionMenu::initSeparator(QFrame *frame) {
    frame->setFixedWidth(675);
    frame->setObjectName("separator");
    frame->setFrameShape(QFrame::HLine);  // 水平分隔线
    frame->setFrameShadow(QFrame::Sunken);
}

const ListenOptionMenu * ListenOptionMenu::getMenu() const {
    return this;
}

