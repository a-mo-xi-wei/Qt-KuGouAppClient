#include "TableWidget.h"

#include <QLabel>
#include <QToolButton>
#include <QTimer>
#include <QEnterEvent>
#include <QFile>
#include <QSpacerItem>
#include <QStyleOption>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QFontMetrics>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

TableWidget::TableWidget(const QString &title, KIND kind, QWidget *parent)
    : QWidget(parent)
      , m_titleLab(new QLabel(title, this))
      , m_kind(kind)
      , m_tabHLayout(std::make_unique<QHBoxLayout>())
      , m_gridLayout(std::make_unique<QGridLayout>())
      , m_gridContainer(std::make_unique<QWidget>(this))
      , m_refreshTimer(new QTimer(this)) {
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
    if (kind == KIND::ItemList) {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] { emit hideTitle(); });
    } else if (kind == KIND::BlockList) {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {
            this->m_gridContainer->setHidden(!this->m_gridContainer->isHidden());
        });
    }

    connect(this, &TableWidget::gridChange, this, [this](const int &len) { onGridChange(len); });

    connect(this->m_refresh_ToolBtn, &QToolButton::clicked, this, &TableWidget::onRefreshBtnClicked);
    connect(this->m_refreshTimer, &QTimer::timeout, this, &TableWidget::onRefreshTimeout);
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
    if (this->m_kind == KIND::ItemList) {
        this->m_topWindow = this->window();
        if (!m_topWindow) {
            qWarning() << "无法获取顶级窗口！";
            return;
        }
        int topLevelWidth = m_topWindow->width();
        auto average = (topLevelWidth - 255) / 3;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                this->m_gridLayout->itemAtPosition(i, j)->widget()->setFixedWidth(average);
            }
        }
    }
}

void TableWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void TableWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void TableWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void TableWidget::initUi() {
    this->m_play_ToolBtn->setToolTip(QStringLiteral("开始播放"));
    this->m_adjust_ToolBtn->setToolTip(QStringLiteral("隐藏栏目"));
    this->m_refresh_ToolBtn->setToolTip(QStringLiteral("刷新"));

    this->m_play_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_adjust_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_refresh_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_more_Lab->setCursor(Qt::PointingHandCursor);

    this->m_titleLab->setObjectName(QStringLiteral("titleLab"));
    this->m_play_ToolBtn->setObjectName(QStringLiteral("play_ToolBtn"));
    this->m_adjust_ToolBtn->setObjectName(QStringLiteral("adjust_ToolBtn"));
    this->m_refresh_ToolBtn->setObjectName(QStringLiteral("refresh_ToolBtn"));
    this->m_more_Lab->setObjectName(QStringLiteral("moreLab"));

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->m_refreshTimer->setSingleShot(true);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/tab.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
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

    if (this->m_kind == KIND::ItemList) {
        initItemListWidget();
    } else if (this->m_kind == KIND::BlockList) {
        this->m_play_ToolBtn->hide();
        this->m_more_Lab->setText(QStringLiteral("歌单广场 >"));
        initItemBlockWidget();
    }
}

