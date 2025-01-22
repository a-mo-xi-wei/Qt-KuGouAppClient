//
// Created by WeiWang on 25-1-12.
//

#include "ListOptionMenu.h"

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
    area->setFixedSize(678, 360);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setContentsMargins(0,0,0,0);
    area->setFrameStyle(QFrame::NoFrame);
    //创建总体窗口
    auto contentWidget = new QWidget(area);
    contentWidget->setContentsMargins(0,0,0,0);
    auto layout = new QVBoxLayout(contentWidget); // 使用垂直布局管理菜单项
    // 创建内容窗口（可以是 QWidget 或其他自定义控件）
    //场景
    auto contentWidget_scene = new QWidget(contentWidget);
    initSceneWidget(contentWidget_scene);
    const auto separator1 = new QFrame(contentWidget);
    separator1->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator1->setFrameShadow(QFrame::Sunken);
    //主题
    auto contentWidget_theme = new QWidget(contentWidget);
    initThemeWidget(contentWidget_theme);
    const auto separator2 = new QFrame(contentWidget);
    separator2->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator2->setFrameShadow(QFrame::Sunken);
    //语种
    auto contentWidget_language = new QWidget(contentWidget);
    initLanguageWidget(contentWidget_language);
    const auto separator3 = new QFrame(contentWidget);
    separator3->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator3->setFrameShadow(QFrame::Sunken);
    //风格
    auto contentWidget_style = new QWidget(contentWidget);
    initStyleWidget(contentWidget_style);
    const auto separator4 = new QFrame(contentWidget);
    separator4->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator4->setFrameShadow(QFrame::Sunken);
    //心情
    auto contentWidget_mood = new QWidget(contentWidget);
    initMoodWidget(contentWidget_mood);
    const auto separator5 = new QFrame(contentWidget);
    separator5->setFrameShape(QFrame::HLine);  // 水平分隔线
    separator5->setFrameShadow(QFrame::Sunken);
    //年代
    auto contentWidget_period = new QWidget(contentWidget);
    initPeriodWidget(contentWidget_period);
    const auto separator6 = new QFrame(contentWidget);
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
    layout->addWidget(separator6);
    
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
    widget->setFixedSize(695, 180); // 设置内容窗口大小（根据需求调整）
}

void ListOptionMenu::initLanguageWidget(QWidget *widget) {
    widget->setFixedSize(695, 140); // 设置内容窗口大小（根据需求调整）
}

void ListOptionMenu::initStyleWidget(QWidget *widget) {
    widget->setFixedSize(695, 180); // 设置内容窗口大小（根据需求调整）
}

void ListOptionMenu::initMoodWidget(QWidget *widget) {
    widget->setFixedSize(695, 140); // 设置内容窗口大小（根据需求调整）
}

void ListOptionMenu::initPeriodWidget(QWidget *widget) {
    widget->setFixedSize(695, 100); // 设置内容窗口大小（根据需求调整）
}

const ListOptionMenu * ListOptionMenu::getMenu() const {
    return this;
}

