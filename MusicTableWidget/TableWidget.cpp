#include "TableWidget.h"

#include<QLabel>
#include<QToolButton>
#include<QTimer>
#include<QEnterEvent>
#include<QBoxLayout>
#include<QSpacerItem>
#include<QStyleOption>
#include<QPainter>
#include<QPainterPath>
#include<QRandomGenerator>
#include<vector>
#include<QFontMetrics>


TableWidget::TableWidget(const QString &title, KIND kind, QWidget *parent)
    : QWidget(parent)
      , m_titleLab(new QLabel(title, this))
      , m_kindList(kind)
      , m_tabHLayout(std::make_unique<QHBoxLayout>())
      , m_gridLayout(std::make_unique<QGridLayout>())
      , m_gridContainer(std::make_unique<QWidget>(this))
      , m_refreshTimer(new QTimer(this))
{
    this->m_play_ToolBtn = new QToolButton(this);
    this->m_adjust_ToolBtn = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_Lab = new QLabel(QStringLiteral("更多 >"), this);

    initDescVector();
    initSongInfo();
    initBlockCover();
    initListCover();
    initUi();

    this->m_adjust_ToolBtn->hide();
    if(kind == KIND::ItemList){
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {emit hideTitle();});
    }
    else if(kind == KIND::BlockList) {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {
            this->m_gridContainer->setHidden(!this->m_gridContainer->isHidden());
        });
    }

    connect(this, &TableWidget::gridChange, this, [this](int len) { onGridChange(len); });

    connect(this->m_refresh_ToolBtn,&QToolButton::clicked,this,&TableWidget::onRefreshBtnClicked);
    connect(this->m_refreshTimer,&QTimer::timeout,this,&TableWidget::onRefreshTimeout);
}

void TableWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TableWidget::enterEvent(QEnterEvent *ev) {
    this->m_adjust_ToolBtn->show();
    this->line1->show();
}

void TableWidget::leaveEvent(QEvent *ev) {
    this->m_adjust_ToolBtn->hide();
    this->line1->hide();
}

void TableWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"this->width : "<<this->width();
    emit gridChange(this->width());
}

void TableWidget::initUi() {

    this->m_play_ToolBtn->setToolTip(QStringLiteral("开始播放"));
    this->m_adjust_ToolBtn->setToolTip(QStringLiteral("隐藏栏目"));
    this->m_refresh_ToolBtn->setToolTip(QStringLiteral("刷新"));

    this->m_play_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_adjust_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_refresh_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_titleLab->setObjectName(QStringLiteral("titleLab"));
    this->m_play_ToolBtn->setObjectName(QStringLiteral("play_ToolBtn"));
    this->m_adjust_ToolBtn->setObjectName(QStringLiteral("adjust_ToolBtn"));
    this->m_refresh_ToolBtn->setObjectName(QStringLiteral("refresh_ToolBtn"));
    this->m_more_Lab->setObjectName(QStringLiteral("moreLab"));

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->m_refreshTimer->setSingleShot(true);

    this->setStyleSheet(R"(QLabel#titleLab{font-size:20px;}
                           QLabel#moreLab{color:gray;font-size:12px;padding-bottom: 3px;}
                           QLabel#moreLab:hover{color:#26a1ff;}
                           QToolButton{background-color:rgba(255,255,255,0);}
                           QToolButton#adjust_ToolBtn{border-image: url(':///Res/tabIcon/adjust-column-gray.svg');}
                           QToolButton#adjust_ToolBtn:hover{border-image: url(':///Res/tabIcon/adjust-column-blue.svg');}
                           QToolButton#refresh_ToolBtn{border-image: url(':///Res/tabIcon/refresh-gray.svg');}
                           QToolButton#refresh_ToolBtn:hover{border-image: url(':///Res/tabIcon/refresh-blue.svg');}
                           QToolButton#play_ToolBtn{border-image: url(':///Res/tabIcon/play2-gray.svg');}
                           QToolButton#play_ToolBtn:hover{border-image: url(':///Res/tabIcon/play2-blue.svg');}
                        )");

    m_tabHLayout->setSpacing(0);
    m_tabHLayout->addWidget(this->m_titleLab);
    m_tabHLayout->addWidget(this->m_play_ToolBtn);
    m_tabHLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
    m_tabHLayout->addWidget(this->m_adjust_ToolBtn);
    this->line1 = new QLabel(this);
    line1->hide();
    line1->setScaledContents(true);
    line1->setFixedSize(20, 15);
    line1->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-gray.svg")));
    m_tabHLayout->addWidget(line1);
    m_tabHLayout->addWidget(this->m_refresh_ToolBtn);
    const auto line2 = new QLabel(this);
    line2->setScaledContents(true);
    line2->setFixedSize(20, 15);
    line2->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-gray.svg")));
    m_tabHLayout->addWidget(line2);
    m_tabHLayout->addWidget(this->m_more_Lab);
    this->m_gridContainer->setAttribute(Qt::WA_OpaquePaintEvent);

    if (this->m_kindList == KIND::ItemList) {
        initItemListWidget();
    }
    else if (this->m_kindList == KIND::BlockList) {
        this->m_play_ToolBtn->hide();
        this->m_more_Lab->setText(QStringLiteral("歌单广场 >"));
        initItemBlockWidget();
    }
}