void TableWidget::initBlockCover() {
    for (int i = 1; i <= 60; ++i) {
        this->m_blockCoverPaths.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i));
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
    for (int i = 1; i <= 60; ++i) {
        this->m_listCoverPaths.emplace_back(QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(i));
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
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("先说爱的人为什么先离开"),QStringLiteral("张宇佳")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("白鸽乌鸦相爱的戏码"),QStringLiteral("七喜")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我知道 (我知道你还是爱着我)"),QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("亲爱的, 那不是爱情"),QStringLiteral("en")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("알려줄게 (Feat. 설지안)"),QStringLiteral("Jason &amp; 38SUN、설지안")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("归途的光"),QStringLiteral("诺爷Plus")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风经过 (烟嗓女版)"),QStringLiteral("黄静美")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Под луной (在月亮下)(Silver Ace remix)"),QStringLiteral("SUNAMI")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("只要你还需要我"),QStringLiteral("半吨兄弟")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("错位时空"),QStringLiteral("海边的浪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("全世界只爱你 (无论我走到哪里)"),QStringLiteral("KEY.Y杨思豪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("来我身边，给你所有安全感"),QStringLiteral("半吨儿")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("唱月"),QStringLiteral("李耀阳")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Hate Me (remix)"),QStringLiteral("TaIsil")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("假如我不再善良"),QStringLiteral("歌手玫玫")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("与天空比个耶 (父子版)"),QStringLiteral("袁大脑袋爱嘟嘟<")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("放纵L"),QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("ありがとう・・・ (谢谢…)"),QStringLiteral("KOKIA")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("一切都是我太过骄纵 (这份爱没停过)"),QStringLiteral("zyy")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Yesterday Once More (新版)"),QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤独患者"),QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("土坡上的狗尾草"),QStringLiteral("卢润泽")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("APT."),QStringLiteral("ROSÉ、Bruno Mars")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Cry For Me (feat. Ami)"),QStringLiteral("Michita、Ami")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("凑热闹"),QStringLiteral("BY2")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("有爱就不怕"),QStringLiteral("庄心妍")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风经过"),QStringLiteral("任夏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("情罪"),QStringLiteral("陈瑞")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我知道"),QStringLiteral("BY2")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("青丝"),QStringLiteral("时光胶囊")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("望故乡"),QStringLiteral("文夫")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("归途的光"),QStringLiteral("诺爷Plus")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("小孩"),QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"),QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("红昭愿 (高音质版)"),QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"),QStringLiteral("王梓钰")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("相思遥"),QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"),QStringLiteral("玉慧同学")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("小孩 (反盗版)"),QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("换个方向风景会更好看"),QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"),QStringLiteral("杨一琪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("才二十三"),QStringLiteral("方大同")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("当你看着我的时候1.3x"),QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Lifestyle"),QStringLiteral("HMHK")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Fantasy"),QStringLiteral("Nova Aurora")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("时代进行曲 (Original Mix)"),QStringLiteral("VZEUS")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("梦里花 (唯一纯白的茉莉花)(DJ版)"),QStringLiteral("DJ阿智、yiyi林淑怡")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("画心 (猜不透是你瞳孔的颜色)"),QStringLiteral("于春洋")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("只你"),QStringLiteral("颜人中")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("千年 (DJ版)"),QStringLiteral("DJHZ")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("爱错"),QStringLiteral("en")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("执子之手"),QStringLiteral("宝石Gem、一哩哩一")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("regret (我在未来等你)"),QStringLiteral("Chypy！")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Blame You"),QStringLiteral("E27XG_")));
}

