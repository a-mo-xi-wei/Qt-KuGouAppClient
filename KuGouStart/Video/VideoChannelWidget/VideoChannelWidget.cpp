//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoChannelWidget.h" resolved

#include "VideoChannelWidget.h"
#include "ui_VideoChannelWidget.h"

#include<QFile>
#include<QButtonGroup>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>
#include <random>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

VideoChannelWidget::VideoChannelWidget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::VideoChannelWidget)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
      , m_upBtn(std::make_unique<UpToolButton>(this))
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
      , m_scrollBarTimer(new QTimer(this)) {
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channelwidget.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initButtonGroup();
    initTotalWidget();
    initVector();
    initUi();
}

VideoChannelWidget::~VideoChannelWidget() {
    delete ui;
}

void VideoChannelWidget::initButtonGroup()const {
    this->m_buttonGroup->addButton(ui->popular_pushButton);
    this->m_buttonGroup->addButton(ui->children_pushButton);
    this->m_buttonGroup->addButton(ui->theme_pushButton);
    this->m_buttonGroup->addButton(ui->film_pushButton);
    this->m_buttonGroup->addButton(ui->variety_pushButton);
    this->m_buttonGroup->addButton(ui->ACGN_pushButton);
    this->m_buttonGroup->addButton(ui->scene_pushButton);
    this->m_buttonGroup->addButton(ui->language_pushButton);
    this->m_buttonGroup->addButton(ui->dance_pushButton);
    this->m_buttonGroup->addButton(ui->site_pushButton);
    this->m_buttonGroup->addButton(ui->singer_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void VideoChannelWidget::initTotalWidget()const {
    this->m_popularWidget->setTitleName("热门");
    this->m_childrenWidget->setTitleName("儿童");
    this->m_themeWidget->setTitleName("主题");
    this->m_filmWidget->setTitleName("影视");
    this->m_varietyWidget->setTitleName("综艺");
    this->m_ACGNWidget->setTitleName("二次元");
    this->m_sceneWidget->setTitleName("场景");
    this->m_languageWidget->setTitleName("语言");
    this->m_danceWidget->setTitleName("舞蹈");
    this->m_siteWidget->setTitleName("现场");
    this->m_singerWidget->setTitleName("歌手");
}

void VideoChannelWidget::initUi() {
    {
        //加入布局
        auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
        lay->setSpacing(0);
        if (!lay) {
            qWarning() << "布局不存在";
            return;
        }
        lay->insertWidget(lay->count() - 1, m_popularWidget.get());
        lay->insertWidget(lay->count() - 1, m_childrenWidget.get());
        lay->insertWidget(lay->count() - 1, m_themeWidget.get());
        lay->insertWidget(lay->count() - 1, m_filmWidget.get());
        lay->insertWidget(lay->count() - 1, m_varietyWidget.get());
        lay->insertWidget(lay->count() - 1, m_ACGNWidget.get());
        lay->insertWidget(lay->count() - 1, m_sceneWidget.get());
        lay->insertWidget(lay->count() - 1, m_languageWidget.get());
        lay->insertWidget(lay->count() - 1, m_danceWidget.get());
        lay->insertWidget(lay->count() - 1, m_siteWidget.get());
        lay->insertWidget(lay->count() - 1, m_singerWidget.get());
    }
    {
        this->m_vScrollBar = ui->scrollArea->verticalScrollBar();
        ui->scrollArea->setScrollAreaKind(MyScrollArea::ScrollAreaKind::Inside);
        //处理信号
        connect(ui->popular_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_popularWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->children_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_childrenWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->theme_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_themeWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->film_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_filmWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->variety_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_varietyWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->ACGN_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_ACGNWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->scene_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_sceneWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->language_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_languageWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->dance_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_danceWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->site_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_siteWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->singer_pushButton,&QPushButton::clicked,this,[this] {
            this->m_vScrollBar->setValue(this->m_singerWidget->mapToParent(QPoint(0, 0)).y());
        });
        //wheelVaue信号
        connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &VideoChannelWidget::handleWheelValue);
        connect(this->m_vScrollBar,&QScrollBar::valueChanged,this, &VideoChannelWidget::handleWheelValue);
        //回到最顶部信号
        connect(this->m_upBtn.get(), &QToolButton::clicked, this, &VideoChannelWidget::onUpBtnClicked);
        connect(this->m_scrollBarTimer, &QTimer::timeout, this, &VideoChannelWidget::onUpBtnShowOrNot);
    }
    auto cur = 0;
    {
        //m_popularWidget 10 个
        const QString coverText[] = {
            "","腾讯音乐盛典","网络红歌","每周推荐","伤感网络情歌","最热歌曲",
            "年度电影金曲","武侠剧","宫廷剧","酷狗热歌","偶像剧"
        };
        for (int i = 1; i <= 10; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i]);
            block->setCoverText(coverText[i]);
            this->m_popularWidget->addBlockWidget(block);
        }
    }
    cur += 10;
    {
        //m_childrenWidget 14 个
        const QString coverText[] = {
            "","宝宝巴士","贝贝虎","启蒙故事","幼儿园","动画片",
            "启蒙音乐剧","幼儿唐诗","迪士尼","男孩最爱","少儿才艺",
            "水果冰激凌","歌声的翅膀","女孩最爱","麦杰克小镇"
        };
        for (int i = 1; i <= 14; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_childrenWidget->addBlockWidget(block);
        }
    }
    cur += 14;
    {
        //m_themeWidget 10 个
        const QString coverText[] = {
            "","网络红歌","老情歌","电音","伤感网络情歌","单曲循环音乐",
            "酷狗热歌","新歌","每周推荐","音乐人","燃爆歌曲"
        };
        for (int i = 1; i <= 10; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_themeWidget->addBlockWidget(block);
        }
    }
    cur += 10;
    {
        //m_filmWidget 7 个
        const QString coverText[] = {
            "","年度电影金曲","最热影视歌曲","武侠剧","宫廷剧","偶像剧",
            "校园剧","韩剧"
        };
        for (int i = 1; i <= 7; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_filmWidget->addBlockWidget(block);
        }
    }
    cur += 7;
    {
        //m_varietyWidget 1 个
        const QString coverText[] = {
            "","炙热的我们"
        };
        for (int i = 1; i <= 1; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_varietyWidget->addBlockWidget(block);
        }
    }
    cur += 1;
    {
        //m_ACGNWidget 6 个
        const QString coverText[] = {
            "","国漫","动漫","王者荣耀","古风ACG","超福利宅舞",
            "游戏"
        };
        for (int i = 1; i <= 6; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_ACGNWidget->addBlockWidget(block);
        }
    }
    cur += 6;
    {
        //m_sceneWidget 3 个
        const QString coverText[] = {
            "","中文DJ","派对","清吧"
        };
        for (int i = 1; i <= 3; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_sceneWidget->addBlockWidget(block);
        }
    }
    cur += 3;
    {
        //m_languageWidget 9 个
        const QString coverText[] = {
            "","怀旧国语","怀旧粤语","欧美","国语","粤语",
            "国语流行","粤语流行","闽南语","日语"
        };
        for (int i = 1; i <= 9; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_languageWidget->addBlockWidget(block);
        }
    }
    cur += 9;
    {
        //m_danceWidget 3 个
        const QString coverText[] = {
            "","广场舞","舞蹈达人","热舞"
        };
        for (int i = 1; i <= 3; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_danceWidget->addBlockWidget(block);
        }
    }
    cur += 3;
    {
        //m_siteWidget 14 个
        const QString coverText[] = {
            "","腾讯音乐庆典","演唱会","网路视听盛典","国风经典","韩国音乐现场",
            "央视网络春晚","声家班","欧美音乐现场","韩国歌谣盛典","蘑菇音乐节",
            "乐人现场","翻唱现场","Live现场","摇滚现场"
        };
        for (int i = 1; i <= 14; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_siteWidget->addBlockWidget(block);
        }
    }
    cur += 14;
    {
        //m_singerWidget 26 个
        const QString coverText[] = {
            "","周深","薛之谦","BLACKPINK","TaylorSwift","叶洛洛",
            "SM家族","AKB48 GROUP","TFBOYS","凤凰传奇","刘德华",
            "AlanWalker","EXO","艾薇儿","SING女团","少女时代",
            "鹿晗","五月天","防弹少年团","BIGBANG","邓紫棋",
            "洛天依","庄心妍","周杰伦","陈奕迅","张杰","张国荣"
        };
        for (int i = 1; i <= 26; ++i) {
            auto block = new VideoChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i + cur]);
            block->setCoverText(coverText[i]);
            this->m_singerWidget->addBlockWidget(block);
        }
    }

}

