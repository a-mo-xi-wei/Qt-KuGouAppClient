//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoWidget.h" resolved

#include "VideoWidget.h"
#include "ui_VideoWidget.h"
#include "logger.hpp"

#include<QFile>
#include<QButtonGroup>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <random>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

int offY = 297;

VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_recommendWidget(std::make_unique<VideoPartWidget>(this))
    , m_videoRankWidget(std::make_unique<VideoPartWidget>(this))
    , m_MVWidget (std::make_unique<VideoPartWidget>(this))
    , m_siteWidget(std::make_unique<VideoPartWidget>(this))
    , m_coverWidget(std::make_unique<VideoPartWidget>(this))
    , m_danceWidget(std::make_unique<VideoPartWidget>(this))
    , m_childrenWidget(std::make_unique<VideoPartWidget>(this))
    , m_liveWidget(std::make_unique<VideoPartWidget>(this))
    , m_firstConcertWidget(std::make_unique<VideoPartWidget>(this))
    , m_chineseLanguageWidget(std::make_unique<VideoPartWidget>(this))
    , m_southKoreaWidget(std::make_unique<VideoPartWidget>(this))
    , m_japanWidget(std::make_unique<VideoPartWidget>(this))
    , m_americanWidget(std::make_unique<VideoPartWidget>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initButtonGroup();
    initTotalWidget();
    initVector();
    initUi();
}

VideoWidget::~VideoWidget() {
    delete ui;
}

void VideoWidget::initButtonGroup() const {
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->video_rank_pushButton);
    this->m_buttonGroup->addButton(ui->MV_pushButton);
    this->m_buttonGroup->addButton(ui->site_pushButton);
    this->m_buttonGroup->addButton(ui->cover_pushButton);
    this->m_buttonGroup->addButton(ui->dance_pushButton);
    this->m_buttonGroup->addButton(ui->children_pushButton);
    this->m_buttonGroup->addButton(ui->live_pushButton);
    this->m_buttonGroup->addButton(ui->first_concert_pushButton);
    this->m_buttonGroup->addButton(ui->chinese_language_pushButton);
    this->m_buttonGroup->addButton(ui->South_Korea_pushButton);
    this->m_buttonGroup->addButton(ui->Japan_pushButton);
    this->m_buttonGroup->addButton(ui->American_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void VideoWidget::initTotalWidget() const {
    this->m_recommendWidget      ->setTitleName("推荐");
    this->m_videoRankWidget      ->setTitleName("视频榜");
    this->m_MVWidget             ->setTitleName("MV");
    this->m_siteWidget           ->setTitleName("现场");
    this->m_coverWidget          ->setTitleName("翻唱");
    this->m_danceWidget          ->setTitleName("舞蹈");
    this->m_childrenWidget       ->setTitleName("儿童");
    this->m_liveWidget           ->setTitleName("TME LIVE");
    this->m_firstConcertWidget   ->setTitleName("首唱会");
    this->m_chineseLanguageWidget->setTitleName("华语");
    this->m_southKoreaWidget     ->setTitleName("韩国");
    this->m_japanWidget          ->setTitleName("日本");
    this->m_americanWidget       ->setTitleName("欧美");
}

void VideoWidget::initVector() {
    for (int i = 1; i <= 120; ++i) {
        this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i));
    }
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed));
}