void TableWidget::initDescVector() {
    //插入至少28条描述
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
    this->m_descVector.emplace_back(QStringLiteral("清晨的泥土芳香扑鼻而来，深呼大自然的气息，安静的欣赏花草树木换装后新装，尽情享受这美妙的春光。"));
    this->m_descVector.emplace_back(QStringLiteral("爱环绕在旋律中"));
    this->m_descVector.emplace_back(QStringLiteral("狂暴硬核，一起抖腿！"));
    this->m_descVector.emplace_back(QStringLiteral("最新一期最火爆的中文DJ推荐，尽管嗨起来吧！"));
    this->m_descVector.emplace_back(QStringLiteral("南抖音北快手 欢乐相聚在酷狗"));
    this->m_descVector.emplace_back(QStringLiteral("精选新儿歌，节奏欢快，边听边学~陪伴宝宝活泼成长！（最适合3-6岁宝宝收听）"));
    this->m_descVector.emplace_back(QStringLiteral("选取近期网络流行热歌，欢迎收藏"));
    this->m_descVector.emplace_back(QStringLiteral("本歌单收录抖音上的热门英文歌曲，一网打尽，应有尽有！"));
    this->m_descVector.emplace_back(QStringLiteral("抖腿达人必备DJ神曲，适合夜店/车载/party"));
    this->m_descVector.emplace_back(QStringLiteral("滚烫人生，披荆斩棘"));
    this->m_descVector.emplace_back(QStringLiteral("丹心赋：望九州风月，祈万世长安"));
    this->m_descVector.emplace_back(QStringLiteral("蛰龙已惊眠，凤凰已展翅，这繁华盛世，终会如君所愿"));
    this->m_descVector.emplace_back(QStringLiteral("如果经常听欧美歌的“童鞋”，播放列表里肯定多多少少会存在像比伯、泰勒、碧莉这样大牌欧美明星的单曲，但是！你知道有关于他们之前的故事吗？那就让我们揭开那些“不为人知”的秘密吧！"));
    this->m_descVector.emplace_back(QStringLiteral("国创动画，经历了时代的洗涤，经历了人心的历练，满满的走向光明"));
    this->m_descVector.emplace_back(QStringLiteral("难得让自己能够放松的时刻，可能是在下班的地铁公交上，上班行走的路上，这个时候就需要给自己戴上耳机放一些歌曲，享受片刻；有音乐相伴，上下班的路上也不会孤单。"));
    this->m_descVector.emplace_back(QStringLiteral("一壶酒，一卷书，半世逍遥半日闲"));
    this->m_descVector.emplace_back(QStringLiteral("戴上耳机，一起聆听的歌曲中的“泪点"));
    this->m_descVector.emplace_back(QStringLiteral("人生不羡君临天下，唯羡世间天地客；人生不慕红尘雨，唯慕人间诗酒茶花；人生不爱高墙院，唯爱故城杨柳溪。"));
    this->m_descVector.emplace_back(QStringLiteral("古灵精怪，俏皮可爱，可是不分男女的呢~你说是不是？"));
    this->m_descVector.emplace_back(QStringLiteral("收录各大中文V家歌曲，一同赏鉴精灵般的歌姬轻吟"));
    this->m_descVector.emplace_back(QStringLiteral("跑步的过程漫长而枯燥，而且动作比较单一，所以适合用音乐来分散自己的注意力。"));
    this->m_descVector.emplace_back(QStringLiteral("希望今年冬天递给你的第一份国风歌单是暖心的、温柔的。"));
    this->m_descVector.emplace_back(QStringLiteral("随时随地可以进行的手游已经成为了国民不可或缺的娱乐方式和社交媒介，一起欣赏那些国民手游里的动听旋律吧。"));
    this->m_descVector.emplace_back(QStringLiteral("华夏是礼仪之邦，当谈起时，似乎总有说不尽的故事，说不尽的惊艳，有多少一鸣惊鸿？"));
    this->m_descVector.emplace_back(QStringLiteral("国创动漫的崛起，让优质小说的角色和桥段被像素点拼凑在屏幕中央，当文字和动漫碰撞，又会擦出怎样的音乐火花呢？"));
    this->m_descVector.emplace_back(QStringLiteral("平凡坚持到底就是伟大。奔跑吧打工人，向阳而生，完成蜕变，总有一天，你会感激每一个努力奋斗的日子。"));
    this->m_descVector.emplace_back(QStringLiteral("王者荣耀已经陪伴广大玩家走过五年，不仅成为了国民手游，也成为了新型的社交媒介和传统文化的弘扬者，让我们一同走进王者荣耀的音乐世界。"));
    this->m_descVector.emplace_back(QStringLiteral("国漫正在崛起，一同赏鉴近两年的优质国漫歌曲。"));
    this->m_descVector.emplace_back(QStringLiteral("网络中国节·中秋精选曲，佳“音”相伴，中秋赏“乐"));
    this->m_descVector.emplace_back(QStringLiteral("用音乐预热召唤师峡谷，英雄联盟高燃歌曲合辑"));
    this->m_descVector.emplace_back(QStringLiteral("2024中文DJ热曲推荐，继续嗨翻生活，用DJ打败一切丧气，奥力给！！！"));
    this->m_descVector.emplace_back(QStringLiteral("每周一定时更新，抖音人手一份精选热播BGM，你值得拥有~！"));
    this->m_descVector.emplace_back(QStringLiteral("精挑细选的好听中文歌，时光的流水没有将它们冲淡，反倒把它们洗刷得更加纯粹，更能令你感动。"));
    this->m_descVector.emplace_back(QStringLiteral("让走心的歌陪你在夜里哭泣！"));
    this->m_descVector.emplace_back(QStringLiteral("最具人气的DJ热歌，跟随音乐一起摇摆！"));
    this->m_descVector.emplace_back(QStringLiteral("精选2024抖音歌曲最火国风歌曲"));
    this->m_descVector.emplace_back(QStringLiteral("曾经唱响我们的街头巷尾的网络红歌，听了就有点停不下来了，有毒！"));
    this->m_descVector.emplace_back(QStringLiteral("每首歌都像星星，在星球上拥有自己的位置"));
    this->m_descVector.emplace_back(QStringLiteral("到底是先爆的抖音，还是先爆的酷狗？是个问题。。。"));
    this->m_descVector.emplace_back(QStringLiteral("抖友集合地！一起来听最全的抖音热曲！"));
    this->m_descVector.emplace_back(QStringLiteral("VIPER HiFi官方歌单，最全Hi-Res抖音热歌，不定期更新。"));
    this->m_descVector.emplace_back(QStringLiteral("体验速度激情，一起来飙车吧。"));
    this->m_descVector.emplace_back(QStringLiteral("小编精选最舒缓最好听的轻音乐，每天更新！"));

}

