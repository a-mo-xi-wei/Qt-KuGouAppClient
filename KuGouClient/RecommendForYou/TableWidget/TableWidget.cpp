/**
 * @file TableWidget.cpp
 * @brief 实现 TableWidget 类，管理表格布局推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "TableWidget.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"

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

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化推荐界面
 * @param title 标题文本
 * @param kind 推荐界面类型
 * @param parent 父控件指针，默认为 nullptr
 */
TableWidget::TableWidget(const QString &title, KIND kind, QWidget *parent)
    : QWidget(parent)
    , m_titleLab(new QLabel(title, this))            ///< 初始化标题标签
    , m_kind(kind)                                   ///< 初始化界面类型
    , m_tabHLayout(new QHBoxLayout)  ///< 初始化标题栏布局
    , m_gridLayout(new QGridLayout)  ///< 初始化网格布局
    , m_gridContainer(new QWidget(this)) ///< 初始化网格容器
    , m_refreshTimer(new QTimer(this))               ///< 初始化刷新定时器
{
    this->m_play_ToolBtn = new QToolButton(this);    ///< 初始化播放按钮
    this->m_adjust_ToolBtn = new QToolButton(this);  ///< 初始化调整按钮
    this->m_refresh_ToolBtn = new QToolButton(this); ///< 初始化刷新按钮
    this->m_more_Lab = new QLabel("更多 >", this);   ///< 初始化更多标签
    this->m_more_Lab->installEventFilter(this);      ///< 为更多标签安装事件过滤器

    initDescVector();                                ///< 初始化描述文本
    initSongInfo();                                  ///< 初始化歌曲信息
    initBlockCover();                                ///< 初始化块状封面
    initListCover();                                 ///< 初始化列表封面
    initUi();                                        ///< 初始化界面

    this->m_adjust_ToolBtn->hide();                  ///< 初始隐藏调整按钮
    if (kind == KIND::ItemList)
    {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] { emit hideTitle(); }); ///< 连接调整按钮隐藏标题信号
    }
    else if (kind == KIND::BlockList)
    {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {
            this->m_gridContainer->setHidden(!this->m_gridContainer->isHidden()); }); ///< 连接调整按钮切换网格容器显示
    }

    connect(this, &TableWidget::gridChange, this, [this](const int &len) { onGridChange(len); }); ///< 连接网格列数变化信号

    connect(this->m_refresh_ToolBtn, &QToolButton::clicked, this, &TableWidget::onRefreshBtnClicked); ///< 连接刷新按钮点击信号
    connect(this->m_refreshTimer, &QTimer::timeout, this, &TableWidget::onRefreshTimeout); ///< 连接刷新定时器超时信号
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制控件背景
 */
void TableWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); ///< 绘制控件背景
}

/**
 * @brief 鼠标进入事件
 * @param ev 进入事件
 * @note 显示调整按钮和分隔线
 */
void TableWidget::enterEvent(QEnterEvent *ev)
{
    this->m_adjust_ToolBtn->show();                      ///< 显示调整按钮
    this->line1->show();                                 ///< 显示分隔线
}

/**
 * @brief 鼠标离开事件
 * @param ev 离开事件
 * @note 隐藏调整按钮和分隔线
 */
void TableWidget::leaveEvent(QEvent *ev)
{
    this->m_adjust_ToolBtn->hide();                      ///< 隐藏调整按钮
    this->line1->hide();                                 ///< 隐藏分隔线
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整网格布局并更新列宽
 */
void TableWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // qDebug() << "this->width : " << this->width();   ///< 未使用，保留用于调试
    emit gridChange(this->width());                      ///< 触发网格列数变化信号
    if (this->m_kind == KIND::ItemList)
    {
        if (!this->window())
        {
            qWarning() << "无法获取顶级窗口！";
            STREAM_WARN() << "无法获取顶级窗口！";      ///< 记录错误日志
            return;
        }
        int topLevelWidth = this->window()->width();
        auto average = (topLevelWidth - 255) / 3;        ///< 计算平均列宽
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                this->m_gridLayout->itemAtPosition(i, j)->widget()->setFixedWidth(average); ///< 设置推荐项宽度
            }
        }
    }
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void TableWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void TableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void TableWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理更多标签的鼠标点击事件
 */
bool TableWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->m_more_Lab)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                QString("%1 功能未实现 敬请期待").arg(this->m_more_Lab->text().left(this->m_more_Lab->text().size() - 2)), 1000, this->window()); ///< 显示未实现提示
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类处理
}

/**
 * @brief 初始化界面
 * @note 初始化工具提示、按钮、样式表、标题栏布局和网格容器
 */