void VideoWidget::initUi() {
    {
        //加入布局
        auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
        lay->setSpacing(0);
        lay->insertWidget(lay->count() - 1, m_recommendWidget.get());
        lay->insertWidget(lay->count() - 1, m_videoRankWidget.get());
        lay->insertWidget(lay->count() - 1, m_MVWidget.get());
        lay->insertWidget(lay->count() - 1, m_siteWidget.get());
        lay->insertWidget(lay->count() - 1, m_coverWidget.get());
        lay->insertWidget(lay->count() - 1, m_danceWidget.get());
        lay->insertWidget(lay->count() - 1, m_childrenWidget.get());
        lay->insertWidget(lay->count() - 1, m_liveWidget.get());
        lay->insertWidget(lay->count() - 1, m_firstConcertWidget.get());
        lay->insertWidget(lay->count() - 1, m_chineseLanguageWidget.get());
        lay->insertWidget(lay->count() - 1, m_southKoreaWidget.get());
        lay->insertWidget(lay->count() - 1, m_japanWidget.get());
        lay->insertWidget(lay->count() - 1, m_americanWidget.get());
    }
    {
        this->m_vScrollBar = ui->scrollArea->verticalScrollBar();

        auto connectButton = [this](const QPushButton* button, QWidget* targetWidget) {
            connect(button, &QPushButton::clicked, this, [this, targetWidget] {
                ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y());
            });
        };
        /*
        //处理信号
        connect(ui->recommend_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_recommendWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->video_rank_pushButton,&QPushButton::clicked,this,[this] {
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
        connectButton(ui->recommend_pushButton, this->m_recommendWidget.get());
        connectButton(ui->video_rank_pushButton, this->m_videoRankWidget.get());
        connectButton(ui->MV_pushButton, this->m_MVWidget.get());
        connectButton(ui->site_pushButton, this->m_siteWidget.get());
        connectButton(ui->cover_pushButton, this->m_coverWidget.get());
        connectButton(ui->dance_pushButton, this->m_danceWidget.get());
        connectButton(ui->children_pushButton, this->m_childrenWidget.get());
        connectButton(ui->live_pushButton, this->m_liveWidget.get());
        connectButton(ui->first_concert_pushButton, this->m_firstConcertWidget.get());
        connectButton(ui->chinese_language_pushButton, this->m_chineseLanguageWidget.get());
        connectButton(ui->South_Korea_pushButton, this->m_southKoreaWidget.get());
        connectButton(ui->Japan_pushButton, this->m_japanWidget.get());
        connectButton(ui->American_pushButton, this->m_americanWidget.get());

        //wheelVaue信号
        connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &VideoWidget::handleWheelValue);
        connect(this->m_vScrollBar,&QScrollBar::valueChanged,this, &VideoWidget::handleWheelValue);
    }
    {
        //m_recommendWidget
        const QString tipText[] = {
            "","音乐现场","综艺制作形式","舞蹈","创意","演奏","舞蹈"
        };
        const QString videoName[] = {
            "","把《恐龙抗狼》改得燃一点？","连名带姓（live）","花姐《一个多情又无情的人》DJ舞曲版",
            "民族舞","二胡演奏 萨卡萨卡班班甲鱼鱼","ROSE《APT》2024/11/29"
        };
        const QString author[] = {
            "","武言圣","刘天佑Tay","网络潮流大时代","往昔忆尽红","雷天神","你好我的女神"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx]);
                block->setShowTip();
                block->setTipText(tipText[idx]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx]);
                this->m_recommendWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    auto cur = 6;
    {
        //m_videoRankWidget
        const QString videoName[] = {
            "","永不言败","无名的人","toxic till the end",
            "茶花开了","只字不提","陪伴"
        };
        const QString author[] = {
            "","周深","张韶涵","ROSE","王睿卓","于和伟","陈慧娴"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_videoRankWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_MVWidget
        const QString videoName[] = {
            "","APT.","aespa 《Whiplash》MV","Stray Kids \"Walkin On Water\" M/V",
            "战·永不言败","谭维维MV上线","\'DRIP\' MV"
        };
        const QString author[] = {
            "","Bruno Mars","aespa","Stray Kids","周深","谭维维","BABYMONSTER"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_MVWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_siteWidget
        const QString videoName[] = {
            "","无效陪伴","达拉崩吧","壁上观",
            "11","Strategy","Warp Me In Plastic"
        };
        const QString author[] = {
            "","汪苏泷","周深","张远","队长","TWICE","7mm"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_siteWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_coverWidget
        const QString videoName[] = {
            "","《土坡上的狗尾巴草》卢润泽","我是奶龙，我才是奶龙！#每日一笑","《谁》洋澜一",
            "《苹果香》安安","离别开出花","Empty Love"
        };
        const QString author[] = {
            "","夏娃子","夏娃子","豆豆果","豆豆果","陆鳐LuLu","陆鳐LuLu"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_coverWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_danceWidget
        const QString videoName[] = {
            "","科目三(舞蹈)","LIKE THAT","DRIP",
            "梦","Strategy","Whiplash"
        };
        const QString author[] = {
            "","蛋蛋","BABYMONSTER","BABYMONSTER","时代少年团","TWICE","aespa"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_danceWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_childrenWidget
        const QString videoName[] = {
            "","Buster the Hero Fire Truck Saves the Day","第二季45-船长爸爸","Colors Rap",
            "生日快乐","第三季01-堆肥","奇迹无限"
        };
        const QString author[] = {
            "","Little Baby Nursery Rhyme Friends","Peppa Pig","Little Baby Nursery Rhyme Friends",
            "贝瓦儿歌","Peppa Pig","何洛洛"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_childrenWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_liveWidget
        const QString videoName[] = {
            "","《纯妹妹》单依纯-第五届腾讯音乐娱乐盛典","男儿歌","《璀璨冒险人》周深-第五届腾讯音乐娱乐盛典",
            "《小美满》-第五届腾讯音乐娱乐盛典","醉","第四届TMEA盛典 张杰《少年中国说》"
        };
        const QString author[] = {
            "","单依纯","时代少年团","周深","周深","时代少年团","张杰"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_liveWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_firstConcertWidget
        const QString videoName[] = {
            "","夏天的风","暗里着迷（温柔女生版）","孟婆的碗",
            "迟来的爱","我是你的格桑花","辞九门回忆"
        };
        const QString author[] = {
            "","Uu（刘梦妤）","童珺","魏佳艺","金润吉","欣宝儿","邓寓君(等什么君)"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_firstConcertWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_chineseLanguageWidget
        const QString videoName[] = {
            "","战·永不言败","谭维维新歌MV上线","姑娘别哭泣",
            "无效陪伴","达拉崩吧","壁上观"
        };
        const QString author[] = {
            "","周深","谭维维","懒大王","汪苏泷","周深","张远"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_chineseLanguageWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_southKoreaWidget
        const QString videoName[] = {
            "","APT.","aespa 《Whiplash》MV","Stray Kids \"Walkin On Water\" M/V",
            "\'DRIP\' MV","TOMBOY","Nxde"
        };
        const QString author[] = {
            "","Bruno Mars","aespa","Stray Kids","BABYMONSTER","(G)I-DLE","(G)I-DLE"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_southKoreaWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_japanWidget
        const QString videoName[] = {
            "","不乱不破","おべか","责任集合体(无责任集合体)",
            "アイドル","絶対敵対メチャキライャ-","aespa《Hot Mess》MV"
        };
        const QString author[] = {
            "","HOYO-MiX","すりい","マサラダ","YOASOBI","aespa","Strategy"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_japanWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    cur += 6;
    {
        //m_americanWidget
        const QString videoName[] = {
            "","Wrap Me In Plastic","Uptown Funk","ADDICT",
            "Come Home","Smokin Out The Window","Uptown Funk"
        };
        const QString author[] = {
            "","7mm","Mark Ronson","Hazbin Hotel","Thomas Day","Bruno Mars","Mark Ronson"
        };
        for (int i = 0; i < 2; ++i) {
            for (int j = 1; j <= 3; ++j) {
                auto block = new VideoBlockWidget(this);
                auto idx = i * 3 + j;
                block->setCoverPix(this->m_pixPathVector[idx + cur]);
                block->setVideoName(videoName[idx]);
                block->setAuthor(author[idx]);
                block->setIconPix(this->m_pixPathVector[idx + cur]);
                this->m_americanWidget->addBlockWidget(i,j-1,block);
            }
        }
    }
    //设置ToolButton
    ui->all_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
    ui->all_toolButton->setApproach(true);
    ui->all_toolButton->setIconSize(QSize(10,10));
    ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    //设置title_widget2初始隐藏
    ui->title_widget2->hide();

}

void VideoWidget::on_all_toolButton_toggled() {
    if (ui->all_toolButton->isChecked()) {
        ui->all_toolButton->setText(QStringLiteral("收起"));
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        ui->title_widget2->show();
        offY = 335;
    }
    else {
        ui->all_toolButton->setText(QStringLiteral("全部"));
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->title_widget2->hide();
        offY = 297;
    }
    // 直接更新滚动区域高度
    auto parent = this->window();
    ui->scrollArea->setFixedHeight(parent->height() - offY);

    update(); // 触发重绘
}

void VideoWidget::handleWheelValue(const int &value) {
    if (value >= this->m_recommendWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_videoRankWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->recommend_pushButton->setChecked(true);
    }
    else if (value >= this->m_videoRankWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_MVWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->video_rank_pushButton->setChecked(true);
    }
    else if (value >= this->m_MVWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_siteWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->MV_pushButton->setChecked(true);
    }
    else if (value >= this->m_siteWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_coverWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->site_pushButton->setChecked(true);
    }
    else if (value >= this->m_coverWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_danceWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->cover_pushButton->setChecked(true);
    }
    else if (value >= this->m_danceWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->dance_pushButton->setChecked(true);
    }
    else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_liveWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->children_pushButton->setChecked(true);
    }
    else if (value >= this->m_liveWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_firstConcertWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->live_pushButton->setChecked(true);
    }
    else if (value >= this->m_firstConcertWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_chineseLanguageWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->first_concert_pushButton->setChecked(true);
    }
    else if (value >= this->m_chineseLanguageWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_southKoreaWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->chinese_language_pushButton->setChecked(true);
    }
    else if (value >= this->m_southKoreaWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_japanWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->South_Korea_pushButton->setChecked(true);
    }
    else if (value >= this->m_japanWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_americanWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->Japan_pushButton->setChecked(true);
    }
    else if (value >= this->m_americanWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->American_pushButton->setChecked(true);
    }
}

void VideoWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    auto parent = this->window();
    ui->scrollArea->setFixedHeight(parent->height() - offY);
}

void VideoWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