void TableWidget::initItemBlockWidget() {
    this->m_gridContainer->setLayout(m_gridLayout.get());
    this->m_gridLayout->setVerticalSpacing(10);
    this->m_gridLayout->setHorizontalSpacing(5);
    this->m_gridLayout->setContentsMargins(0, 5, 0, 5);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            auto pixPath = this->m_blockCoverPaths[i * 7 + j];
            auto desc = this->m_descVector[i * 7 + j];
            auto block = new ItemBlockWidget(pixPath, desc, this);
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
    vlayout->setSpacing(16);
    vlayout->setContentsMargins(0, 5, 0, 5);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addWidget(this->m_gridContainer.get());
}

void TableWidget::initItemListWidget() {
    this->m_gridLayout->setSpacing(10);
    this->m_gridLayout->setRowStretch(0, 1);
    this->m_gridLayout->setRowStretch(1, 1);
    this->m_gridLayout->setRowStretch(2, 1);
    this->m_gridLayout->setColumnStretch(0, 1);
    this->m_gridLayout->setColumnStretch(1, 1);
    this->m_gridLayout->setColumnStretch(2, 1);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto pixPath = this->m_listCoverPaths[i * 3 + j];
            auto name = this->m_songInfo[i * 3 + j].first;
            auto author = this->m_songInfo[i * 3 + j].second;
            auto item = new ItemListWidget(pixPath, name, author, this);
            this->m_gridLayout->addWidget(item, i, j);
        }
    }

    auto vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(16);
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
                if (item)item->widget()->show();
                //qDebug()<<"show一个";
            }
            this->m_showCol = 6;
            //qDebug()<<"//////////////////////////";
        }
        //由于直接全屏需要一定时间，因此不存在从5列直接变成7列的情况
        //但是，上面说的是正常缩放的情况，但如果存在突然发送的信号的情况。。。
        else {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if (item)item->widget()->show();
                item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item)item->widget()->show();
                //qDebug()<<"show一个";
            }
            this->m_showCol = 7;
        }
    } else if (!this->m_hide_col_6 && this->m_hide_col_7) {
        //显示前6列
        this->m_showCol = 6;
        if (len < 1000) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if (item)item->widget()->hide();
            }
            this->m_showCol = 5;
        } else if (len >= 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item)item->widget()->show();
            }
            this->m_showCol = 7;
        }
    } else if (!this->m_hide_col_7) {
        this->m_showCol = 7;
        //显示前7列
        if (len < 1000) {
            this->m_showCol = 5;
        } else if (len < 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item)item->widget()->hide();
            }
            this->m_showCol = 6;
            //qDebug()<<"********************";
        }
    }
}