void TableWidget::initUi()
{
    auto play_toolBtn_toolTip = new ElaToolTip(this->m_play_ToolBtn); ///< 初始化播放按钮工具提示
    play_toolBtn_toolTip->setToolTip(QStringLiteral("开始播放"));

    auto adjust_toolBtn_toolTip = new ElaToolTip(this->m_adjust_ToolBtn); ///< 初始化调整按钮工具提示
    adjust_toolBtn_toolTip->setToolTip(QStringLiteral("隐藏栏目"));

    auto refresh_toolBtn_toolTip = new ElaToolTip(this->m_refresh_ToolBtn); ///< 初始化刷新按钮工具提示
    refresh_toolBtn_toolTip->setToolTip(QStringLiteral("刷新"));

    this->m_play_ToolBtn->setCursor(Qt::PointingHandCursor); ///< 设置播放按钮光标
    this->m_adjust_ToolBtn->setCursor(Qt::PointingHandCursor); ///< 设置调整按钮光标
    this->m_refresh_ToolBtn->setCursor(Qt::PointingHandCursor); ///< 设置刷新按钮光标
    this->m_more_Lab->setCursor(Qt::PointingHandCursor);     ///< 设置更多标签光标

    this->m_titleLab->setObjectName(QStringLiteral("titleLab")); ///< 设置标题标签对象名
    this->m_play_ToolBtn->setObjectName(QStringLiteral("play_ToolBtn")); ///< 设置播放按钮对象名
    this->m_adjust_ToolBtn->setObjectName(QStringLiteral("adjust_ToolBtn")); ///< 设置调整按钮对象名
    this->m_refresh_ToolBtn->setObjectName(QStringLiteral("refresh_ToolBtn")); ///< 设置刷新按钮对象名
    this->m_more_Lab->setObjectName(QStringLiteral("moreLab")); ///< 设置更多标签对象名

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));        ///< 设置播放按钮图标大小

    this->m_refreshTimer->setSingleShot(true);               ///< 设置刷新定时器为单次触发

    QFile file(GET_CURRENT_DIR + QStringLiteral("/tab.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());                 ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";             ///< 记录错误日志
        return;
    }
    m_tabHLayout->setSpacing(0);                             ///< 设置标题栏布局间距
    m_tabHLayout->addWidget(this->m_titleLab);               ///< 添加标题标签
    m_tabHLayout->addWidget(this->m_play_ToolBtn);           ///< 添加播放按钮
    m_tabHLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding)); ///< 添加扩展间隔
    m_tabHLayout->addWidget(this->m_adjust_ToolBtn);         ///< 添加调整按钮
    this->line1 = new QLabel(this);                          ///< 初始化分隔线
    line1->hide();                                           ///< 初始隐藏分隔线
    line1->setScaledContents(true);                          ///< 设置分隔线内容缩放
    line1->setFixedSize(20, 15);                             ///< 设置分隔线大小
    line1->setPixmap(QPixmap(QStringLiteral(":/TabIcon/Res/tabIcon/line-gray.svg"))); ///< 设置分隔线图片
    m_tabHLayout->addWidget(line1);                          ///< 添加分隔线
    m_tabHLayout->addWidget(this->m_refresh_ToolBtn);        ///< 添加刷新按钮
    const auto line2 = new QLabel(this);                     ///< 初始化第二分隔线
    line2->setScaledContents(true);                          ///< 设置第二分隔线内容缩放
    line2->setFixedSize(20, 15);                             ///< 设置第二分隔线大小
    line2->setPixmap(QPixmap(QStringLiteral(":/TabIcon/Res/tabIcon/line-gray.svg"))); ///< 设置第二分隔线图片
    m_tabHLayout->addWidget(line2);                          ///< 添加第二分隔线
    m_tabHLayout->addWidget(this->m_more_Lab);               ///< 添加更多标签
    this->m_gridContainer->setAttribute(Qt::WA_OpaquePaintEvent); ///< 设置网格容器不透明绘制

    if (this->m_kind == KIND::ItemList)
    {
        initItemListWidget();                                ///< 初始化小图标列表推荐控件
    }
    else if (this->m_kind == KIND::BlockList)
    {
        this->m_play_ToolBtn->hide();                        ///< 隐藏播放按钮
        this->m_more_Lab->setText(QStringLiteral("歌单广场 >")); ///< 设置更多标签文本
        initItemBlockWidget();                               ///< 初始化块状推荐控件
    }
}

/**
 * @brief 初始化块状封面
 * @note 设置块状推荐项的封面路径
 */
void TableWidget::initBlockCover()
{
    for (int i = 1; i <= 60; ++i)
    {
        this->m_blockCoverPaths.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i)); ///< 添加封面路径
    }
}

/**
 * @brief 打乱块状封面
 * @note 随机打乱块状封面和描述顺序
 */
void TableWidget::shuffleBlockCover()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机数种子
    std::shuffle(this->m_blockCoverPaths.begin(), this->m_blockCoverPaths.end(), std::default_random_engine(seed)); ///< 随机打乱封面
    std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed)); ///< 随机打乱描述
}

/**
 * @brief 初始化列表封面
 * @note 设置小图标列表推荐项的封面路径
 */
void TableWidget::initListCover()
{
    for (int i = 1; i <= 60; ++i)
    {
        this->m_listCoverPaths.emplace_back(QString(":/ListCover/Res/listcover/music-list-cover%1.jpg").arg(i)); ///< 添加封面路径
    }
    shuffleListCover();                                      ///< 打乱列表封面
}

/**
 * @brief 打乱列表封面
 * @note 随机打乱列表封面和歌曲信息顺序
 */
void TableWidget::shuffleListCover()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机数种子
    std::shuffle(this->m_listCoverPaths.begin(), this->m_listCoverPaths.end(), std::default_random_engine(seed)); ///< 随机打乱封面
    std::shuffle(this->m_songInfo.begin(), this->m_songInfo.end(), std::default_random_engine(seed)); ///< 随机打乱歌曲信息
}

/**
 * @brief 初始化歌曲信息
 * @note 设置歌曲名和歌手信息，共 87 条
 */