void TableWidget::initBlockCover() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            this->m_blockCoverPaths.emplace_back(QString(":/Res/tabIcon/music-block-cover%1.jpg").arg(i * 7 + j + 1));
        }
    }
}

void TableWidget::shuffleBlockCover() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_blockCoverPaths.begin(), this->m_blockCoverPaths.end(), std::default_random_engine(seed));
    std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed));
}

void TableWidget::initListCover() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 6; ++j) {
            this->m_listCoverPaths.emplace_back(QString(":/Res/tabIcon/music-list-cover%1.jpg").arg(i * 3 + j + 1));
        }
    }
    shuffleListCover();

}

void TableWidget::shuffleListCover() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_listCoverPaths.begin(), this->m_listCoverPaths.end(), std::default_random_engine(seed));
    std::shuffle(this->m_songInfo.begin(), this->m_songInfo.end(), std::default_random_engine(seed));
}

void TableWidget::initSongInfo() {
    //插入27条数据
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谁"),QStringLiteral("洋澜一")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我的楼兰"),QStringLiteral("云朵")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("白鸽乌鸦相爱的戏码"),QStringLiteral("潘成（皮卡潘）")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("爱错"),QStringLiteral("王力宏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("西海情歌"),QStringLiteral("刀郎")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("有爱就不怕"),QStringLiteral("庄心妍")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("芦苇飞"),QStringLiteral("泡芙芙Scarlett")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不负众望"),QStringLiteral("陈婉菁")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("转身即心痛"),QStringLiteral("吉星出租")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风吹起 (男版)"),QStringLiteral("清唯")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("喀什噶尔胡杨"),QStringLiteral("刀郎")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谦让"),QStringLiteral("王靖雯")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("青花"),QStringLiteral("周传雄")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("时光胶囊"),QStringLiteral("青丝")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("相思遥"),QStringLiteral("玉慧同学")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谁不是过着坎坎坷坷的人生"),QStringLiteral("任夏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不要慌太阳下山有月光"),QStringLiteral("GooGoo")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("卡拉永远OK"),QStringLiteral("谭咏麟")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("春不晚 (女生版)"),QStringLiteral("冰洁")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不说"),QStringLiteral("周公")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("달과 6펜스 (月亮与6便士)"),QStringLiteral("Lucia")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("明明 (深爱着你)"),QStringLiteral("丁肆Dicey")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("赛勒斯的爱"),QStringLiteral("张敬轩")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤独患者"),QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤勇者"),QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("麦恩莉"),QStringLiteral("方大同")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("野孩子"),QStringLiteral("李昊")));
}