void TableWidget::onRefreshBtnClicked() {
    // 启动定时器，延迟处理
    if (!this->m_refreshTimer->isActive()) {
        this->m_refreshTimer->start(300); // 200ms 延迟，避免过于频繁地触发
    }
}

void TableWidget::onRefreshTimeout() {
    //qDebug()<<"刷新";
    if (this->m_kind == KIND::BlockList) {
        shuffleBlockCover();
        this->m_gridContainer->setUpdatesEnabled(false);
        this->m_gridContainer->hide(); // 隐藏容器
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 7; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i, j);
                if (item) {
                    auto widget = item->widget();
                    if (widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                auto pixPath = this->m_blockCoverPaths[i * 7 + j];
                auto desc = this->m_descVector[i * 7 + j];
                auto block = new ItemBlockWidget(pixPath, desc, this);
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
    } else {
        shuffleListCover();
        this->m_gridContainer->setUpdatesEnabled(false);
        this->m_gridContainer->hide(); // 隐藏容器
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i, j);
                if (item) {
                    auto widget = item->widget();
                    if (widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                QString pixPath = this->m_listCoverPaths[i * 3 + j];
                auto name = this->m_songInfo[i * 3 + j].first;
                auto author = this->m_songInfo[i * 3 + j].second;
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
      , m_songNameLab(new QLabel(this))
      , m_singerLab(new QLabel(this))
      , m_optionWidget(new QWidget(this))
      , m_play_add_ToolBtn(new QToolButton(this))
      , m_like_ToolBtn(new QToolButton(this))
      , m_more_ToolBtn(new QToolButton(this))
      , m_songName(name)
      , m_singer(author) {
    this->setFixedHeight(90);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_coverLab->setPixmap(roundedPixmap(coverPix, this->m_coverLab->size(), 12));
    this->m_coverLab->setScaledContents(true);

    this->setObjectName("listWidget");
    this->m_coverLab->setObjectName(QStringLiteral("coverLab"));
    this->m_songNameLab->setObjectName(QStringLiteral("nameLab"));
    this->m_singerLab->setObjectName(QStringLiteral("singerLab"));
    this->m_play_add_ToolBtn->setObjectName(QStringLiteral("play_add_ToolBtn"));
    this->m_like_ToolBtn->setObjectName(QStringLiteral("like_ToolBtn"));
    this->m_more_ToolBtn->setObjectName(QStringLiteral("more_ToolBtn"));

    QFile file(GET_CURRENT_DIR + QStringLiteral("/tablist.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();

    connect(qobject_cast<TableWidget *>(parent), &TableWidget::hideTitle, this, &ItemListWidget::onHide);
}

void ItemListWidget::setNameText(const QString &name) {
    this->m_songName = name;
    updateSongName();
}

void ItemListWidget::setSingerText(const QString &singer) {
    this->m_singer = singer;
    updateSinger();
}

void ItemListWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    //qDebug()<<"ItemListWidget重绘";
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

    //设置文字
    updateSongName();
    updateSinger();
}

void ItemListWidget::enterEvent(QEnterEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    if (!this->m_isHoverCoverLab) {
        this->m_isHoverCoverLab = true;
        //设置文字
        updateSongName();
        updateSinger();
        this->m_play_add_ToolBtn->show();
        this->m_like_ToolBtn->show();
        this->m_more_ToolBtn->show();
        update();
    }
}

void ItemListWidget::leaveEvent(QEvent *ev) {
    QWidget::leaveEvent(ev);
    if (this->m_isHoverCoverLab) {
        this->m_isHoverCoverLab = false;
        //设置文字
        updateSongName();
        updateSinger();
        this->m_play_add_ToolBtn->hide();
        this->m_like_ToolBtn->hide();
        this->m_more_ToolBtn->hide();
        update();
    }
}

void ItemListWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / this->m_aspectRatio);
    //this->setGeometry(this->geometry().x(), this->geometry().y(),this->geometry().width(),event->size().width() / this->m_aspectRatio);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    //设置文字
    updateSongName();
    updateSinger();
}

void ItemListWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void ItemListWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void ItemListWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void ItemListWidget::initUi() {
    //设置鼠标指向
    this->m_coverLab->setCursor(Qt::PointingHandCursor);
    this->m_songNameLab->setCursor(Qt::PointingHandCursor);
    this->m_singerLab->setCursor(Qt::PointingHandCursor);
    this->m_play_add_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_like_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_more_ToolBtn->setCursor(Qt::PointingHandCursor);
    //label
    this->m_songNameLab->setScaledContents(true);
    this->m_singerLab->setScaledContents(true);
    this->m_songNameLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->m_singerLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //optionwidget
    this->m_optionWidget->setFixedWidth(80);
    this->m_optionWidget->setContentsMargins(0, 0, 0, 0);
    this->m_optionWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //布局设置
    auto hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 10, 0);
    hlayout->addWidget(this->m_coverLab);
    //歌曲和歌手垂直布局
    auto vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_songNameLab);
    vlayout->addWidget(this->m_singerLab);

    hlayout->addLayout(vlayout);
    hlayout->addSpacerItem(new QSpacerItem(5, 20, QSizePolicy::Expanding));
    //optionWidget垂直布局
    auto hhlayout = new QHBoxLayout(this->m_optionWidget);
    hhlayout->addWidget(this->m_play_add_ToolBtn);
    hhlayout->addWidget(this->m_like_ToolBtn);
    hhlayout->addWidget(this->m_more_ToolBtn);

    hlayout->addWidget(this->m_optionWidget);

    //遮罩设置
    this->m_mask->setEnterWidgetChangeCursor(false);
    this->m_mask->move(this->m_coverLab->pos());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    this->m_mask->setStander(70);
    this->m_mask->hide();
    //初始隐藏按钮
    this->m_optionWidget->show();
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
}

void ItemListWidget::updateSongName() const {
    //设置字体测量工具
    auto font = this->m_songNameLab->font();
    QFontMetrics fm(font);
    this->m_songNameLab->setToolTip(this->m_songName);
    QString elidedText;
    elidedText = fm.elidedText(this->m_songName, Qt::ElideRight,
                               this->width() - this->m_optionWidget->width() - 60);
    this->m_songNameLab->setText(elidedText);
}

void ItemListWidget::updateSinger() const {
    //设置字体测量工具
    auto font = this->m_singerLab->font();
    QFontMetrics fm(font);
    this->m_singerLab->setToolTip(this->m_singer);
    QString elidedText;
    elidedText = fm.elidedText(this->m_singer, Qt::ElideRight,
                               this->width() - this->m_optionWidget->width() - 60);

    this->m_singerLab->setText(elidedText);
}

void ItemListWidget::onHide() {
    qDebug() << "隐藏/显示";
    this->setHidden(!this->isHidden());
    update();
}

#define DescribeLabHeight 25
std::vector<QString> tipArr =
{
    QStringLiteral("日语"),QStringLiteral("经典"),
    QStringLiteral("国语"), QStringLiteral("流行"),
    QStringLiteral("兴奋"), QStringLiteral("英语"),
    QStringLiteral("古风"), QStringLiteral("韩语"),
    QStringLiteral("寂寞"), QStringLiteral("运动"),
    QStringLiteral("说唱"), QStringLiteral("校园")
};

ItemBlockWidget::ItemBlockWidget(const QString &path, const QString &desc, QWidget *parent)
    : QWidget(parent)
      , m_bacWidget(new QWidget(this))
      , m_mask(std::make_unique<SMaskWidget>(this))
      , m_describeLab(new QLabel(this))
      , m_tipLab(new QLabel(this))
      , m_popularBtn(new QToolButton(this))
      , m_descText(desc) {
    QString style = QString("border-radius:8px;border-image:url(%1);").arg(path);
    this->m_bacWidget->setStyleSheet(style);
    this->setFixedHeight(150 + DescribeLabHeight);
    initUi();
    this->m_mask->setParent(this->m_bacWidget);
    this->m_mask->move(this->m_bacWidget->pos());
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_mask->setStander(130);
    this->m_mask->hide();
}

void ItemBlockWidget::setTipLabText(const QString &text) const {
    this->m_tipLab->setText(text);
}

void ItemBlockWidget::setPopularBtnText(const QString &text) const {
    this->m_popularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
}

void ItemBlockWidget::setDescribeText(QString desc) const {
    const QFont font(QStringLiteral("楷体"),DescribeLabHeight - 14);
    // 应用字体到标签
    this->m_describeLab->setFont(font);
    //设置字体测量工具
    QFontMetrics fm(font);
    //设置Tip
    this->m_describeLab->setToolTip(desc);
    desc = fm.elidedText(desc, Qt::ElideRight, this->m_describeLab->width());
    this->m_describeLab->setText(desc);
}

void ItemBlockWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    //qDebug()<<"ItemBlockWidget重绘";
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    // 如果鼠标悬停，绘制半透明蒙层
    /*if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);"));
    } else {
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
        this->m_mask->hide();
    }*/
}