void TableWidget::initSongInfo()
{
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谁"), QStringLiteral("洋澜一")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我的楼兰"), QStringLiteral("云朵")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("白鸽乌鸦相爱的戏码"), QStringLiteral("潘成（皮卡潘）")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("爱错"), QStringLiteral("王力宏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("西海情歌"), QStringLiteral("刀郎")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("有爱就不怕"), QStringLiteral("庄心妍")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("芦苇飞"), QStringLiteral("泡芙芙Scarlett")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不负众望"), QStringLiteral("陈婉菁")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("转身即心痛"), QStringLiteral("吉星出租")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风吹起 (男版)"), QStringLiteral("清唯")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("喀什噶尔胡杨"), QStringLiteral("刀郎")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谦让"), QStringLiteral("王靖雯")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("青花"), QStringLiteral("周传雄")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("时光胶囊"), QStringLiteral("青丝")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("相思遥"), QStringLiteral("玉慧同学")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("谁不是过着坎坎坷坷的人生"), QStringLiteral("任夏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不要慌太阳下山有月光"), QStringLiteral("GooGoo")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("卡拉永远OK"), QStringLiteral("谭咏麟")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("春不晚 (女生版)"), QStringLiteral("冰洁")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("不说"), QStringLiteral("周公")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("달과 6펜스 (月亮与6便士)"), QStringLiteral("Lucia")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("明明 (深爱着你)"), QStringLiteral("丁肆Dicey")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("赛勒斯的爱"), QStringLiteral("张敬轩")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤独患者"), QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤勇者"), QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("麦恩莉"), QStringLiteral("方大同")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("野孩子"), QStringLiteral("李昊")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("先说爱的人为什么先离开"), QStringLiteral("张宇佳")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("白鸽乌鸦相爱的戏码"), QStringLiteral("七喜")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我知道 (我知道你还是爱着我)"), QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("亲爱的, 那不是爱情"), QStringLiteral("en")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("알려줄게 (Feat. 설지안)"), QStringLiteral("Jason &amp; 38SUN、설지안")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("归途的光"), QStringLiteral("诺爷Plus")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风经过 (烟嗓女版)"), QStringLiteral("黄静美")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Под луной (在月亮下)(Silver Ace remix)"), QStringLiteral("SUNAMI")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("只要你还需要我"), QStringLiteral("半吨兄弟")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("错位时空"), QStringLiteral("海边的浪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("全世界只爱你 (无论我走到哪里)"), QStringLiteral("KEY.Y杨思豪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("来我身边，给你所有安全感"), QStringLiteral("半吨儿")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("唱月"), QStringLiteral("李耀阳")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Hate Me (remix)"), QStringLiteral("TaIsil")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("假如我不再善良"), QStringLiteral("歌手玫玫")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("与天空比个耶 (父子版)"), QStringLiteral("袁大脑袋爱嘟嘟<")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("放纵L"), QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("ありがとう・・・ (谢谢…)"), QStringLiteral("KOKIA")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("一切都是我太过骄纵 (这份爱没停过)"), QStringLiteral("zyy")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Yesterday Once More (新版)"), QStringLiteral("七元")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("孤独患者"), QStringLiteral("陈奕迅")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("土坡上的狗尾草"), QStringLiteral("卢润泽")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("APT."), QStringLiteral("ROSÉ、Bruno Mars")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Cry For Me (feat. Ami)"), QStringLiteral("Michita、Ami")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("凑热闹"), QStringLiteral("BY2")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("有爱就不怕"), QStringLiteral("庄心妍")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("秋风经过"), QStringLiteral("任夏")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("情罪"), QStringLiteral("陈瑞")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("我知道"), QStringLiteral("BY2")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("青丝"), QStringLiteral("时光胶囊")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("望故乡"), QStringLiteral("文夫")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("归途的光"), QStringLiteral("诺爷Plus")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("小孩"), QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"), QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("红昭愿 (高音质版)"), QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"), QStringLiteral("王梓钰")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("相思遥"), QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"), QStringLiteral("玉慧同学")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("小孩 (反盗版)"), QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("换个方向风景会更好看"), QStringLiteral("黑大婶回乡带娃")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("苹果香"), QStringLiteral("杨一琪")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("才二十三"), QStringLiteral("方大同")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("当你看着我的时候1.3x"), QStringLiteral("罗森涛")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Lifestyle"), QStringLiteral("HMHK")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Fantasy"), QStringLiteral("Nova Aurora")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("时代进行曲 (Original Mix)"), QStringLiteral("VZEUS")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("梦里花 (唯一纯白的茉莉花)(DJ版)"), QStringLiteral("DJ阿智、yiyi林淑怡")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("画心 (猜不透是你瞳孔的颜色)"), QStringLiteral("于春洋")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("只你"), QStringLiteral("颜人中")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("千年 (DJ版)"), QStringLiteral("DJHZ")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("爱错"), QStringLiteral("en")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("执子之手"), QStringLiteral("宝石Gem、一哩哩一")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("regret (我在未来等你)"), QStringLiteral("Chypy！")));
    this->m_songInfo.emplace_back(QPair<QString, QString>(QStringLiteral("Blame You"), QStringLiteral("E27XG_")));
}

/**
 * @brief 初始化描述文本
 * @note 设置推荐项的描述文本，共 73 条
 */
void TableWidget::initDescVector()
{
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
    this->m_descVector.emplace_back(QStringLiteral("酷音乐亚洲盛典邀你重温经典网络红歌，让记忆回到最美的年华！"));
    this->m_descVector.emplace_back(QStringLiteral("聆听90后记忆中的声音，希望你喜欢！"));
    this->m_descVector.emplace_back(QStringLiteral("抖腿达人必备DJ神曲，适合夜店/车载/party！"));
    this->m_descVector.emplace_back(QStringLiteral("喜欢的歌 静静听！"));
    this->m_descVector.emplace_back(QStringLiteral("绝绝子R&B精选60首！"));
    this->m_descVector.emplace_back(QStringLiteral("爆燃欧美英文完整版丨听完满血复活的歌曲！"));
    this->m_descVector.emplace_back(QStringLiteral("阿米歌单：防弹咖啡味（个人向）"));
    this->m_descVector.emplace_back(QStringLiteral("一曲入魂，那些自带泪点的OST！"));
    this->m_descVector.emplace_back(QStringLiteral("小声唱我们的歌，属于成长的印记"));
    this->m_descVector.emplace_back(QStringLiteral("千秋如一梦，历史终留名"));
    this->m_descVector.emplace_back(QStringLiteral("民谣弹唱｜献给生活的一首诗"));
    this->m_descVector.emplace_back(QStringLiteral("只待独梅映残雪，不求寒月照旧人"));
    this->m_descVector.emplace_back(QStringLiteral("新一天快乐依旧，轻松节奏继续前进"));
    this->m_descVector.emplace_back(QStringLiteral("想剪下一米阳光，照亮你的世界"));
    this->m_descVector.emplace_back(QStringLiteral("国风新歌：炉向初冬火，温暖庇人心"));
    this->m_descVector.emplace_back(QStringLiteral("清晨暖心民谣，温暖你我的心"));
    this->m_descVector.emplace_back(QStringLiteral("通勤BGM：下班需要一点仪式感！"));
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

/**
 * @brief 初始化块状推荐控件
 * @note 创建块状推荐项并添加到网格（2行7列）
 */
void TableWidget::initItemBlockWidget()
{
    this->m_gridContainer->setLayout(m_gridLayout); ///< 设置网格容器布局
    this->m_gridLayout->setVerticalSpacing(10);          ///< 设置垂直间距
    this->m_gridLayout->setHorizontalSpacing(5);         ///< 设置水平间距
    this->m_gridLayout->setContentsMargins(0, 5, 0, 5);  ///< 设置边距
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            auto pixPath = this->m_blockCoverPaths[i * 7 + j]; ///< 获取封面路径
            auto desc = this->m_descVector[i * 7 + j];       ///< 获取描述文本
            auto block = new ItemBlockWidget(pixPath, desc, this); ///< 创建块状推荐项
            block->hide();                                   ///< 初始隐藏
            this->m_gridLayout->addWidget(block, i, j);      ///< 添加到网格
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < this->m_showCol; ++j)
        {
            this->m_gridLayout->itemAtPosition(i, j)->widget()->show(); ///< 显示前 m_showCol 列
        }
    }
    auto vlayout = new QVBoxLayout(this);                ///< 创建主垂直布局
    vlayout->setSpacing(16);                             ///< 设置间距
    vlayout->setContentsMargins(0, 5, 0, 5);             ///< 设置边距
    vlayout->addLayout(m_tabHLayout);              ///< 添加标题栏布局
    vlayout->addWidget(this->m_gridContainer);     ///< 添加网格容器
}