void TableWidget::initDescVector() {
    //插入28条描述
    this->m_descVector.emplace_back(QStringLiteral("【欧美】让人一听就上头的欧美歌曲"));
    this->m_descVector.emplace_back(QStringLiteral("适合晚上听的甜甜的歌"));
    this->m_descVector.emplace_back(QStringLiteral("『欧美R＆B』比红唇更加性感致命的旋律"));
    this->m_descVector.emplace_back(QStringLiteral("内耗就一定要听这里"));
    this->m_descVector.emplace_back(QStringLiteral("最怕rapper唱情歌"));
    this->m_descVector.emplace_back(QStringLiteral("体验速度激情，一起来飙车吧"));
    this->m_descVector.emplace_back(QStringLiteral("最舒缓最好听的轻音乐，每天更新"));
    this->m_descVector.emplace_back(QStringLiteral("深受宝宝欢迎的热歌"));
    this->m_descVector.emplace_back(QStringLiteral("南抖音北快手 欢乐相聚在酷狗"));
    this->m_descVector.emplace_back(QStringLiteral("精选新儿歌，节奏欢快，边听边学~陪伴宝宝活泼成长！"));
    this->m_descVector.emplace_back(QStringLiteral("酷音乐亚洲盛典邀你重温经典网络红歌，让记忆回到最美的年华！!"));
    this->m_descVector.emplace_back(QStringLiteral("聆听90后记忆中的声音，希望你喜欢!"));
    this->m_descVector.emplace_back(QStringLiteral("抖腿达人必备DJ神曲，适合夜店/车载/party!"));
    this->m_descVector.emplace_back(QStringLiteral("爱环绕在旋律中"));
    this->m_descVector.emplace_back(QStringLiteral("喜欢的歌 静静听!"));
    this->m_descVector.emplace_back(QStringLiteral("绝绝子R&B精选60首!"));
    this->m_descVector.emplace_back(QStringLiteral("爆燃欧美英文完整版丨听完满血复活的歌曲!"));
    this->m_descVector.emplace_back(QStringLiteral("阿米歌单：防弹咖啡味（个人向）"));
    this->m_descVector.emplace_back(QStringLiteral("一曲入魂，那些自带泪点的OST!"));
    this->m_descVector.emplace_back(QStringLiteral("小声唱我们的歌，属于成长的印记"));
    this->m_descVector.emplace_back(QStringLiteral("千秋如一梦，历史终留名"));
    this->m_descVector.emplace_back(QStringLiteral("民谣弹唱｜献给生活的一首诗"));
    this->m_descVector.emplace_back(QStringLiteral("只待独梅映残雪，不求寒月照旧人"));
    this->m_descVector.emplace_back(QStringLiteral("新一天快乐依旧，轻松节奏继续前进"));
    this->m_descVector.emplace_back(QStringLiteral("想剪下一米阳光，照亮你的世界"));
    this->m_descVector.emplace_back(QStringLiteral("国风新歌：炉向初冬火，温暖庇人心"));
    this->m_descVector.emplace_back(QStringLiteral("清晨暖心民谣，温暖你我的心"));
    this->m_descVector.emplace_back(QStringLiteral("通勤BGM：下班需要一点仪式感!"));
}

void TableWidget::initItemBlockWidget() {
    this->m_gridContainer->setLayout(m_gridLayout.get());
    this->m_gridLayout->setVerticalSpacing(10);
    this->m_gridLayout->setHorizontalSpacing(5);
    this->m_gridLayout->setContentsMargins(0,5,0,5);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto pixPath = this->m_blockCoverPaths[i * 7 + j];
            auto desc = this->m_descVector[i * 7 + j];
            auto block = new ItemBlockWidget(pixPath, desc,this);
            block->hide();
            this->m_gridLayout->addWidget(block, i, j);
        }
    }
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < this->m_showCol; ++j) {
            this->m_gridLayout->itemAtPosition(i, j)->widget()->show();
        }
    }
    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,5,0,5);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addWidget(this->m_gridContainer.get());
}

void TableWidget::initItemListWidget() {
    this->m_gridLayout->setSpacing(10);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto pixPath = this->m_listCoverPaths[i*3+j];
            auto name = this->m_songInfo[i*3+j].first;
            auto author = this->m_songInfo[i*3+j].second;
            auto item = new ItemListWidget(pixPath, name, author, this);
            this->m_gridLayout->addWidget(item, i, j);
        }
    }

    auto vlayout = new QVBoxLayout(this);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addLayout(this->m_gridLayout.get());
}