void VideoChannelWidget::initVector() {
    for (int i = 1; i <= 120; ++i) {
        this->m_pixPathVector.emplace_back(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i));
    }
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed));
}

void VideoChannelWidget::handleWheelValue(const int &value) {
    if (value >= this->m_popularWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->popular_pushButton->setChecked(true);
    } else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_themeWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->children_pushButton->setChecked(true);
    } else if (value >= this->m_themeWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_filmWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->theme_pushButton->setChecked(true);
    } else if (value >= this->m_filmWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_varietyWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->film_pushButton->setChecked(true);
    } else if (value >= this->m_varietyWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_ACGNWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->variety_pushButton->setChecked(true);
    } else if (value >= this->m_ACGNWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_sceneWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->ACGN_pushButton->setChecked(true);
    } else if (value >= this->m_sceneWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_languageWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->scene_pushButton->setChecked(true);
    } else if (value >= this->m_languageWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_danceWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->language_pushButton->setChecked(true);
    } else if (value >= this->m_danceWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_siteWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->dance_pushButton->setChecked(true);
    } else if (value >= this->m_siteWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_singerWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->site_pushButton->setChecked(true);
    } else if (value >= this->m_singerWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->singer_pushButton->setChecked(true);
    }
    // 启动定时器，延迟处理
    if (!this->m_scrollBarTimer->isActive()) {
        this->m_scrollBarTimer->start(500); // 500ms 延迟，避免过于频繁地触发
    }
}