/**
 * @brief 初始化小图标列表推荐控件
 * @note 创建小图标列表推荐项并添加到网格（3行3列）
 */
void TableWidget::initItemListWidget()
{
    this->m_gridLayout->setSpacing(10);                  ///< 设置间距
    this->m_gridLayout->setRowStretch(0, 1);             ///< 设置行拉伸
    this->m_gridLayout->setRowStretch(1, 1);
    this->m_gridLayout->setRowStretch(2, 1);
    this->m_gridLayout->setColumnStretch(0, 1);          ///< 设置列拉伸
    this->m_gridLayout->setColumnStretch(1, 1);
    this->m_gridLayout->setColumnStretch(2, 1);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            auto pixPath = this->m_listCoverPaths[i * 3 + j]; ///< 获取封面路径
            auto name = this->m_songInfo[i * 3 + j].first;   ///< 获取歌曲名
            auto author = this->m_songInfo[i * 3 + j].second; ///< 获取歌手
            auto item = new ItemListWidget(pixPath, name, author, this); ///< 创建列表推荐项
            this->m_gridLayout->addWidget(item, i, j);       ///< 添加到网格
        }
    }

    auto vlayout = new QVBoxLayout(this);        ///< 创建主垂直布局
    vlayout->setSpacing(16);                             ///< 设置间距
    vlayout->addLayout(m_tabHLayout);                    ///< 添加标题栏布局
    vlayout->addLayout(this->m_gridLayout);              ///< 添加网格布局
}

/**
 * @brief 网格列数改变槽函数
 * @param len 窗口宽度
 * @note 动态调整网格列数（5、6、7列）
 */
void TableWidget::onGridChange(int len)
{
    this->m_hide_col_6 = true;                           ///< 初始化第六列隐藏状态
    this->m_hide_col_7 = true;                           ///< 初始化第七列隐藏状态
    auto item = this->m_gridLayout->itemAtPosition(0, 5);
    if (item)
    {
        this->m_hide_col_6 = item->widget()->isHidden(); ///< 更新第六列隐藏状态
        // qDebug() << "hide_col_6 : " << hide_col_6;   ///< 未使用，保留用于调试
    }
    item = this->m_gridLayout->itemAtPosition(0, 6);
    if (item)
    {
        this->m_hide_col_7 = item->widget()->isHidden(); ///< 更新第七列隐藏状态
        // qDebug() << "hide_col_7 : " << hide_col_7;   ///< 未使用，保留用于调试
    }

    if (this->m_hide_col_6)
    {
        this->m_showCol = 5;                             ///< 显示前5列
        if (len < 1000) return;
        if (len < 1200)
        {
            for (int i = 0; i < 2; ++i)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if (item) item->widget()->show();        ///< 显示第六列
                // qDebug() << "show一个";              ///< 未使用，保留用于调试
            }
            this->m_showCol = 6;                         ///< 更新显示列数
            // qDebug() << "//////////////////////////"; ///< 未使用，保留用于调试
        }
        //由于直接全屏需要一定时间，因此不存在从5列直接变成7列的情况
        //但是，上面说的是正常缩放的情况，但如果存在突然发送的信号的情况。。。
        else
        {
            for (int i = 0; i < 2; ++i)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if (item) item->widget()->show();        ///< 显示第六列
                item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item) item->widget()->show();        ///< 显示第七列
                // qDebug() << "show一个";              ///< 未使用，保留用于调试
            }
            this->m_showCol = 7;                         ///< 更新显示列数
            // @note 直接全屏需要时间，不存在从5列直接变为7列，但可能因异常信号触发
        }
    }
    else if (!this->m_hide_col_6 && this->m_hide_col_7)
    {
        this->m_showCol = 6;                             ///< 显示前6列
        if (len < 1000)
        {
            for (int i = 0; i < 2; ++i)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if (item) item->widget()->hide();        ///< 隐藏第六列
            }
            this->m_showCol = 5;                         ///< 更新显示列数
        }
        else if (len >= 1200)
        {
            for (int i = 0; i < 2; ++i)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item) item->widget()->show();        ///< 显示第七列
            }
            this->m_showCol = 7;                         ///< 更新显示列数
        }
    }
    else if (!this->m_hide_col_7)
    {
        this->m_showCol = 7;                             ///< 显示前7列
        if (len < 1000)
        {
            this->m_showCol = 5;                         ///< 更新显示列数
        }
        else if (len < 1200)
        {
            for (int i = 0; i < 2; ++i)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if (item) item->widget()->hide();        ///< 隐藏第七列
            }
            this->m_showCol = 6;                         ///< 更新显示列数
            // qDebug() << "********************";       ///< 未使用，保留用于调试
        }
    }
}

/**
 * @brief 刷新按钮点击槽函数
 * @note 启动刷新定时器以延迟处理
 */
void TableWidget::onRefreshBtnClicked()
{
    if (!this->m_refreshTimer->isActive())
    {
        this->m_refreshTimer->start(300);                ///< 启动定时器，300ms 延迟
    }
}