void TableWidget::onGridChange(int len) {
    this->m_hide_col_6 = true;
    this->m_hide_col_7 = true;
    auto item = this->m_gridLayout->itemAtPosition(0, 5);
    if (item) {
        this->m_hide_col_6 = item->widget()->isHidden();
        //qDebug()<<"hide_col_6 : "<<hide_col_6;
    }
    item = this->m_gridLayout->itemAtPosition(0, 6);
    if (item) {
        this->m_hide_col_7 = item->widget()->isHidden();
        //qDebug()<<"hide_col_7 : "<<hide_col_7;
    }

    if (this->m_hide_col_6) {
        //显示前5列
        //if(this->m_gridLayout->columnCount() == 7){
        this->m_showCol = 5;
        if (len < 1000)return;
        if (len < 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if(item)item->widget()->show();
                //qDebug()<<"show一个";
            }
            this->m_showCol = 6;
            //qDebug()<<"//////////////////////////";
        }
        //由于直接全屏需要一定时间，因此不存在从5列直接变成7列的情况
        //但是，上面说的是正常缩放的情况，但如果存在突然发送的信号的情况。。。
        else {
            this->m_showCol = 7;
        }
    }
    else if (!this->m_hide_col_6 && this->m_hide_col_7) {
        //显示前6列
        this->m_showCol = 6;
        if (len < 1000) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if(item)item->widget()->hide();
            }
            this->m_showCol = 5;
        } else if (len >= 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if(item)item->widget()->show();
            }
            this->m_showCol = 7;
        }
    }
    else if (!this->m_hide_col_7) {
        this->m_showCol = 7;
        //显示前7列
        if(len < 1000) {
            this->m_showCol = 5;
        }
        else if(len < 1200){
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if(item)item->widget()->hide();
            }
            this->m_showCol = 6;
            //qDebug()<<"********************";
        }
    }
}

void TableWidget::onRefreshBtnClicked() {
    // 启动定时器，延迟处理
    if (!this->m_refreshTimer->isActive()) {
        this->m_refreshTimer->start(200);  // 200ms 延迟，避免过于频繁地触发
    }
}

void TableWidget::onRefreshTimeout() {
    if(this->m_kindList == KIND::BlockList) {
        shuffleBlockCover();
        this->m_gridContainer->setUpdatesEnabled(false);
        this->m_gridContainer->hide(); // 隐藏容器
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 7; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i,j);
                if(item) {
                    auto widget = item->widget();
                    if(widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                auto pixPath = this->m_blockCoverPaths[i * 7 + j];
                auto desc = this->m_descVector[i * 7 + j];
                auto block = new ItemBlockWidget(pixPath, desc,this);
                block->hide();
                this->m_gridLayout->addWidget(block, i, j);
            }
        }
        emit gridChange(this->width());
        //qDebug()<<"当前显示 ： "<<this->m_showCol;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < this->m_showCol; ++j) {
                this->m_gridLayout->itemAtPosition(i, j)->widget()->show();
            }
        }
        this->m_gridContainer->setUpdatesEnabled(true);
        this->m_gridContainer->show(); // 显示容器
    }
    else {
        shuffleListCover();
        this->m_gridContainer->setUpdatesEnabled(false);
        this->m_gridContainer->hide(); // 隐藏容器
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i,j);
                if(item) {
                    auto widget = item->widget();
                    if(widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                QString pixPath = this->m_listCoverPaths[i * 3 + j];
                auto name = this->m_songInfo[i*3+j].first;
                auto author = this->m_songInfo[i*3+j].second;
                auto list = new ItemListWidget(pixPath, name, author, this);
                this->m_gridLayout->addWidget(list, i, j);
            }
        }
        this->m_gridContainer->setUpdatesEnabled(true);
        this->m_gridContainer->show(); // 显示容器
    }
}

QPixmap roundedPixmap(QPixmap &src, QSize size, int radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);
    return dest;
}

ItemListWidget::ItemListWidget(QPixmap coverPix, const QString &name, const QString &author, QWidget *parent)
    : QWidget(parent)
      , m_mask(std::make_unique<SMaskWidget>(this))
      , m_coverLab(new QLabel(this))
      , m_nameLab(new QLabel(this))
      , m_authorLab(new QLabel(this))
      , m_play_add_ToolBtn(new QToolButton(this))
      , m_like_ToolBtn(new QToolButton(this))
      , m_more_ToolBtn(new QToolButton(this))
      , m_songName(name)
      , m_singer(author)
{
    this->setFixedHeight(65);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_coverLab->setPixmap(roundedPixmap(coverPix, this->m_coverLab->size(), 8));
    this->m_coverLab->setScaledContents(true);
    this->m_coverLab->setStyleSheet(R"(margin:0px;padding:0px;background-color: rgba(255, 0, 0, 0);)");

    initUi();
    this->m_mask->move(this->m_coverLab->pos());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    this->m_mask->hide();
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();

    connect(qobject_cast<TableWidget *>(parent), &TableWidget::hideTitle, this, &ItemListWidget::onHide);
}

