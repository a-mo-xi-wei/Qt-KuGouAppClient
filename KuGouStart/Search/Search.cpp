//
// Created by WeiWang on 24-11-18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Search.h" resolved

#include "Search.h"
#include "ui_Search.h"
#include"MyFlowLayout.h"

#include<QFile>
#include<QButtonGroup>
#include<QRandomGenerator>

#include <mutex>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

//唯一标志
namespace SearchFlag {
    std::once_flag flag1;
    std::once_flag flag2;
    std::once_flag flag3;
    std::once_flag flag4;
}

#define IMAGE_WIDTH 102     //图片的宽度（正方形）

Search::Search(QWidget *parent) :
    QWidget(parent), ui(new Ui::Search)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/search.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initStackWidget();
    initUi();

}

Search::~Search() {
    delete ui;
}

void Search::initUi() {
    {
        //设置图标
        ui->toolButton1->setIcon(QIcon(QStringLiteral(":/Search/Res/search/phonePlay.png")));
        ui->toolButton2->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-live.png")));
        ui->toolButton3->setIcon(QIcon(QStringLiteral(":/Search/Res/search/wallpaper.png")));
        ui->toolButton4->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-pingbao.png")));
        ui->toolButton5->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundEffect.png")));
        ui->toolButton6->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundPlugin.png")));
        ui->toolButton7->setIcon(QIcon(QStringLiteral(":/Search/Res/search/ringMake.png")));
        ui->toolButton8->setIcon(QIcon(QStringLiteral(":/Search/Res/search/remoteControl.png")));
        ui->toolButton9->setIcon(QIcon(QStringLiteral(":/Search/Res/search/musicCircle.png")));
        ui->toolButton10->setIcon(QIcon(QStringLiteral(":/Search/Res/search/cd.png")));
        ui->toolButton11->setIcon(QIcon(QStringLiteral(":/Search/Res/search/equalizer.png")));
        ui->toolButton12->setIcon(QIcon(QStringLiteral(":/Search/Res/search/timing.png")));
        ui->toolButton13->setIcon(QIcon(QStringLiteral(":/Search/Res/search/DLNA.png")));
        ui->toolButton14->setIcon(QIcon(QStringLiteral(":/Search/Res/search/change.png")));
        ui->toolButton15->setIcon(QIcon(QStringLiteral(":/Search/Res/search/netTest.png")));
        ui->toolButton16->setIcon(QIcon(QStringLiteral(":/Search/Res/search/earnCoin.png")));
    }
    {
        //设置互斥
        this->m_buttonGroup->addButton(ui->recommend_pushButton);
        this->m_buttonGroup->addButton(ui->rank_pushButton);
        this->m_buttonGroup->addButton(ui->special_pushButton);
        this->m_buttonGroup->addButton(ui->channel_pushButton);
        this->m_buttonGroup->setExclusive(true);
    }
    {
        //下标图片
        ui->index_label1->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label2->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label3->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label4->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg")));
        ui->index_label2->hide();
        ui->index_label3->hide();
        ui->index_label4->hide();
    }
    initCoverVector();
    initDescVector();
    ui->recommend_pushButton->clicked();
}

void Search::initStackWidget() {
    //分配内存
    this->m_recommendWidget = std::make_unique<QWidget>(ui->stackedWidget);
    this->m_rankWidget      = std::make_unique<QWidget>(ui->stackedWidget);
    this->m_specialWidget   = std::make_unique<QWidget>(ui->stackedWidget);
    this->m_channelWidget   = std::make_unique<QWidget>(ui->stackedWidget);
    //设置布局
    auto lay1 = new MyFlowLayout(this->m_recommendWidget.get(),10, -1, 2);
    this->m_recommendWidget->setLayout(lay1);
    this->m_recommendWidget->setFixedHeight(1200);
    auto lay2 = new MyFlowLayout(this->m_rankWidget.get(),10, -1, -1);
    this->m_rankWidget->setLayout(lay2);
    this->m_rankWidget->setFixedHeight(600);
    auto lay3 = new MyFlowLayout(this->m_specialWidget.get(),10, -1, -1);
    this->m_specialWidget->setLayout(lay3);
    this->m_specialWidget->setFixedHeight(800);
    auto lay4 = new MyFlowLayout(this->m_channelWidget.get(),10, -1, -1);
    this->m_channelWidget->setLayout(lay4);
    this->m_channelWidget->setFixedHeight(200);
    this->m_channelWidget->setObjectName("channelWidget");

    ui->stackedWidget->addWidget(this->m_recommendWidget.get());
    ui->stackedWidget->addWidget(this->m_rankWidget.get());
    ui->stackedWidget->addWidget(this->m_specialWidget.get());
    ui->stackedWidget->addWidget(this->m_channelWidget.get());
}

