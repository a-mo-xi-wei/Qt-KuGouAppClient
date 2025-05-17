/**
 * @file ListenRecommend.cpp
 * @brief 实现 ListenRecommend 类，提供推荐界面功能
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#include "ListenRecommend.h"
#include "ui_ListenRecommend.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "Async.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 画廊索引数组
 */
static int idx[20] = {0};

/**
 * @brief 当前刷新对象
 */
static ListenTableWidget *refreshObj{};

/**
 * @brief 构造函数，初始化推荐界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenRecommend::ListenRecommend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenRecommend)
    , m_refreshTimer(new QTimer(this))
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();                                            ///< 初始化界面
    auto menu = new MyMenu(MyMenu::MenuKind::ListenOption, this); ///< 创建菜单
    m_menu = menu->getMenu<ListenOptionMenu>();          ///< 获取分类菜单
    connect(m_menu, &ListenOptionMenu::clickedFuncName, this, &ListenRecommend::onMenuFuncClicked); ///< 连接菜单点击信号
    connect(ui->daily_recommend_widget, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked); ///< 连接每日推荐刷新信号
    connect(this->m_refreshTimer, &QTimer::timeout, this, &ListenRecommend::onRefreshTimeout); ///< 连接定时器超时信号
}

/**
 * @brief 析构函数，清理资源
 */
ListenRecommend::~ListenRecommend()
{
    delete ui;
}

/**
 * @brief 初始化界面
 * @note 设置全部分类按钮、定时器和画廊
 */
void ListenRecommend::initUi()
{
    ui->all_classify_toolButton->setHoverFontColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停字体颜色
    ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg"))); ///< 设置默认图标
    ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置悬停图标
    ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-black.svg"))); ///< 设置离开图标
    ui->all_classify_toolButton->setIconSize(QSize(10, 10)); ///< 设置图标大小
    ui->all_classify_toolButton->setApproach(true);      ///< 启用接近效果
    this->m_refreshTimer->setSingleShot(true);           ///< 设置定时器单次触发
    initDailyRecommendGalleryWidget();                   ///< 初始化每日推荐画廊
    initTableWidgets();                                  ///< 初始化其他表格控件
    QList<QToolButton *> buttons = ui->classify_widget->findChildren<QToolButton *>(); ///< 获取分类按钮
    for (const auto &button : buttons)
    {
        connect(button, &QToolButton::clicked, this, [this, button] {
            ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                                       QString("%1 功能未实现 敬请期待").arg(button->text()), 1000, this->window()); ///< 显示未实现提示
        });
    }
}

/**
 * @brief 初始化表格控件
 * @note 创建并布局多个分类表格
 */
