/**
 * @file Search.cpp
 * @brief 实现 Search 类，管理搜索界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "Search.h"
#include "ui_Search.h"
#include "ElaFlowLayout.h"
#include "MyFlowLayout.h"
#include "logger.hpp"

#include <QFile>
#include <QButtonGroup>
#include <chrono>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 图片宽度常量（正方形） */
#define IMAGE_WIDTH 102

/**
 * @brief 构造函数，初始化搜索界面
 * @param parent 父控件指针，默认为 nullptr
 */
Search::Search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Search)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/search.css"));
    if (file.open(QIODevice::ReadOnly))
    {
        setStyleSheet(file.readAll());
    }
    else
    {
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    initStackedWidget();
    m_currentBtn = nullptr;

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
Search::~Search()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* Search::createPage(int id)
{
    QWidget* page = nullptr;
    QLayout* lay = nullptr;
    int itemCount = 0;
    int height = 0;
    QString objectName;

    switch (id) {
        case 0: // Recommend
            if (!m_recommendWidget) {
                m_recommendWidget = std::make_unique<QWidget>(ui->stackedWidget);
                lay = new MyFlowLayout(m_recommendWidget.get(), 10, -1, 2);
                m_recommendWidget->setLayout(lay);
                m_recommendWidget->setFixedHeight(1000);
                itemCount = 54;
            }
            page = m_recommendWidget.get();
            break;
        case 1: // Rank
            if (!m_rankWidget) {
                m_rankWidget = std::make_unique<QWidget>(ui->stackedWidget);
                lay = new ElaFlowLayout(m_rankWidget.get(), 5, 8, 6);
                static_cast<ElaFlowLayout*>(lay)->setIsAnimation(true);
                m_rankWidget->setLayout(lay);
                m_rankWidget->setFixedHeight(500);
                itemCount = 19;
            }
            page = m_rankWidget.get();
            break;
        case 2: // Special
            if (!m_specialWidget) {
                m_specialWidget = std::make_unique<QWidget>(ui->stackedWidget);
                lay = new ElaFlowLayout(m_specialWidget.get(), 5, 8, 6);
                static_cast<ElaFlowLayout*>(lay)->setIsAnimation(true);
                m_specialWidget->setLayout(lay);
                m_specialWidget->setFixedHeight(700);
                itemCount = 27;
            }
            page = m_specialWidget.get();
            break;
        case 3: // Channel
            if (!m_channelWidget) {
                m_channelWidget = std::make_unique<QWidget>(ui->stackedWidget);
                lay = new ElaFlowLayout(m_channelWidget.get(), 5, 8, 6);
                static_cast<ElaFlowLayout*>(lay)->setIsAnimation(true);
                m_channelWidget->setLayout(lay);
                m_channelWidget->setFixedHeight(200);
                m_channelWidget->setObjectName("channelWidget");
                itemCount = 7;
            }
            page = m_channelWidget.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }

    if (lay && itemCount > 0) {
        refresh();
        for (int i = 0; i < itemCount; ++i) {
            auto btn = new QToolButton(page);
            btn->setCursor(Qt::PointingHandCursor);
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH));
            btn->setIcon(m_coverVector[i]);
            QFont font(btn->font().family(), 10);
            QFontMetrics fm(font);
            auto text = m_descVector[i];
            auto elidedText = fm.elidedText(text, Qt::ElideRight, IMAGE_WIDTH);
            if (elidedText != text) {
                btn->setToolTip(text);
            }
            btn->setText(elidedText);
            lay->addWidget(btn);
        }
    }

    return page;
}

/**
 * @brief 初始化界面
 * @note 设置工具按钮图标、按钮组和下标图片
 */