/**
 * @brief 刷新定时器超时槽函数
 * @note 刷新网格内容并显示成功提示
 */
void TableWidget::onRefreshTimeout()
{
    qDebug() << "刷新TableWidget";
    STREAM_INFO() << "刷新TableWidget";                 ///< 记录刷新日志
    if (this->m_kind == KIND::BlockList)
    {
        shuffleBlockCover();                             ///< 打乱块状封面和描述
        this->m_gridContainer->setUpdatesEnabled(false); ///< 禁用更新以优化性能
        this->m_gridContainer->hide();                   ///< 隐藏容器
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 7; ++j)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, j);
                if (item)
                {
                    auto widget = item->widget();
                    if (widget)
                    {
                        this->m_gridLayout->removeWidget(widget); ///< 移除旧控件
                        widget->deleteLater();               ///< 延迟删除
                    }
                }
                auto pixPath = this->m_blockCoverPaths[i * 7 + j]; ///< 获取封面路径
                auto desc = this->m_descVector[i * 7 + j]; ///< 获取描述文本
                auto block = new ItemBlockWidget(pixPath, desc, this); ///< 创建新块状推荐项
                block->hide();                               ///< 初始隐藏
                this->m_gridLayout->addWidget(block, i, j);  ///< 添加到网格
            }
        }
        emit gridChange(this->width());                  ///< 触发网格列数变化
        // qDebug() << "当前显示 ： " << this->m_showCol; ///< 未使用，保留用于调试
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < this->m_showCol; ++j)
            {
                this->m_gridLayout->itemAtPosition(i, j)->widget()->show(); ///< 显示前 m_showCol 列
            }
        }
        this->m_gridContainer->setUpdatesEnabled(true);  ///< 启用更新
        this->m_gridContainer->show();                   ///< 显示容器
    }
    else
    {
        shuffleListCover();                              ///< 打乱列表封面和歌曲信息
        this->m_gridContainer->setUpdatesEnabled(false); ///< 禁用更新以优化性能
        this->m_gridContainer->hide();                   ///< 隐藏容器
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                auto item = this->m_gridLayout->itemAtPosition(i, j);
                if (item)
                {
                    auto widget = item->widget();
                    if (widget)
                    {
                        this->m_gridLayout->removeWidget(widget); ///< 移除旧控件
                        widget->deleteLater();               ///< 延迟删除
                    }
                }
                QString pixPath = this->m_listCoverPaths[i * 3 + j]; ///< 获取封面路径
                auto name = this->m_songInfo[i * 3 + j].first; ///< 获取歌曲名
                auto author = this->m_songInfo[i * 3 + j].second; ///< 获取歌手
                auto list = new ItemListWidget(pixPath, name, author, this); ///< 创建新列表推荐项
                this->m_gridLayout->addWidget(list, i, j);   ///< 添加到网格
            }
        }
        this->m_gridContainer->setUpdatesEnabled(true);  ///< 启用更新
        this->m_gridContainer->show();                   ///< 显示容器
    }
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success",
        QString("%1 刷新成功").arg(this->m_titleLab->text()), 1000, this->window()); ///< 显示刷新成功提示
}

/**
 * @brief 创建圆角图片
 * @param src 源图片
 * @param size 目标大小
 * @param radius 圆角半径
 * @return 圆角图片
 * @note 使用抗锯齿和剪裁路径生成圆角图片
 */
QPixmap roundedPixmap(QPixmap &src, QSize size, int radius)
{
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放源图片
    QPixmap dest(size);
    dest.fill(Qt::transparent);                          ///< 设置透明背景

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);       ///< 启用抗锯齿
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius); ///< 创建圆角矩形路径
    painter.setClipPath(path);                           ///< 设置剪裁路径
    painter.drawPixmap(0, 0, scaled);                    ///< 绘制图片
    return dest;
}

/**
 * @brief 构造函数，初始化列表推荐项
 * @param coverPix 封面图片
 * @param name 歌曲名
 * @param author 歌手
 * @param parent 父控件指针，默认为 nullptr
 */
ItemListWidget::ItemListWidget(QPixmap coverPix, const QString &name, const QString &author, QWidget *parent)
    : QWidget(parent)
    , m_mask(std::make_unique<SMaskWidget>(this))        ///< 初始化遮罩控件
    , m_coverLab(new QLabel(this))                       ///< 初始化封面标签
    , m_songNameLab(new QLabel(this))                    ///< 初始化歌曲名标签
    , m_singerLab(new QLabel(this))                      ///< 初始化歌手标签
    , m_optionWidget(new QWidget(this))                  ///< 初始化选项控件
    , m_play_add_ToolBtn(new QToolButton(this))          ///< 初始化播放添加按钮
    , m_like_ToolBtn(new QToolButton(this))              ///< 初始化喜欢按钮
    , m_more_ToolBtn(new QToolButton(this))              ///< 初始化更多按钮
    , m_songName(name)                                   ///< 初始化歌曲名
    , m_singer(author)                                   ///< 初始化歌手
{
    this->setFixedHeight(90);                            ///< 设置固定高度
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); ///< 设置大小策略
    this->m_coverLab->setFixedSize(this->height(), this->height()); ///< 设置封面大小
    this->m_coverLab->setPixmap(roundedPixmap(coverPix, this->m_coverLab->size(), 12)); ///< 设置圆角封面
    this->m_coverLab->setScaledContents(true);           ///< 设置封面内容缩放

    this->setObjectName("listWidget");                   ///< 设置控件对象名
    this->m_coverLab->setObjectName(QStringLiteral("coverLab")); ///< 设置封面标签对象名
    this->m_songNameLab->setObjectName(QStringLiteral("nameLab")); ///< 设置歌曲名标签对象名
    this->m_singerLab->setObjectName(QStringLiteral("singerLab")); ///< 设置歌手标签对象名
    this->m_play_add_ToolBtn->setObjectName(QStringLiteral("play_add_ToolBtn")); ///< 设置播放添加按钮对象名
    this->m_like_ToolBtn->setObjectName(QStringLiteral("like_ToolBtn")); ///< 设置喜欢按钮对象名
    this->m_more_ToolBtn->setObjectName(QStringLiteral("more_ToolBtn")); ///< 设置更多按钮对象名

    QFile file(GET_CURRENT_DIR + QStringLiteral("/tablist.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";         ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面

    connect(qobject_cast<TableWidget *>(parent), &TableWidget::hideTitle, this, &ItemListWidget::onHide); ///< 连接隐藏标题信号
}

/**
 * @brief 设置歌曲名
 * @param name 歌曲名
 * @note 更新歌曲名并触发显示更新
 */
void ItemListWidget::setNameText(const QString &name)
{
    this->m_songName = name;                             ///< 更新歌曲名
    updateSongName();                                    ///< 更新显示
}

/**
 * @brief 设置歌手
 * @param singer 歌手
 * @note 更新歌手并触发显示更新
 */
void ItemListWidget::setSingerText(const QString &singer)
{
    this->m_singer = singer;                             ///< 更新歌手
    updateSinger();                                      ///< 更新显示
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制控件背景和悬停遮罩
 */
void ItemListWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);                             ///< 调用父类绘制
    // qDebug() << "ItemListWidget重绘";                ///< 未使用，保留用于调试
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); ///< 绘制控件背景

    p.setRenderHint(QPainter::Antialiasing, true);       ///< 启用抗锯齿
    if (this->m_isHoverCoverLab)
    {
        this->m_mask->show();                            ///< 显示遮罩
        this->m_mask->raise();                           ///< 提升遮罩层级
    }
    else
    {
        this->m_mask->hide();                            ///< 隐藏遮罩
    }

    updateSongName();                                    ///< 更新歌曲名显示
    updateSinger();                                      ///< 更新歌手显示
}