void VideoChannelWidget::onUpBtnClicked() {
    // 标记动画开始
    ui->scrollArea->setAnimating(true); //开始禁用滚轮

    auto animation = new QPropertyAnimation(this->m_vScrollBar, "value", this);
    // 设置动画的起始值（当前滚动条位置）和结束值（最顶部）
    animation->setStartValue(this->m_vScrollBar->value()); // 当前滚动条位置
    animation->setEndValue(0); // 滚动到顶部（0 表示最上方）
    animation->setDuration(500); // 动画持续时间，500ms
    animation->setEasingCurve(QEasingCurve::OutBounce); // 缓动曲线

    // 在动画结束后标记动画停止
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        ui->scrollArea->setAnimating(false); //动画结束启用滚轮
    });

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped); // 动画结束后自动删除
}

void VideoChannelWidget::onUpBtnShowOrNot() {
    //qDebug()<<this->m_vScrollBar->value();
    if (this->m_vScrollBar->value() > 200)this->m_upBtn->show();
    else this->m_upBtn->hide();
}

void VideoChannelWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    auto parent = this->window();
    ui->scrollArea->setFixedHeight(parent->height() - 280);
    //UpWidget移动
    this->m_upBtn->move(parent->width() - this->m_upBtn->width() - 206,
                        parent->height() - this->m_upBtn->height() - 250);
    this->m_upBtn->raise();
}

void VideoChannelWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}