void ItemListWidget::setNameText(QString name) {
    const QFont font("楷体",this->m_coverLab->height()/4);
    // 应用字体到标签
    this->m_nameLab->setFont(font);
    //设置字体测量工具
    QFontMetrics fm(font);
    int maxWidth = this->m_nameLab->width() * 3 / 4;
    //设置tip
    this->m_nameLab->setToolTip(name);
    name = fm.elidedText(name,Qt::ElideRight,maxWidth);
    this->m_nameLab->setText(name);
}

void ItemListWidget::setAuthorText(QString author) {
    //qDebug()<<"font size : "<<this->m_coverLab->height()/4;
    const QFont font(QStringLiteral("楷体"),this->m_coverLab->height()/4-2);
    // 应用字体到标签
    this->m_authorLab->setFont(font);
    //设置字体测量工具
    QFontMetrics fm(font);
    int maxWidth = this->m_authorLab->width() * 3 / 4;
    //设置Tip
    this->m_authorLab->setToolTip(author);
    author = fm.elidedText(author,Qt::ElideRight,maxWidth);
    this->m_authorLab->setText(author);
}

void ItemListWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
    } else {
        this->m_mask->hide();
    }
}

void ItemListWidget::enterEvent(QEnterEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    this->m_isHoverCoverLab = true;
    this->m_play_add_ToolBtn->show();
    this->m_like_ToolBtn->show();
    this->m_more_ToolBtn->show();
    this->setStyleSheet(R"(QWidget{border-radius:8px;background-color:#e8eafb;}
                           QLabel{background-color:#e8eafb;}
                           QLabel:hover{color:#2291e6;})");
    update();
}

void ItemListWidget::leaveEvent(QEvent *ev) {
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
    this->setStyleSheet(R"(QWidget{border-radius:8px;})");
    update();
}

void ItemListWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / this->m_aspectRatio);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_mask->setFixedSize(this->m_coverLab->size());

    //设置文字
    this->setNameText(this->m_songName);
    this->setAuthorText(this->m_singer);
    update();
}

void ItemListWidget::initUi() {
    this->m_coverLab->setCursor(Qt::PointingHandCursor);
    this->m_nameLab->setCursor(Qt::PointingHandCursor);
    this->m_authorLab->setCursor(Qt::PointingHandCursor);
    this->m_play_add_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_like_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_more_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_coverLab->setObjectName(QStringLiteral("coverLab"));
    this->m_nameLab->setObjectName(QStringLiteral("nameLab"));
    this->m_authorLab->setObjectName(QStringLiteral("authorLab"));
    this->m_play_add_ToolBtn->setObjectName(QStringLiteral("play_add_ToolBtn"));
    this->m_like_ToolBtn->setObjectName(QStringLiteral("like_ToolBtn"));
    this->m_more_ToolBtn->setObjectName(QStringLiteral("more_ToolBtn"));


    this->m_play_add_ToolBtn->setStyleSheet(
        R"(QToolButton#play_add_ToolBtn{border-image: url(':///Res/tabIcon/play-add-gray.svg');}
                                               QToolButton#play_add_ToolBtn:hover{border-image: url(':///Res/tabIcon/play-add-blue.svg');})");
    this->m_like_ToolBtn->setStyleSheet(R"(QToolButton#like_ToolBtn{border-image: url(':///Res/playbar/collect.svg')}
                                           QToolButton#like_ToolBtn:hover{border-image: url(':///Res/tabIcon/collect-blue.svg');})");
    this->m_more_ToolBtn->setStyleSheet(R"(QToolButton#more_ToolBtn{border-image: url(':///Res/playbar/more.svg');}
                                           QToolButton#more_ToolBtn:hover{border-image: url(':///Res/playbar/more-blue.svg');})");

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_authorLab);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 10, 0);
    hlayout->addWidget(this->m_coverLab);

    hlayout->addLayout(vlayout);

    hlayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Preferred));
    hlayout->addWidget(this->m_play_add_ToolBtn);
    hlayout->addWidget(this->m_like_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);
}