/**
 * @brief 鼠标进入事件
 * @param ev 进入事件
 * @note 显示遮罩和操作按钮
 */
void ItemListWidget::enterEvent(QEnterEvent *ev)
{
    QWidget::enterEvent(ev);                             ///< 调用父类处理
    if (!this->m_isHoverCoverLab)
    {
        this->m_isHoverCoverLab = true;                  ///< 设置悬停状态
        updateSongName();                                ///< 更新歌曲名显示
        updateSinger();                                  ///< 更新歌手显示
        this->m_play_add_ToolBtn->show();                ///< 显示播放添加按钮
        this->m_like_ToolBtn->show();                    ///< 显示喜欢按钮
        this->m_more_ToolBtn->show();                    ///< 显示更多按钮
        update();                                        ///< 触发重绘
    }
}

/**
 * @brief 鼠标离开事件
 * @param ev 离开事件
 * @note 隐藏遮罩和操作按钮
 */
void ItemListWidget::leaveEvent(QEvent *ev)
{
    QWidget::leaveEvent(ev);                             ///< 调用父类处理
    if (this->m_isHoverCoverLab)
    {
        this->m_isHoverCoverLab = false;                 ///< 清除悬停状态
        updateSongName();                                ///< 更新歌曲名显示
        updateSinger();                                  ///< 更新歌手显示
        this->m_play_add_ToolBtn->hide();                ///< 隐藏播放添加按钮
        this->m_like_ToolBtn->hide();                    ///< 隐藏喜欢按钮
        this->m_more_ToolBtn->hide();                    ///< 隐藏更多按钮
        update();                                        ///< 触发重绘
    }
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整控件高度、封面和遮罩大小
 */
void ItemListWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类处理
    this->setFixedHeight(event->size().width() / this->m_aspectRatio); ///< 设置固定高度（宽高比 6）
    this->m_coverLab->setFixedSize(this->height(), this->height()); ///< 设置封面大小
    this->m_mask->setFixedSize(this->m_coverLab->size()); ///< 设置遮罩大小
    updateSongName();                                    ///< 更新歌曲名显示
    updateSinger();                                      ///< 更新歌手显示
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemListWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemListWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 初始化界面
 * @note 初始化工具提示、按钮、布局和遮罩
 */
void ItemListWidget::initUi()
{
    auto songName_toolTip = new ElaToolTip(this->m_songNameLab); ///< 初始化歌曲名工具提示
    songName_toolTip->setToolTip(this->m_songName);
    auto singer_toolTip = new ElaToolTip(this->m_singerLab);    ///< 初始化歌手工具提示
    singer_toolTip->setToolTip(this->m_singer);

    this->m_coverLab->setCursor(Qt::PointingHandCursor);        ///< 设置封面标签光标
    this->m_songNameLab->setCursor(Qt::PointingHandCursor);     ///< 设置歌曲名标签光标
    this->m_singerLab->setCursor(Qt::PointingHandCursor);       ///< 设置歌手标签光标
    this->m_play_add_ToolBtn->setCursor(Qt::PointingHandCursor); ///< 设置播放添加按钮光标
    this->m_like_ToolBtn->setCursor(Qt::PointingHandCursor);    ///< 设置喜欢按钮光标
    this->m_more_ToolBtn->setCursor(Qt::PointingHandCursor);    ///< 设置更多按钮光标

    this->m_songNameLab->setScaledContents(true);               ///< 设置歌曲名标签内容缩放
    this->m_singerLab->setScaledContents(true);                 ///< 设置歌手标签内容缩放
    this->m_songNameLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); ///< 设置歌曲名标签大小策略
    this->m_singerLab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred); ///< 设置歌手标签大小策略

    this->m_optionWidget->setFixedWidth(80);                    ///< 设置选项控件固定宽度
    this->m_optionWidget->setContentsMargins(0, 0, 0, 0);       ///< 设置选项控件边距
    this->m_optionWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); ///< 设置选项控件大小策略
    // @note 选项控件仅用于计算标签宽度

    auto hlayout = new QHBoxLayout(this);                       ///< 创建主水平布局
    hlayout->setContentsMargins(0, 0, 10, 0);                   ///< 设置边距
    hlayout->addWidget(this->m_coverLab);                       ///< 添加封面标签

    auto vlayout = new QVBoxLayout;                             ///< 创建歌曲信息垂直布局
    vlayout->addWidget(this->m_songNameLab);                    ///< 添加歌曲名标签
    vlayout->addWidget(this->m_singerLab);                      ///< 添加歌手标签

    hlayout->addLayout(vlayout);                                ///< 添加歌曲信息布局
    hlayout->addSpacerItem(new QSpacerItem(5, 20, QSizePolicy::Expanding)); ///< 添加扩展间隔

    auto hhlayout = new QHBoxLayout(this->m_optionWidget);      ///< 创建选项控件水平布局
    hhlayout->addWidget(this->m_play_add_ToolBtn);              ///< 添加播放添加按钮
    hhlayout->addWidget(this->m_like_ToolBtn);                  ///< 添加喜欢按钮
    hhlayout->addWidget(this->m_more_ToolBtn);                  ///< 添加更多按钮

    hlayout->addWidget(this->m_optionWidget);                   ///< 添加选项控件

    this->m_mask->setEnterWidgetChangeCursor(false);            ///< 设置遮罩不更改光标
    this->m_mask->move(this->m_coverLab->pos());                ///< 设置遮罩位置
    this->m_mask->setFixedSize(this->m_coverLab->size());       ///< 设置遮罩大小
    this->m_mask->setStander(60);                               ///< 设置遮罩标准值
    this->m_mask->hide();                                       ///< 初始隐藏遮罩

    this->m_optionWidget->show();                               ///< 显示选项控件
    this->m_play_add_ToolBtn->hide();                           ///< 初始隐藏播放添加按钮
    this->m_like_ToolBtn->hide();                               ///< 初始隐藏喜欢按钮
    this->m_more_ToolBtn->hide();                               ///< 初始隐藏更多按钮

    connect(m_play_add_ToolBtn, &QToolButton::clicked, this, &ItemListWidget::onPlayAddToolBtn); ///< 连接播放添加按钮信号
    connect(m_like_ToolBtn, &QToolButton::clicked, this, &ItemListWidget::onLikeToolBtn); ///< 连接喜欢按钮信号
    connect(m_more_ToolBtn, &QToolButton::clicked, this, &ItemListWidget::onMoreToolBtn); ///< 连接更多按钮信号
}