void Search::initUi()
{
    QToolButton* toolButtons[] = {
        ui->toolButton1, ui->toolButton2, ui->toolButton3, ui->toolButton4,
        ui->toolButton5, ui->toolButton6, ui->toolButton7, ui->toolButton8,
        ui->toolButton9, ui->toolButton10, ui->toolButton11, ui->toolButton12,
        ui->toolButton13, ui->toolButton14, ui->toolButton15, ui->toolButton16
    };
    const QString iconPaths[] = {
        ":/Search/Res/search/phonePlay.png",
        ":/Search/Res/search/kugou-live.png",
        ":/Search/Res/search/wallpaper.png",
        ":/Search/Res/search/kugou-pingbao.png",
        ":/Search/Res/search/soundEffect.png",
        ":/Search/Res/search/soundPlugin.png",
        ":/Search/Res/search/ringMake.png",
        ":/Search/Res/search/remoteControl.png",
        ":/Search/Res/search/musicCircle.png",
        ":/Search/Res/search/cd.png",
        ":/Search/Res/search/equalizer.png",
        ":/Search/Res/search/timing.png",
        ":/Search/Res/search/DLNA.png",
        ":/Search/Res/search/change.png",
        ":/Search/Res/search/netTest.png",
        ":/Search/Res/search/earnCoin.png"
    };

    for (int i = 0; i < 16; ++i) {
        toolButtons[i]->setIcon(QIcon(iconPaths[i]));
    }

    m_buttonGroup->addButton(ui->recommend_pushButton, 0);
    m_buttonGroup->addButton(ui->rank_pushButton, 1);
    m_buttonGroup->addButton(ui->special_pushButton, 2);
    m_buttonGroup->addButton(ui->channel_pushButton, 3);
    m_buttonGroup->setExclusive(true);

    QLabel* idxLabels[] = { ui->index_label1, ui->index_label2, ui->index_label3, ui->index_label4 };
    for (int i = 0; i < 4; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Search/Res/search/index_lab.svg"));
        idxLabels[i]->setVisible(i == 0);
    }

    initCoverVector();
    initDescVector();

    ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief 初始化堆栈窗口
 * @note 创建推荐、排行、专题和频道页面
 */
void Search::initStackedWidget()
{
    for (int i = 0; i < 4; ++i) {
        auto* placeholder = new QWidget;
        auto* layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    m_pages[0]->layout()->addWidget(createPage(0));
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setFixedHeight(m_recommendWidget->height());
    m_currentBtn = ui->recommend_pushButton;

    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](int id) {
        if (m_currentIdx == id) {
            return;
        }

        enableButton(false);

        QWidget* placeholder = m_pages[m_currentIdx];
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }

        QLayout* layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem* item = layout->takeAt(0)) {
                if (QWidget* widget = item->widget()) {
                    qDebug()<<"删除旧页面";
                    widget->deleteLater();
                    switch (m_currentIdx) {
                        case 0: m_recommendWidget.reset();break;
                        case 1: m_rankWidget.reset();break;
                        case 2: m_specialWidget.reset();break;
                        case 3: m_channelWidget.reset();break;
                        default: break;
                    }
                }
                delete item;
            }
        }

        placeholder = m_pages[id];
        layout = placeholder->layout();

        QWidget* realPage = createPage(id);
        if (!realPage) {
            qWarning() << "[WARNING] Failed to create page at index:" << id;
        } else {
            layout->addWidget(realPage);
            ui->stackedWidget->setFixedHeight(realPage->height());
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;
        m_currentBtn = static_cast<QPushButton *>(m_buttonGroup->button(id));

        QLabel* idxLabels[] = { ui->index_label1, ui->index_label2, ui->index_label3, ui->index_label4 };
        for (int i = 0; i < 4; ++i) {
            idxLabels[i]->setVisible(i == id);
        }

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });

    ui->recommend_pushButton->click();
}

/**
 * @brief 初始化封面库
 * @note 加载 60 张封面图片
 */
void Search::initCoverVector()
{
    for (int i = 1; i <= 9; ++i) {
        m_coverVector.emplace_back(QString(":/Search/Res/search/block0%1.png").arg(i));
    }
    for (int i = 10; i <= 60; ++i) {
        m_coverVector.emplace_back(QString(":/Search/Res/search/block%1.png").arg(i));
    }
}

/**
 * @brief 初始化描述库
 * @note 加载描述文本列表
 */
void Search::initDescVector()
{
    QStringList list = {
        "酷歌词", "抖音潮流区", "开车必备歌曲专区", "抖音DJ", "2021抖音最火歌曲", "DJ必备歌曲",
        "伤感音乐", "车载DJ", "植物大战僵尸", "抖音热歌", "刀郎老歌合集", "魔道祖师",
        "邓丽君老歌合集", "学生党专区", "夜听伤感频道", "纯音乐路的尽头会是温柔和月光",
        "鞠婧祎的歌", "快手抖音最火歌曲集合", "肖战", "KG大神", "我的世界", "神仙翻唱",
        "岁月陈酿过的粤语老歌", "治愈专区", "林俊杰音乐汇", "第五人格角色曲", "满载回忆的华语经典",
        "云南山歌-单曲-专辑精选汇聚", "抖音热歌榜", "轻音乐", "睡眠音乐", "游戏高燃",
        "车载电音缓解疲劳专用", "古风视频专区", "TFBOYS音乐小屋", "纯音乐钢琴",
        "伤感情歌静静聆听", "名侦探柯南", "DJ龙二少音乐作品", "初音未来", "德云社",
        "王俊凯免费歌曲不重复", "草原歌后乌兰图雅", "就爱老哥带DJ", "心情治疗诊所",
        "民谣聚集地", "私藏歌单等你来听", "古风亦可DJ-中国风也能蹦迪", "朴彩英专区",
        "AW经典电影", "电子音乐", "BLACKPINK", "每日必听的粤语歌单", "薛之谦热歌榜"
    };
    for (const auto &str : list) {
        m_descVector.emplace_back(str);
    }
}

/**
 * @brief 刷新数据
 * @note 随机打乱封面和描述数据
 */
void Search::refresh()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_coverVector.begin(), m_coverVector.end(), std::default_random_engine(seed));
    std::shuffle(m_descVector.begin(), m_descVector.end(), std::default_random_engine(seed));
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 * @note 控制按钮交互性
 */
void Search::enableButton(bool flag) const
{
    ui->recommend_pushButton->setEnabled(flag);
    ui->rank_pushButton->setEnabled(flag);
    ui->special_pushButton->setEnabled(flag);
    ui->channel_pushButton->setEnabled(flag);
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整窗口宽度和触发当前页面刷新
 */
void Search::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!window()) {
        qWarning() << "无法获取顶级窗口！";
        STREAM_WARN() << "无法获取顶级窗口！";
        return;
    }

    const int topLevelWidth = window()->width();
    if (width() > topLevelWidth) {
        auto geo = geometry();
        geo.setWidth(topLevelWidth - 10);
        setGeometry(geo);
    }
    if (m_currentBtn) {
        m_currentBtn->click();
    }
    enableButton(true);
}

/**
 * @brief 显示事件
 * @param event 显示事件
 * @note 触发当前页面刷新
 */
void Search::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (m_currentBtn) {
        m_currentBtn->click();
    }
    resize(size());
    enableButton(true);
}