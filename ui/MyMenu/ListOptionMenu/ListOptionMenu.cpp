//
// Created by WeiWang on 25-1-12.
//

#include "ListOptionMenu.h"
#include "logger.hpp"

#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

ListOptionMenu::ListOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/listmenu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}
void ListOptionMenu::initMenu() {
    this->setFixedSize(700,400);
    this->setMouseTracking(true);
    //设置滚动区域
    auto area = new QScrollArea(this);
    //area->setGeometry(17,20, 678, 360);
    area->move(10,15);
    area->setFixedSize(685, 360);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setContentsMargins(0,0,0,0);
    area->setFrameStyle(QFrame::NoFrame);
    //创建总体窗口
    const auto contentWidget = new QWidget(area);
    contentWidget->setContentsMargins(0,0,0,0);
    auto layout = new QVBoxLayout(contentWidget); // 使用垂直布局管理菜单项
    // 创建内容窗口（可以是 QWidget 或其他自定义控件）
    //场景
    const auto contentWidget_scene = new QWidget(contentWidget);
    initSceneWidget(contentWidget_scene);
    const auto separator1 = new QFrame(contentWidget);
    separator1->setFixedWidth(650);
    separator1->setObjectName("separator");
    separator1->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator1->setFrameShadow(QFrame::Sunken);
    //主题
    const auto contentWidget_theme = new QWidget(contentWidget);
    initThemeWidget(contentWidget_theme);
    const auto separator2 = new QFrame(contentWidget);
    separator2->setFixedWidth(650);
    separator2->setObjectName("separator");
    separator2->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator2->setFrameShadow(QFrame::Sunken);
    //语种
    const auto contentWidget_language = new QWidget(contentWidget);
    initLanguageWidget(contentWidget_language);
    const auto separator3 = new QFrame(contentWidget);
    separator3->setFixedWidth(650);
    separator3->setObjectName("separator");
    separator3->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator3->setFrameShadow(QFrame::Sunken);
    //风格
    const auto contentWidget_style = new QWidget(contentWidget);
    initStyleWidget(contentWidget_style);
    const auto separator4 = new QFrame(contentWidget);
    separator4->setFixedWidth(650);
    separator4->setObjectName("separator");
    separator4->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator4->setFrameShadow(QFrame::Sunken);
    //心情
    const auto contentWidget_mood = new QWidget(contentWidget);
    initMoodWidget(contentWidget_mood);
    const auto separator5 = new QFrame(contentWidget);
    separator5->setFixedWidth(650);
    separator5->setObjectName("separator");
    separator5->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator5->setFrameShadow(QFrame::Sunken);
    //年代
    const auto contentWidget_period = new QWidget(contentWidget);
    initPeriodWidget(contentWidget_period);
    // 设置滚动区域的内容窗口
    layout->addWidget(contentWidget_scene);
    layout->addWidget(separator1);
    layout->addWidget(contentWidget_theme);
    layout->addWidget(separator2);
    layout->addWidget(contentWidget_language);
    layout->addWidget(separator3);
    layout->addWidget(contentWidget_style);
    layout->addWidget(separator4);
    layout->addWidget(contentWidget_mood);
    layout->addWidget(separator5);
    layout->addWidget(contentWidget_period);

    contentWidget->setLayout(layout);
    area->setWidget(contentWidget);
}