/**
 * @brief 更新歌曲名
 * @note 使用字体测量工具处理文本省略
 */
void ItemListWidget::updateSongName() const
{
    auto font = this->m_songNameLab->font();                    ///< 获取字体
    QFontMetrics fm(font);                                      ///< 创建字体测量工具
    QString elidedText = fm.elidedText(this->m_songName, Qt::ElideRight,
        this->width() - this->m_optionWidget->width() - 60);    ///< 计算省略文本
    this->m_songNameLab->setText(elidedText);                   ///< 设置歌曲名文本
}

/**
 * @brief 更新歌手
 * @note 使用字体测量工具处理文本省略
 */
void ItemListWidget::updateSinger() const
{
    auto font = this->m_singerLab->font();                      ///< 获取字体
    QFontMetrics fm(font);                                      ///< 创建字体测量工具
    QString elidedText = fm.elidedText(this->m_singer, Qt::ElideRight,
        this->width() - this->m_optionWidget->width() - 60);    ///< 计算省略文本
    this->m_singerLab->setText(elidedText);                     ///< 设置歌手文本
}

/**
 * @brief 播放添加按钮点击槽函数
 * @note 显示未实现提示
 */
void ItemListWidget::onPlayAddToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
        "下一首播放 功能未实现 敬请期待", 1000, this->window()); ///< 显示未实现提示
}

/**
 * @brief 喜欢按钮点击槽函数
 * @note 显示未实现提示
 */
void ItemListWidget::onLikeToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
        "收藏 功能未实现 敬请期待", 1000, this->window()); ///< 显示未实现提示
}

/**
 * @brief 更多按钮点击槽函数
 * @note 显示未实现提示
 */
void ItemListWidget::onMoreToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
        "更多 功能未实现 敬请期待", 1000, this->window()); ///< 显示未实现提示
}

/**
 * @brief 隐藏/显示槽函数
 * @note 切换控件隐藏状态并触发重绘
 */
void ItemListWidget::onHide()
{
    // @note 未使用，保留用于调试
    // qDebug() << "隐藏/显示";
    // STREAM_INFO() << "隐藏/显示";
    this->setHidden(!this->isHidden());                  ///< 切换隐藏/显示状态
    update();                                            ///< 触发重绘
}

/** @brief 描述标签高度常量 */
#define DescribeLabHeight 25

/** @brief 推荐标签数组 */
QList<QString> tipArr =
{
    QStringLiteral("日语"), QStringLiteral("经典"),
    QStringLiteral("国语"), QStringLiteral("流行"),
    QStringLiteral("兴奋"), QStringLiteral("英语"),
    QStringLiteral("古风"), QStringLiteral("韩语"),
    QStringLiteral("寂寞"), QStringLiteral("运动"),
    QStringLiteral("说唱"), QStringLiteral("校园")
};

/**
 * @brief 构造函数，初始化块状推荐项
 * @param path 封面图片路径
 * @param desc 描述文本
 * @param parent 父控件指针，默认为 nullptr
 */
ItemBlockWidget::ItemBlockWidget(const QString &path, const QString &desc, QWidget *parent)
    : QWidget(parent)
    , m_bacWidget(new QWidget(this))                     ///< 初始化背景控件
    , m_mask(std::make_unique<SMaskWidget>(this))        ///< 初始化遮罩控件
    , m_describeLab(new QLabel(this))                    ///< 初始化描述标签
    , m_tipLab(new QLabel(this))                         ///< 初始化提示标签
    , m_popularBtn(new QToolButton(this))                ///< 初始化流行按钮
    , m_descText(desc)                                   ///< 初始化描述文本
{
    QString style = QString("border-radius:8px;border-image:url(%1);").arg(path); ///< 设置背景控件封面样式
    this->m_bacWidget->setStyleSheet(style);
    this->setFixedHeight(150 + DescribeLabHeight);       ///< 设置固定高度
    initUi();                                            ///< 初始化界面
    this->m_mask->setParent(this->m_bacWidget);          ///< 设置遮罩父控件
    this->m_mask->move(this->m_bacWidget->pos());        ///< 设置遮罩位置
    this->m_mask->setFixedSize(this->m_bacWidget->size()); ///< 设置遮罩大小
    this->m_mask->setStander(130);                       ///< 设置遮罩标准值
    this->m_mask->hide();                                ///< 初始隐藏遮罩
}