void Search::initCoverVector() {
    //插入60张图片
    for(int i = 1 ; i <= 9 ; i++) {
        this->m_coverVector.emplace_back(QString(":/Search/Res/search/block0%1.png").arg(i));
    }
    for(int i = 10 ; i <= 60 ; i++) {
        this->m_coverVector.emplace_back(QString(":/Search/Res/search/block%1.png").arg(i));
    }
}

void Search::initDescVector() {
    QStringList list = {"酷歌词","抖音潮流区","开车必备歌曲专区","抖音DJ",
        "2021抖音最火歌曲","DJ必备歌曲","伤感音乐","车载DJ","植物大战僵尸",
        "抖音热歌","刀郎老哥合集","魔道祖师","邓丽君老歌合集","学生党专区",
        "夜听伤感频道","纯音乐路的尽头会是温柔和月光","鞠婧祎的歌","快手抖音最火歌曲集合",
        "肖战","KG大神","我的世界","神仙翻唱","岁月陈酿过的粤语老歌","治愈专区",
        "林俊杰音乐汇","第五人格角色曲","满载回忆的华语经典","云南山歌-单曲-专辑精选汇聚",
        "抖音热歌榜","轻音乐","睡眠音乐","游戏高燃","车载电音缓解疲劳专用","古风视频专区",
        "TFBOYS音乐小屋","纯音乐钢琴","伤感情歌静静聆听","名侦探柯南","DJ龙二少音乐作品",
        "初音未来","德云社","王俊凯免费歌曲不重复","草原歌后乌兰图雅","就爱老哥带DJ",
        "心情治疗诊所","民谣聚集地","私藏歌单等你来听","古风亦可DJ-中国风也能蹦迪",
        "朴彩英专区","AW经典电影","电子音乐","BLACKPINK","每日必听的粤语歌单",
        "薛之谦热歌榜"};
    for(const auto &str : list) {
        this->m_descVector.emplace_back(str);
    }
}

void Search::refresh() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_coverVector.begin(), this->m_coverVector.end(), std::default_random_engine(seed));
    std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed));
}

void Search::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"Search::resizeEvent";
    // 获取顶级窗口的指针
    QWidget *topLevelWindow = this->window();
    if (!topLevelWindow) {
        qWarning() << "无法获取顶级窗口！";
        return;
    }

    const int topLevelWidth = topLevelWindow->width();
    //qDebug()<<"顶级窗口宽度为："<<topLevelWidth<<" 当前窗口宽度为："<<this->width();
    if(this->width() > topLevelWidth) {
        auto geo = this->geometry();
        geo.setWidth(topLevelWidth - 10);
        this->setGeometry(geo);
        //qDebug()<<"修改后顶级窗口宽度为："<<topLevelWidth<<" 当前窗口宽度为："<<this->width();
    }
    {
        //无用
        //auto geo = ui->stackedWidget->geometry();
        //for (int i = 0; i < ui->stackedWidget->count(); ++i) {
        //    QWidget *page = ui->stackedWidget->widget(i);
        //    if (page) {
        //        auto pageGeo = page->geometry();
        //        pageGeo.setWidth(geo.width()); // 与堆栈窗口一致
        //        page->setGeometry(pageGeo);
        //        qDebug()<<"page "<<i + 1<<" 改变宽度为 "<<geo.width();
        //    }
        //}
    }

}