void ListOptionMenu::initSceneWidget(QWidget *widget) {
    widget->setFixedSize(695, 130); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,0,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("场景");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 3, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "学习" , "工作" , "通勤" , "运动" , "校园" , "旅途" , "咖啡厅" ,
        "店铺" , "清晨" , "下午茶" , "夜晚" , "睡前" , "派对" , "宅家" ,
        "车载" , "夜店" , "婚礼" , "瑜伽" , "做家务" , "公共交通" , "茶馆"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 7; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
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

void ListOptionMenu::initThemeWidget(QWidget *widget) {
    widget->setFixedSize(695, 130); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,11,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("主题");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 14, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "精选" , "经典" , "网络" , "游戏" , "DJ热碟" , "情歌对唱" , "舞曲" ,
        "KTV" , "影视" , "翻唱" , "ACG" , "现场" , "综艺" , "厂牌音乐" ,
        "BGM" , "儿童" , "器乐演奏" , "官方歌单" , "草原风" , "广场舞" , "露营"
    };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 7; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
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

void ListOptionMenu::initLanguageWidget(QWidget *widget) {
    widget->setFixedSize(695, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,11,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("语种");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 14, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "国语" , "英语" , "粤语" , "日语" , "韩语" , "闽南语" , "小语种" ,"法语"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 7; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
            gridLayout->addWidget(label, i, j);
        }
    }
    //剩余的一个
    const auto label = new QLabel(contentWidget);
    label->setMouseTracking(true);
    label->setFixedSize(75, 30);
    label->setAlignment(Qt::AlignCenter);
    label->setCursor(Qt::PointingHandCursor);
    label->setText(labArr[7]);
    gridLayout->addWidget(label, 1,0);
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

void ListOptionMenu::initStyleWidget(QWidget *widget) {
    widget->setFixedSize(695, 130); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,11,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("风格");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 14, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "流行" , "古风" , "电子" , "民谣" , "摇滚" , "说唱" , "后摇" ,
        "中国风" , "R&B" , "古典" , "乡村" , "爵士" , "新世纪" , "布鲁斯" ,
        "拉丁" , "轻音乐" , "中国传统" , "金属" , "雷鬼" , "另类独立"
    };

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
            gridLayout->addWidget(label, i, j);
        }
    }
    for (int j = 0; j < 6; ++j) {
        const auto label = new QLabel(contentWidget);
        label->setMouseTracking(true);
        label->setFixedSize(75, 30);
        label->setAlignment(Qt::AlignCenter);
        label->setCursor(Qt::PointingHandCursor);
        label->setText(labArr[2 * 7 + j]);
        gridLayout->addWidget(label, 2, j);
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

void ListOptionMenu::initMoodWidget(QWidget *widget) {
    widget->setFixedSize(695, 100); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,11,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("心情");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 14, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "怀旧" , "伤感" , "安静" , "兴奋" , "轻松" , "治愈" , "快乐" ,
        "甜蜜" , "寂寞" , "感动" , "小清晰" , "励志" , "减压" , "失恋"
    };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
            gridLayout->addWidget(label, i, j);
        }
    }

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

void ListOptionMenu::initPeriodWidget(QWidget *widget) {
    widget->setFixedSize(695, 40); // 设置内容窗口大小（根据需求调整）
    //整体水平布局
    const auto mainLayout = new QHBoxLayout(widget);
    const auto titleLab = new QLabel(widget);
    const auto contentWidget = new QWidget(widget);
    contentWidget->setContentsMargins(0,11,0,0);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(titleLab);
    mainLayout->addWidget(contentWidget);
    //标题
    titleLab->setObjectName("titleLab");
    titleLab->setText("年代");
    titleLab->setFixedWidth(70);
    titleLab->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    titleLab->setContentsMargins(0, 14, 0, 0);
    //突出标记
    //widget->setStyleSheet("background-color:yellow;");
    //contentWidget->setStyleSheet("background-color:red;");
    //宫格布局
    auto gridWidget = new QWidget(contentWidget);
    const auto gridLayout = new QGridLayout(gridWidget);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setHorizontalSpacing(5);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    const QString labArr[] = {
        "70后" , "80后" , "90后" , "00后"
    };
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            const auto label = new QLabel(contentWidget);
            label->setMouseTracking(true);
            label->setFixedSize(75, 30);
            label->setAlignment(Qt::AlignCenter);
            label->setCursor(Qt::PointingHandCursor);
            label->setText(labArr[i * 7 + j]);
            gridLayout->addWidget(label, i, j);
        }
    }

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

const ListOptionMenu * ListOptionMenu::getMenu() const {
    return this;
}