/**
 * @brief 设置提示标签文本
 * @param text 提示文本
 * @note 更新提示标签内容
 */
void ItemBlockWidget::setTipLabText(const QString &text) const
{
    this->m_tipLab->setText(text);                       ///< 设置提示标签文本
}

/**
 * @brief 设置流行按钮文本
 * @param text 播放量文本
 * @note 添加“万”单位并设置文本
 */
void ItemBlockWidget::setPopularBtnText(const QString &text) const
{
    this->m_popularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万")); ///< 设置播放量文本
}

/**
 * @brief 设置描述文本
 * @param desc 描述文本
 * @note 使用字体测量工具处理文本省略
 */
void ItemBlockWidget::setDescribeText(QString desc) const
{
    const QFont font(QStringLiteral("楷体"), DescribeLabHeight - 14); ///< 设置楷体字体
    this->m_describeLab->setFont(font);                  ///< 应用字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    desc = fm.elidedText(desc, Qt::ElideRight, this->m_describeLab->width()); ///< 计算省略文本
    this->m_describeLab->setText(desc);                  ///< 设置描述文本
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制控件背景，注释掉的悬停蒙层绘制逻辑保留
 */
void ItemBlockWidget::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);                             ///< 调用父类绘制
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); ///< 绘制控件背景

    p.setRenderHint(QPainter::Antialiasing, true);       ///< 启用抗锯齿
    // @note 未使用，保留用于调试
    /*if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);"));
    } else {
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
        this->m_mask->hide();
    }*/
}

/**
 * @brief 鼠标进入事件
 * @param ev 进入事件
 * @note 显示遮罩并更新流行按钮样式
 */
void ItemBlockWidget::enterEvent(QEnterEvent *ev)
{
    QWidget::enterEvent(ev);                             ///< 调用父类处理
    if (!this->m_isHoverCoverLab)
    {
        this->m_isHoverCoverLab = true;                  ///< 设置悬停状态
        this->m_mask->show();                            ///< 显示遮罩
        this->m_mask->raise();                           ///< 提升遮罩层级
        this->m_popularBtn->setStyleSheet(
            QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);")); ///< 更新按钮样式
        update();                                        ///< 触发重绘
    }
}

/**
 * @brief 鼠标离开事件
 * @param ev 离开事件
 * @note 隐藏遮罩并恢复流行按钮样式
 */
void ItemBlockWidget::leaveEvent(QEvent *ev)
{
    QWidget::leaveEvent(ev);                             ///< 调用父类处理
    if (this->m_isHoverCoverLab)
    {
        this->m_isHoverCoverLab = false;                 ///< 清除悬停状态
        this->m_popularBtn->setStyleSheet(
            QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);")); ///< 恢复按钮样式
        this->m_mask->hide();                            ///< 隐藏遮罩
        update();                                        ///< 触发重绘
    }
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整控件高度、背景控件、遮罩和描述标签大小
 */
void ItemBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类处理
    this->setFixedHeight(event->size().width() + DescribeLabHeight); ///< 设置固定高度
    this->m_bacWidget->setFixedSize(event->size().width() / 1.05, event->size().width() / 1.05); ///< 设置背景控件大小
    // @note 留一定空间以支持缩小
    this->m_mask->setFixedSize(this->m_bacWidget->size()); ///< 设置遮罩大小
    this->m_describeLab->setFixedSize(this->m_bacWidget->width(), DescribeLabHeight); ///< 设置描述标签大小
    this->m_popularBtn->move(this->m_bacWidget->width() - this->m_popularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_popularBtn->height() - 5); ///< 调整流行按钮位置
    setDescribeText(this->m_descText);                   ///< 更新描述文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void ItemBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 初始化界面
 * @note 初始化工具提示、提示标签、流行按钮和垂直布局
 */
void ItemBlockWidget::initUi()
{
    auto describeLab_toolTip = new ElaToolTip(this->m_describeLab); ///< 初始化描述标签工具提示
    describeLab_toolTip->setToolTip(this->m_descText);   ///< 设置工具提示文本

    this->setCursor(Qt::PointingHandCursor);             ///< 设置控件光标
    this->m_describeLab->setObjectName(QStringLiteral("describeLab")); ///< 设置描述标签对象名
    this->m_tipLab->setObjectName(QStringLiteral("tipLab")); ///< 设置提示标签对象名
    this->m_popularBtn->setObjectName(QStringLiteral("popularLab")); ///< 设置流行按钮对象名

    this->m_tipLab->setFixedSize(50, 20);                ///< 设置提示标签大小
    this->setTipLabText(tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(tipArr.size()))]); ///< 随机选择提示标签
    this->m_tipLab->setAlignment(Qt::AlignCenter);       ///< 设置提示标签居中
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;")); ///< 设置提示标签样式
    this->m_tipLab->move(6, 6);                          ///< 设置提示标签位置

    this->m_popularBtn->setFixedSize(70, 20);            ///< 设置流行按钮大小
    this->m_popularBtn->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/popular-white.svg"))); ///< 设置流行按钮图标
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1)); ///< 设置随机播放量
    this->m_popularBtn->setContentsMargins(5, 0, 5, 0);  ///< 设置流行按钮边距
    this->m_popularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置按钮文本图标样式
    this->m_popularBtn->setStyleSheet(
        QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);")); ///< 设置流行按钮样式

    this->m_describeLab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); ///< 设置描述标签居中

    auto vlayout = new QVBoxLayout(this);                ///< 创建垂直布局
    vlayout->setContentsMargins(0, 0, 0, 0);             ///< 设置布局边距
    vlayout->addWidget(this->m_bacWidget);               ///< 添加背景控件
    vlayout->addWidget(this->m_describeLab);             ///< 添加描述标签
}