void Search::on_recommend_pushButton_clicked() {
    ui->index_label1->show();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();
    auto handle = [this] {
        refresh();
        auto lay = static_cast<MyFlowLayout*>(this->m_recommendWidget->layout());
        if(!lay) {
            qDebug()<<"布局错误";
            return;
        }
        //54个图片、描述
        for (int i = 0; i < 54; ++i) {
            auto btn = new QToolButton(this->m_recommendWidget.get());
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH));
            btn->setIcon(this->m_coverVector[i]);
            QFont font("微软雅黑",10);
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];
            auto elidedText = fm.elidedText(text,Qt::ElideRight,IMAGE_WIDTH);
            // 检查是否被省略
            if (elidedText != text) {
                btn->setToolTip(text); // 被省略时设置完整文本为 tooltip
            }
            btn->setText(elidedText);
            lay->addWidget(btn);
        }
    };
    std::call_once(SearchFlag::flag1,handle);
    ui->stackedWidget->setCurrentWidget(this->m_recommendWidget.get());
    ui->stackedWidget->setFixedHeight(this->m_recommendWidget->height()+30);
}

void Search::on_rank_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->show();
    ui->index_label3->hide();
    ui->index_label4->hide();
    auto handle = [this] {
        refresh();
        auto lay = static_cast<MyFlowLayout*>(this->m_rankWidget->layout());
        if(!lay) {
            qDebug()<<"布局错误";
            return;
        }
        //19个图片、描述
        for (int i = 0; i < 19; ++i) {
            auto btn = new QToolButton(this->m_rankWidget.get());
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH));
            btn->setIcon(this->m_coverVector[i]);
            QFont font("微软雅黑",10);
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];
            auto elidedText = fm.elidedText(text,Qt::ElideRight,IMAGE_WIDTH);
            // 检查是否被省略
            if (elidedText != text) {
                btn->setToolTip(text); // 被省略时设置完整文本为 tooltip
            }
            btn->setText(elidedText);
            lay->addWidget(btn);
        }
    };
    std::call_once(SearchFlag::flag2,handle);
    ui->stackedWidget->setCurrentWidget(this->m_rankWidget.get());
    ui->stackedWidget->setFixedHeight(this->m_rankWidget->height()+30);
}

void Search::on_special_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->show();
    ui->index_label4->hide();
    auto handle = [this] {
        refresh();
        auto lay = static_cast<MyFlowLayout*>(this->m_specialWidget->layout());
        if(!lay) {
            qDebug()<<"布局错误";
            return;
        }
        //27个图片、描述
        for (int i = 0; i < 27; ++i) {
            auto btn = new QToolButton(this->m_specialWidget.get());
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH));
            btn->setIcon(this->m_coverVector[i]);
            QFont font("微软雅黑",10);
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];
            auto elidedText = fm.elidedText(text,Qt::ElideRight,IMAGE_WIDTH);
            // 检查是否被省略
            if (elidedText != text) {
                btn->setToolTip(text); // 被省略时设置完整文本为 tooltip
            }
            btn->setText(elidedText);
            lay->addWidget(btn);
        }
    };
    std::call_once(SearchFlag::flag3,handle);
    ui->stackedWidget->setCurrentWidget(this->m_specialWidget.get());
    ui->stackedWidget->setFixedHeight(this->m_specialWidget->height()+30);
}

void Search::on_channel_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->show();
    auto handle = [this] {
        refresh();
        auto lay = static_cast<MyFlowLayout*>(this->m_channelWidget->layout());
        if(!lay) {
            qDebug()<<"布局错误";
            return;
        }
        //7个图片、描述
        for (int i = 0; i < 7; ++i) {
            auto btn = new QToolButton(this->m_channelWidget.get());
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH));
            btn->setIcon(this->m_coverVector[i]);
            QFont font("微软雅黑",10);
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];
            auto elidedText = fm.elidedText(text,Qt::ElideRight,IMAGE_WIDTH);
            // 检查是否被省略
            if (elidedText != text) {
                btn->setToolTip(text); // 被省略时设置完整文本为 tooltip
            }
            btn->setText(elidedText);
            lay->addWidget(btn);
        }
    };
    std::call_once(SearchFlag::flag4,handle);
    ui->stackedWidget->setCurrentWidget(this->m_channelWidget.get());
    ui->stackedWidget->setFixedHeight(this->m_channelWidget->height()+30);
    //qDebug()<<"this->height() : "<<this->height()<<
    //    "this->m_channelWidget->height() : "<<this->m_channelWidget->height();
}