void ItemListWidget::onHide() {
    this->setHidden(!this->isHidden());
    update();
}

#define DescribeLabHeight 25
std::vector<QString> tipArr =
    {QStringLiteral("日语"),QStringLiteral("经典"),
    QStringLiteral("国语"), QStringLiteral("流行"),
    QStringLiteral("兴奋"), QStringLiteral("英语"),
    QStringLiteral("古风"), QStringLiteral("韩语"),
    QStringLiteral("寂寞"), QStringLiteral("运动"),
    QStringLiteral("说唱"), QStringLiteral("校园") };

ItemBlockWidget::ItemBlockWidget(const QString &path,const QString& desc, QWidget *parent)
    : QWidget(parent)
      , m_bacWidget(new QWidget(this))
      , m_mask(std::make_unique<SMaskWidget>(this))
      , m_describeLab(new QLabel(this))
      , m_tipLab(new QLabel(this))
      , m_popularBtn(new QToolButton(this))
      , m_descText(desc)
{
    QString style = QString("border-radius:8px;border-image:url(%1);").arg(path);
    this->m_bacWidget->setStyleSheet(style);
    this->setFixedHeight(150 + DescribeLabHeight);
    initUi();
    this->m_mask->setParent(this->m_bacWidget);
    this->m_mask->move(this->m_bacWidget->pos());
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_mask->hide();
}

void ItemBlockWidget::setTipLabText(const QString &text) {
    this->m_tipLab->setText(text);
}

void ItemBlockWidget::setPopularBtnText(const QString &text) {
    this->m_popularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
}

void ItemBlockWidget::setDescribeText(QString desc) {
    const QFont font(QStringLiteral("楷体"),DescribeLabHeight-14);
    // 应用字体到标签
    this->m_describeLab->setFont(font);
    //设置字体测量工具
    QFontMetrics fm(font);
    //设置Tip
    this->m_describeLab->setToolTip(desc);
    desc = fm.elidedText(desc,Qt::ElideRight,this->m_describeLab->width());
    this->m_describeLab->setText(desc);
}

void ItemBlockWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);"));
    } else {
        this->m_mask->hide();
    }
}

void ItemBlockWidget::enterEvent(QEnterEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    this->m_isHoverCoverLab = true;
    update();
}

void ItemBlockWidget::leaveEvent(QEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    update();
}

void ItemBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"改变大小 : "<<event->size();

    this->setFixedHeight(event->size().width() + DescribeLabHeight);
    //只要留了一定的空间，就能缩小
    this->m_bacWidget->setFixedSize(event->size().width() / 1.05, event->size().width() / 1.05);
    //this->m_bacWidget->setFixedSize(event->size().width(),event->size().width());
    //qDebug()<<"this->m_bacWidget->size() : "<<this->m_bacWidget->size();
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_describeLab->setFixedSize(this->m_bacWidget->width(),DescribeLabHeight);
    this->m_popularBtn->move(this->m_bacWidget->width() - this->m_popularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_popularBtn->height() - 5);
    setDescribeText(this->m_descText);
    update();
}

void ItemBlockWidget::initUi() {
    this->setCursor(Qt::PointingHandCursor);
    this->m_describeLab->setObjectName(QStringLiteral("describeLab"));
    this->m_tipLab->setObjectName(QStringLiteral("tipLab"));
    this->m_popularBtn->setObjectName(QStringLiteral("popularLab"));

    this->m_tipLab->setFixedSize(50, 20);
    this->setTipLabText(tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(tipArr.size()))]);
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;"));
    this->m_tipLab->move(6, 6);

    this->m_popularBtn->setFixedSize(70, 20);
    this->m_popularBtn->setIcon(QIcon(QStringLiteral("://Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_popularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_popularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
    //this->m_popularBtn->move(this->width()-this->m_popularBtn->height()-10,this->m_bacWidget->height()-this->m_popularBtn->height()-10);
    //qDebug()<<"this->m_bacWidget->width() : "<<this->m_bacWidget->width()<<"\nthis->width() : "<<this->width();

    this->m_describeLab->setText(QStringLiteral("游戏主播常用音乐"));
    this->m_describeLab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(this->m_bacWidget);
    vlayout->addWidget(this->m_describeLab);
}