void ItemBlockWidget::enterEvent(QEnterEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    if (!this->m_isHoverCoverLab) {
        this->m_isHoverCoverLab = true;
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet(
            QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);"));
        update();
    }
}

void ItemBlockWidget::leaveEvent(QEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::leaveEvent(ev);
    if (this->m_isHoverCoverLab) {
        this->m_isHoverCoverLab = false;
        this->m_popularBtn->setStyleSheet(
            QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
        this->m_mask->hide();
        update();
    }
}

void ItemBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    this->setFixedHeight(event->size().width() + DescribeLabHeight);
    //this->setGeometry(this->geometry().x(),this->geometry().y(),this->geometry().width(),event->size().width() + DescribeLabHeight);
    //只要留了一定的空间，就能缩小
    this->m_bacWidget->setFixedSize(event->size().width() / 1.05, event->size().width() / 1.05);
    //this->m_bacWidget->setFixedSize(event->size().width(),event->size().width());
    //qDebug()<<"this->m_bacWidget->size() : "<<this->m_bacWidget->size();
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_describeLab->setFixedSize(this->m_bacWidget->width(),DescribeLabHeight);
    this->m_popularBtn->move(this->m_bacWidget->width() - this->m_popularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_popularBtn->height() - 5);
    setDescribeText(this->m_descText);
}

void ItemBlockWidget::mousePressEvent(QMouseEvent *event) {
    //QWidget::mousePressEvent(event);
    event->ignore();
}

void ItemBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    //QWidget::mouseReleaseEvent(event);
    event->ignore();
}

void ItemBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
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
    this->m_popularBtn->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_popularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_popularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_popularBtn->setStyleSheet(
        QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
    //this->m_popularBtn->move(this->width()-this->m_popularBtn->height()-10,this->m_bacWidget->height()-this->m_popularBtn->height()-10);
    //qDebug()<<"this->m_bacWidget->width() : "<<this->m_bacWidget->width()<<"\nthis->width() : "<<this->width();

    this->m_describeLab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(this->m_bacWidget);
    vlayout->addWidget(this->m_describeLab);
}