void ListenRecommend::initTableWidgets()
{
    const auto lay = new QVBoxLayout(ui->table_widgets); ///< 创建垂直布局
    lay->setContentsMargins(0, 0, 0, 0);                ///< 设置布局边距
    lay->setSpacing(0);                                  ///< 设置控件间距
    // 有声小说
    const auto audioNovel = new ListenTableWidget(ui->table_widgets);
    audioNovel->setCnt(1);                               ///< 设置计数
    audioNovel->setTitle("有声小说");                    ///< 设置标题
    connect(audioNovel, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked); ///< 连接刷新信号
    initOtherGalleryWidget("audioNovel", audioNovel);    ///< 初始化画廊
    // 儿童天地
    const auto childrenWorld = new ListenTableWidget(ui->table_widgets);
    childrenWorld->setCnt(2);
    childrenWorld->setTitle("儿童天地");
    connect(childrenWorld, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("childrenWorld", childrenWorld);
    // 评书
    const auto commentBook = new ListenTableWidget(ui->table_widgets);
    commentBook->setCnt(3);
    commentBook->setTitle("评书");
    connect(commentBook, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("commentBook", commentBook);
    // 助眠解压
    const auto helpSleep = new ListenTableWidget(ui->table_widgets);
    helpSleep->setCnt(4);
    helpSleep->setTitle("助眠解压");
    connect(helpSleep, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("sleepHelp", helpSleep);
    // 人文
    const auto humanities = new ListenTableWidget(ui->table_widgets);
    humanities->setCnt(5);
    humanities->setTitle("人文");
    connect(humanities, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("humanity", humanities);
    // 自我充电
    const auto selfCharge = new ListenTableWidget(ui->table_widgets);
    selfCharge->setCnt(6);
    selfCharge->setTitle("自我充电");
    connect(selfCharge, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("chongdian", selfCharge);
    // 相声曲艺
    const auto voiceArt = new ListenTableWidget(ui->table_widgets);
    voiceArt->setCnt(7);
    voiceArt->setTitle("相声曲艺");
    connect(voiceArt, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("xiangsheng", voiceArt);
    // 情感生活
    const auto emotionLife = new ListenTableWidget(ui->table_widgets);
    emotionLife->setCnt(8);
    emotionLife->setTitle("情感生活");
    connect(emotionLife, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("qinggan", emotionLife);
    // 广播剧
    const auto radioDrama = new ListenTableWidget(ui->table_widgets);
    radioDrama->setCnt(9);
    radioDrama->setTitle("广播剧");
    connect(radioDrama, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("guangboju", radioDrama);
    // 娱乐段子
    const auto entertainmentJoke = new ListenTableWidget(ui->table_widgets);
    entertainmentJoke->setCnt(10);
    entertainmentJoke->setTitle("娱乐段子");
    connect(entertainmentJoke, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("yule", entertainmentJoke);
    // 二次元
    const auto anime = new ListenTableWidget(ui->table_widgets);
    anime->setCnt(11);
    anime->setTitle("二次元");
    connect(anime, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("erciyuan", anime);
    // 播客
    const auto podcast = new ListenTableWidget(ui->table_widgets);
    podcast->setCnt(12);
    podcast->setTitle("播客");
    connect(podcast, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("boke", podcast);
    // 粤语
    const auto yue = new ListenTableWidget(ui->table_widgets);
    yue->setCnt(13);
    yue->setTitle("粤语");
    connect(yue, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("yueyu", yue);
    // 外语
    const auto foreignLanguage = new ListenTableWidget(ui->table_widgets);
    foreignLanguage->setCnt(14);
    foreignLanguage->setTitle("外语");
    connect(foreignLanguage, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("waiyu", foreignLanguage);
    // 创作翻唱
    const auto creativeSing = new ListenTableWidget(ui->table_widgets);
    creativeSing->setCnt(15);
    creativeSing->setTitle("创作翻唱");
    connect(creativeSing, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("createCover", creativeSing);
    // DJ电音
    const auto djElectronic = new ListenTableWidget(ui->table_widgets);
    djElectronic->setCnt(16);
    djElectronic->setTitle("DJ电音");
    connect(djElectronic, &ListenTableWidget::toolBtnClicked, this, &ListenRecommend::onToolButtonClicked);
    initOtherGalleryWidget("djElectronic", djElectronic);
    lay->addWidget(audioNovel);                          ///< 添加表格控件
    lay->addWidget(childrenWorld);
    lay->addWidget(commentBook);
    lay->addWidget(helpSleep);
    lay->addWidget(humanities);
    lay->addWidget(selfCharge);
    lay->addWidget(voiceArt);
    lay->addWidget(emotionLife);
    lay->addWidget(radioDrama);
    lay->addWidget(entertainmentJoke);
    lay->addWidget(anime);
    lay->addWidget(podcast);
    lay->addWidget(yue);
    lay->addWidget(foreignLanguage);
    lay->addWidget(creativeSing);
    lay->addWidget(djElectronic);
}

/**
 * @brief 初始化每日推荐画廊
 * @note 异步加载 JSON 数据并填充画廊
 */
void ListenRecommend::initDailyRecommendGalleryWidget()
{
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/jsonFiles/dailyRecommend.json"); ///< JSON 文件路径
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [jsonPath] {
        QList<QPair<QString, QString>> result;           ///< 数据列表
        QFile file(jsonPath);                            ///< 打开 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading dailyRecommend.json";
            STREAM_WARN() << "Failed to open JSON file:" << jsonPath.toStdString(); ///< 记录警告日志
            return result;
        }
        QJsonParseError parseError;
        const auto doc = QJsonDocument::fromJson(file.readAll(), &parseError); ///< 解析 JSON
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "JSON parse error:" << parseError.errorString();
            STREAM_WARN() << "JSON parse error:" << parseError.errorString().toStdString(); ///< 记录错误日志
            return result;
        }
        const auto arr = doc.array();
        for (const auto &item : arr)
        {
            const auto obj = item.toObject();
            result.append(qMakePair(
                obj.value("title").toString(),
                obj.value("play_count").toString()
            ));                                          ///< 添加标题和播放量
        }
        file.close();
        return result;
    });
    Async::onResultReady(future, this, [this](const QList<QPair<QString, QString>> &data) {
        if (data.isEmpty())
        {
            qWarning() << "Daily recommend data is empty or failed to parse";
            STREAM_WARN() << "Daily recommend data is empty or failed to parse"; ///< 记录警告日志
            return;
        }
        this->m_galleryVector[0] = std::vector(data.cbegin(), data.cend()); ///< 转换为向量
        const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。"; ///< 默认描述
        for (int i = 0; i < 10; ++i)
        {
            const auto it = new GalleryPhotoWidget(ui->daily_recommend_widget->getGalleryWidget()); ///< 创建照片卡片
            it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + idx[0])); ///< 设置封面
            it->setTitleText(this->m_galleryVector[0][idx[0]].first); ///< 设置标题
            it->setPopularText(this->m_galleryVector[0][idx[0]].second); ///< 设置流行度
            it->setDescribeText(subTitle);                   ///< 设置描述
            ui->daily_recommend_widget->getGalleryWidget()->addData(it); ///< 添加到画廊
            idx[0] = ++idx[0] % static_cast<int>(this->m_galleryVector[0].size()); ///< 更新索引
        }
    });
}

/**
 * @brief 初始化其他分类画廊
 * @param jsonFileName JSON 文件名
 * @param gallery 表格控件指针
 * @note 异步加载 JSON 数据并填充画廊
 */
void ListenRecommend::initOtherGalleryWidget(const QString &jsonFileName, const ListenTableWidget *gallery)
{
    const auto cnt = gallery->getCnt();                  ///< 获取计数
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [jsonFileName, cnt]() {
        QList<QPair<QString, QString>> result;           ///< 数据列表
        QFile file(GET_CURRENT_DIR + QString("/jsonFiles/%1.json").arg(jsonFileName)); ///< 打开 JSON 文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << QString("Could not open file for reading %1.json").arg(jsonFileName);
            STREAM_WARN() << QString("Could not open file for reading %1.json").arg(jsonFileName).toStdString(); ///< 记录警告日志
            return result;
        }
        const auto doc = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr = doc.array();
        for (const auto &item : arr)
        {
            QString title = item.toObject().value("desc").toString();
            QString playCount = item.toObject().value("people").toString();
            result.append(qMakePair(title, playCount));  ///< 添加描述和人数
        }
        file.close();
        return result;
    });
    Async::onResultReady(future, this, [this, cnt, gallery](const QList<QPair<QString, QString>> &data) {
        if (data.isEmpty())
        {
            qWarning() << QString("%1.json is empty or failed to parse").arg(gallery->objectName());
            STREAM_WARN() << QString("%1.json is empty or failed to parse").arg(gallery->objectName()).toStdString(); ///< 记录警告日志
            return;
        }
        this->m_galleryVector[cnt] = std::vector(data.cbegin(), data.cend()); ///< 转换为向量
        const QString subTitle = "哈,哈,哈,没有提示文本哦,官网爬不到,我是搬砖的小行家,哒哒哒,哒哒哒。。。"; ///< 默认描述
        for (int i = 0; i < 10; ++i)
        {
            const auto it = new GalleryPhotoWidget(gallery->getGalleryWidget()); ///< 创建照片卡片
            it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + cnt * 40 + idx[cnt])); ///< 设置封面
            it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first); ///< 设置标题
            it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second); ///< 设置流行度
            it->setDescribeText(subTitle);                   ///< 设置描述
            gallery->getGalleryWidget()->addData(it);        ///< 添加到画廊
            idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size()); ///< 更新索引
        }
    });
}

/**
 * @brief 全部分类按钮点击槽函数
 * @note 显示分类菜单并切换图标
 */
void ListenRecommend::on_all_classify_toolButton_clicked()
{
    if (ui->all_classify_toolButton->isChecked())
    {
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg"))); ///< 设置向上图标
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        const QPoint globalPos = ui->all_classify_toolButton->mapToGlobal(
            QPoint(ui->all_classify_toolButton->width() - m_menu->width(),
                   ui->all_classify_toolButton->height() + 10)); ///< 计算菜单位置
        m_menu->setFocusPolicy(Qt::NoFocus);             ///< 设置无焦点
        m_menu->setAttribute(Qt::WA_TransparentForMouseEvents, false); ///< 启用鼠标交互
        connect(m_menu, &QMenu::aboutToHide, this, [this]() {
            ui->all_classify_toolButton->setChecked(false);
            ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 恢复向下图标
            ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
            ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        });
        m_menu->exec(globalPos);                         ///< 显示菜单
    }
    else
    {
        ui->all_classify_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置向下图标
        ui->all_classify_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_classify_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    }
}

/**
 * @brief 刷新按钮点击槽函数
 * @note 启动定时器延迟刷新
 */
void ListenRecommend::onToolButtonClicked()
{
    if (!this->m_refreshTimer->isActive())
    {
        this->m_refreshTimer->start(500);                ///< 启动 500ms 定时器
    }
    refreshObj = qobject_cast<ListenTableWidget *>(sender()); ///< 记录刷新对象
}

/**
 * @brief 刷新定时器超时槽函数
 * @note 更新画廊内容并显示成功提示
 */
void ListenRecommend::onRefreshTimeout()
{
    const auto cnt = refreshObj->getCnt();               ///< 获取计数
    for (const auto &it : refreshObj->getGalleryWidget()->getWidgets())
    {
        it->setCoverPix(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(10 + cnt * 40 + idx[cnt] % 40)); ///< 更新封面
        it->setTitleText(this->m_galleryVector[cnt][idx[cnt]].first); ///< 更新标题
        it->setPopularText(this->m_galleryVector[cnt][idx[cnt]].second); ///< 更新流行度
        it->update();                                    ///< 刷新卡片
        idx[cnt] = ++idx[cnt] % static_cast<int>(this->m_galleryVector[cnt].size()); ///< 更新索引
    }
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success",
                           refreshObj->getTitle() + " 换一批成功", 1000, this->window()); ///< 显示成功提示
}

/**
 * @brief 菜单功能点击槽函数
 * @param funcName 功能名称
 * @note 显示功能未实现的提示
 */
void ListenRecommend::onMenuFuncClicked(const QString &funcName)
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(funcName), 1000, this->window()); ///< 显示未实现提示
}