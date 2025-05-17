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
#include <QRandomGenerator>
#include <chrono>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 图片宽度常量（正方形） */
#define IMAGE_WIDTH 102

/**
 * @namespace SearchFlag
 * @brief 搜索标志命名空间，定义单次初始化标志
 */
namespace SearchFlag {
    std::once_flag flag1; ///< 推荐页面初始化标志
    std::once_flag flag2; ///< 排行页面初始化标志
    std::once_flag flag3; ///< 专题页面初始化标志
    std::once_flag flag4; ///< 频道页面初始化标志
}

/**
 * @brief 构造函数，初始化搜索界面
 * @param parent 父控件指针，默认为 nullptr
 */
Search::Search(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Search)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this)) ///< 初始化按钮组
{
    ui->setupUi(this);                                   ///< 设置 UI 布局
    QFile file(GET_CURRENT_DIR + QStringLiteral("/search.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面
    initStackWidget();                                   ///< 初始化堆栈窗口
    this->m_currentBtn = nullptr;                        ///< 初始化当前按钮为空

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 动画结束恢复按钮交互
    enableButton(true);                                  ///< 初始启用按钮
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
Search::~Search()
{
    delete ui;                                           ///< 释放 UI 界面
}

/**
 * @brief 初始化界面
 * @note 设置工具按钮图标、按钮组、下标图片和堆栈动画
 */
void Search::initUi()
{
    // 设置工具按钮图标
    ui->toolButton1->setIcon(QIcon(QStringLiteral(":/Search/Res/search/phonePlay.png"))); ///< 设置手机播放图标
    ui->toolButton2->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-live.png"))); ///< 设置酷狗直播图标
    ui->toolButton3->setIcon(QIcon(QStringLiteral(":/Search/Res/search/wallpaper.png"))); ///< 设置壁纸图标
    ui->toolButton4->setIcon(QIcon(QStringLiteral(":/Search/Res/search/kugou-pingbao.png"))); ///< 设置酷狗评榜图标
    ui->toolButton5->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundEffect.png"))); ///< 设置音效图标
    ui->toolButton6->setIcon(QIcon(QStringLiteral(":/Search/Res/search/soundPlugin.png"))); ///< 设置声音插件图标
    ui->toolButton7->setIcon(QIcon(QStringLiteral(":/Search/Res/search/ringMake.png"))); ///< 设置铃声制作图标
    ui->toolButton8->setIcon(QIcon(QStringLiteral(":/Search/Res/search/remoteControl.png"))); ///< 设置远程控制图标
    ui->toolButton9->setIcon(QIcon(QStringLiteral(":/Search/Res/search/musicCircle.png"))); ///< 设置音乐圈图标
    ui->toolButton10->setIcon(QIcon(QStringLiteral(":/Search/Res/search/cd.png"))); ///< 设置 CD 图标
    ui->toolButton11->setIcon(QIcon(QStringLiteral(":/Search/Res/search/equalizer.png"))); ///< 设置均衡器图标
    ui->toolButton12->setIcon(QIcon(QStringLiteral(":/Search/Res/search/timing.png"))); ///< 设置定时图标
    ui->toolButton13->setIcon(QIcon(QStringLiteral(":/Search/Res/search/DLNA.png"))); ///< 设置 DLNA 图标
    ui->toolButton14->setIcon(QIcon(QStringLiteral(":/Search/Res/search/change.png"))); ///< 设置切换图标
    ui->toolButton15->setIcon(QIcon(QStringLiteral(":/Search/Res/search/netTest.png"))); ///< 设置网络测试图标
    ui->toolButton16->setIcon(QIcon(QStringLiteral(":/Search/Res/search/earnCoin.png"))); ///< 设置赚币图标

    // 设置按钮组互斥
    this->m_buttonGroup->addButton(ui->recommend_pushButton); ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->rank_pushButton);     ///< 添加排行按钮
    this->m_buttonGroup->addButton(ui->special_pushButton);  ///< 添加专题按钮
    this->m_buttonGroup->addButton(ui->channel_pushButton);  ///< 添加频道按钮
    this->m_buttonGroup->setExclusive(true);                 ///< 设置互斥

    // 设置下标图片
    ui->index_label1->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg"))); ///< 设置推荐下标
    ui->index_label2->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg"))); ///< 设置排行下标
    ui->index_label3->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg"))); ///< 设置专题下标
    ui->index_label4->setPixmap(QPixmap(QStringLiteral(":/Search/Res/search/index_lab.svg"))); ///< 设置频道下标
    ui->index_label2->hide();                                ///< 初始隐藏排行下标
    ui->index_label3->hide();                                ///< 初始隐藏专题下标
    ui->index_label4->hide();                                ///< 初始隐藏频道下标

    initCoverVector();                                       ///< 初始化封面库
    initDescVector();                                        ///< 初始化描述库

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置堆栈动画曲线
    ui->stackedWidget->setSpeed(400);                        ///< 设置动画速度
}

/**
 * @brief 初始化堆栈窗口
 * @note 创建推荐、排行、专题和频道页面，并设置布局
 */
void Search::initStackWidget()
{
    this->m_recommendWidget = std::make_unique<QWidget>(ui->stackedWidget); ///< 初始化推荐页面
    this->m_rankWidget = std::make_unique<QWidget>(ui->stackedWidget);      ///< 初始化排行页面
    this->m_specialWidget = std::make_unique<QWidget>(ui->stackedWidget);   ///< 初始化专题页面
    this->m_channelWidget = std::make_unique<QWidget>(ui->stackedWidget);   ///< 初始化频道页面

    auto lay1 = new MyFlowLayout(this->m_recommendWidget.get(), 10, -1, 2); ///< 创建推荐页面布局
    this->m_recommendWidget->setLayout(lay1);
    this->m_recommendWidget->setFixedHeight(1000);          ///< 设置推荐页面高度
    auto lay2 = new ElaFlowLayout(this->m_rankWidget.get(), 5, 8, 6); ///< 创建排行页面布局
    lay2->setIsAnimation(true);                             ///< 启用动画
    this->m_rankWidget->setLayout(lay2);
    this->m_rankWidget->setFixedHeight(500);               ///< 设置排行页面高度
    auto lay3 = new ElaFlowLayout(this->m_specialWidget.get(), 5, 8, 6); ///< 创建专题页面布局
    lay3->setIsAnimation(true);                             ///< 启用动画
    this->m_specialWidget->setLayout(lay3);
    this->m_specialWidget->setFixedHeight(700);            ///< 设置专题页面高度
    auto lay4 = new ElaFlowLayout(this->m_channelWidget.get(), 5, 8, 6); ///< 创建频道页面布局
    lay4->setIsAnimation(true);                             ///< 启用动画
    this->m_channelWidget->setLayout(lay4);
    this->m_channelWidget->setFixedHeight(200);            ///< 设置频道页面高度
    this->m_channelWidget->setObjectName("channelWidget"); ///< 设置频道页面对象名

    ui->stackedWidget->addWidget(this->m_recommendWidget.get()); ///< 添加推荐页面
    ui->stackedWidget->addWidget(this->m_rankWidget.get());      ///< 添加排行页面
    ui->stackedWidget->addWidget(this->m_specialWidget.get());   ///< 添加专题页面
    ui->stackedWidget->addWidget(this->m_channelWidget.get());   ///< 添加频道页面
    ui->stackedWidget->setCurrentWidget(this->m_rankWidget.get()); ///< 设置默认页面为排行
    ui->recommend_pushButton->clicked();                    ///< 触发推荐按钮点击
    on_recommend_pushButton_clicked();                      ///< 初始化推荐页面
}

/**
 * @brief 初始化封面库
 * @note 加载 60 张封面图片
 */
void Search::initCoverVector()
{
    for (int i = 1; i <= 9; ++i)
    {
        this->m_coverVector.emplace_back(QString(":/Search/Res/search/block0%1.png").arg(i)); ///< 添加编号 01-09 图片
    }
    for (int i = 10; i <= 60; ++i)
    {
        this->m_coverVector.emplace_back(QString(":/Search/Res/search/block%1.png").arg(i)); ///< 添加编号 10-60 图片
    }
}

/**
 * @brief 初始化描述库
 * @note 加载描述文本列表
 */
void Search::initDescVector()
{
    QStringList list = {"酷歌词", "抖音潮流区", "开车必备歌曲专区", "抖音DJ",
                       "2021抖音最火歌曲", "DJ必备歌曲", "伤感音乐", "车载DJ", "植物大战僵尸",
                       "抖音热歌", "刀郎老歌合集", "魔道祖师", "邓丽君老歌合集", "学生党专区",
                       "夜听伤感频道", "纯音乐路的尽头会是温柔和月光", "鞠婧祎的歌", "快手抖音最火歌曲集合",
                       "肖战", "KG大神", "我的世界", "神仙翻唱", "岁月陈酿过的粤语老歌", "治愈专区",
                       "林俊杰音乐汇", "第五人格角色曲", "满载回忆的华语经典", "云南山歌-单曲-专辑精选汇聚",
                       "抖音热歌榜", "轻音乐", "睡眠音乐", "游戏高燃", "车载电音缓解疲劳专用", "古风视频专区",
                       "TFBOYS音乐小屋", "纯音乐钢琴", "伤感情歌静静聆听", "名侦探柯南", "DJ龙二少音乐作品",
                       "初音未来", "德云社", "王俊凯免费歌曲不重复", "草原歌后乌兰图雅", "就爱老哥带DJ",
                       "心情治疗诊所", "民谣聚集地", "私藏歌单等你来听", "古风亦可DJ-中国风也能蹦迪",
                       "朴彩英专区", "AW经典电影", "电子音乐", "BLACKPINK", "每日必听的粤语歌单",
                       "薛之谦热歌榜"};
    for (const auto &str : list)
    {
        this->m_descVector.emplace_back(str);             ///< 添加描述文本
    }
}

/**
 * @brief 刷新数据
 * @note 随机打乱封面和描述数据
 */
void Search::refresh()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机种子
    std::shuffle(this->m_coverVector.begin(), this->m_coverVector.end(), std::default_random_engine(seed)); ///< 打乱封面列表
    std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed)); ///< 打乱描述列表
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 * @note 控制推荐、排行、专题和频道按钮的交互性
 */
void Search::enableButton(const bool &flag) const
{
    // @note 未使用，保留用于调试
    // qDebug() << "可交互性：" << flag;
    ui->recommend_pushButton->setEnabled(flag);          ///< 设置推荐按钮交互性
    ui->rank_pushButton->setEnabled(flag);               ///< 设置排行按钮交互性
    ui->special_pushButton->setEnabled(flag);            ///< 设置专题按钮交互性
    ui->channel_pushButton->setEnabled(flag);            ///< 设置频道按钮交互性
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整窗口宽度并触发当前页面刷新
 */
void Search::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类处理
    // @note 未使用，保留用于调试
    // qDebug() << "Search::resizeEvent";
    if (!this->window())
    {
        qWarning() << "无法获取顶级窗口！";            ///< 记录警告日志
        STREAM_WARN() << "无法获取顶级窗口！";
        return;
    }

    const int topLevelWidth = this->window()->width();   ///< 获取顶级窗口宽度
    // @note 未使用，保留用于调试
    // qDebug() << "顶级窗口宽度为：" << topLevelWidth << " 当前窗口宽度为：" << this->width();
    if (this->width() > topLevelWidth)
    {
        auto geo = this->geometry();
        geo.setWidth(topLevelWidth - 10);                ///< 调整窗口宽度
        this->setGeometry(geo);
        // @note 未使用，保留用于调试
        // qDebug() << "修改后顶级窗口宽度为：" << topLevelWidth << " 当前窗口宽度为：" << this->width();
    }
    if (this->m_currentBtn)
    {
        this->m_currentBtn->clicked();                   ///< 触发当前按钮点击
    }
    enableButton(true);                                  ///< 启用按钮
}

/**
 * @brief 显示事件
 * @param event 显示事件
 * @note 触发当前页面刷新
 */
void Search::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);                           ///< 调用父类处理
    if (this->m_currentBtn)
    {
        this->m_currentBtn->clicked();                   ///< 触发当前按钮点击
    }
    enableButton(true);                                  ///< 启用按钮
}

/**
 * @brief 推荐按钮点击槽函数
 * @note 切换到推荐页面并加载 54 个推荐项
 */
void Search::on_recommend_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_recommendWidget.get())
    {
        return;                                          ///< 已为当前页面，直接返回
    }
    ui->index_label1->show();                           ///< 显示推荐下标
    ui->index_label2->hide();                           ///< 隐藏排行下标
    ui->index_label3->hide();                           ///< 隐藏专题下标
    ui->index_label4->hide();                           ///< 隐藏频道下标
    auto handle = [this] {
        refresh();                                       ///< 刷新数据
        auto lay = static_cast<MyFlowLayout *>(this->m_recommendWidget->layout()); ///< 获取布局
        if (!lay)
        {
            // @note 未使用，保留用于调试
            // qDebug() << " m_recommendWidget 布局错误";
            STREAM_ERROR() << " m_recommendWidget 布局错误"; ///< 记录错误日志
            return;
        }
        for (int i = 0; i < 54; ++i)
        {
            auto btn = new QToolButton(this->m_recommendWidget.get()); ///< 创建工具按钮
            btn->setCursor(Qt::PointingHandCursor);      ///< 设置光标
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); ///< 设置文本在图标下方
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH)); ///< 设置图标大小
            btn->setIcon(this->m_coverVector[i]);        ///< 设置封面
            QFont font(btn->font().family(), 10);        ///< 设置字体
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];           ///< 获取描述
            auto elidedText = fm.elidedText(text, Qt::ElideRight, IMAGE_WIDTH); ///< 计算省略文本
            if (elidedText != text)
            {
                btn->setToolTip(text);                   ///< 设置完整文本为工具提示
            }
            btn->setText(elidedText);                    ///< 设置省略文本
            lay->addWidget(btn);                         ///< 添加到布局
        }
    };
    std::call_once(SearchFlag::flag1, handle);           ///< 单次初始化
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_recommendWidget.get())); ///< 切换到推荐页面
    ui->stackedWidget->setFixedHeight(this->m_recommendWidget->height()); ///< 设置堆栈窗口高度
    this->m_currentBtn = ui->recommend_pushButton;       ///< 更新当前按钮
}

/**
 * @brief 排行按钮点击槽函数
 * @note 切换到排行页面并加载 19 个推荐项
 */
void Search::on_rank_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_rankWidget.get())
    {
        return;                                          ///< 已为当前页面，直接返回
    }
    enableButton(false);                                 ///< 禁用按钮
    ui->index_label1->hide();                           ///< 隐藏推荐下标
    ui->index_label2->show();                           ///< 显示排行下标
    ui->index_label3->hide();                           ///< 隐藏专题下标
    ui->index_label4->hide();                           ///< 隐藏频道下标
    auto handle = [this] {
        refresh();                                       ///< 刷新数据
        auto lay = static_cast<ElaFlowLayout *>(this->m_rankWidget->layout()); ///< 获取布局
        if (!lay)
        {
            // @note 未使用，保留用于调试
            // qDebug() << " m_rankWidget 布局错误";
            STREAM_ERROR() << " m_rankWidget 布局错误";  ///< 记录错误日志
            return;
        }
        for (int i = 0; i < 19; ++i)
        {
            auto btn = new QToolButton(this->m_rankWidget.get()); ///< 创建工具按钮
            btn->setCursor(Qt::PointingHandCursor);      ///< 设置光标
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); ///< 设置文本在图标下方
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH)); ///< 设置图标大小
            btn->setIcon(this->m_coverVector[i]);        ///< 设置封面
            QFont font(btn->font().family(), 10);        ///< 设置字体
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];           ///< 获取描述
            auto elidedText = fm.elidedText(text, Qt::ElideRight, IMAGE_WIDTH); ///< 计算省略文本
            if (elidedText != text)
            {
                btn->setToolTip(text);                   ///< 设置完整文本为工具提示
            }
            btn->setText(elidedText);                    ///< 设置省略文本
            lay->addWidget(btn);                         ///< 添加到布局
        }
    };
    std::call_once(SearchFlag::flag2, handle);           ///< 单次初始化
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_rankWidget.get())); ///< 切换到排行页面
    ui->stackedWidget->setFixedHeight(this->m_rankWidget->height()); ///< 设置堆栈窗口高度
    this->m_currentBtn = ui->rank_pushButton;            ///< 更新当前按钮
}

/**
 * @brief 专题按钮点击槽函数
 * @note 切换到专题页面并加载 27 个推荐项
 */
void Search::on_special_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_specialWidget.get())
    {
        return;                                          ///< 已为当前页面，直接返回
    }
    enableButton(false);                                 ///< 禁用按钮
    ui->index_label1->hide();                           ///< 隐藏推荐下标
    ui->index_label2->hide();                           ///< 隐藏排行下标
    ui->index_label3->show();                           ///< 显示专题下标
    ui->index_label4->hide();                           ///< 隐藏频道下标
    auto handle = [this] {
        refresh();                                       ///< 刷新数据
        auto lay = static_cast<ElaFlowLayout *>(this->m_specialWidget->layout()); ///< 获取布局
        if (!lay)
        {
            // @note 未使用，保留用于调试
            // qDebug() << " m_specialWidget 布局错误";
            STREAM_ERROR() << " m_specialWidget 布局错误"; ///< 记录错误日志
            return;
        }
        for (int i = 0; i < 27; ++i)
        {
            auto btn = new QToolButton(this->m_specialWidget.get()); ///< 创建工具按钮
            btn->setCursor(Qt::PointingHandCursor);      ///< 设置光标
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); ///< 设置文本在图标下方
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH)); ///< 设置图标大小
            btn->setIcon(this->m_coverVector[i]);        ///< 设置封面
            QFont font(btn->font().family(), 10);        ///< 设置字体
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];           ///< 获取描述
            auto elidedText = fm.elidedText(text, Qt::ElideRight, IMAGE_WIDTH); ///< 计算省略文本
            if (elidedText != text)
            {
                btn->setToolTip(text);                   ///< 设置完整文本为工具提示
            }
            btn->setText(elidedText);                    ///< 设置省略文本
            lay->addWidget(btn);                         ///< 添加到布局
        }
    };
    std::call_once(SearchFlag::flag3, handle);           ///< 单次初始化
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_specialWidget.get())); ///< 切换到专题页面
    ui->stackedWidget->setFixedHeight(this->m_specialWidget->height()); ///< 设置堆栈窗口高度
    this->m_currentBtn = ui->special_pushButton;         ///< 更新当前按钮
}

/**
 * @brief 频道按钮点击槽函数
 * @note 切换到频道页面并加载 7 个推荐项
 */
void Search::on_channel_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_channelWidget.get())
    {
        return;                                          ///< 已为当前页面，直接返回
    }
    enableButton(false);                                 ///< 禁用按钮
    ui->index_label1->hide();                           ///< 隐藏推荐下标
    ui->index_label2->hide();                           ///< 隐藏排行下标
    ui->index_label3->hide();                           ///< 隐藏专题下标
    ui->index_label4->show();                           ///< 显示频道下标
    auto handle = [this] {
        refresh();                                       ///< 刷新数据
        auto lay = static_cast<ElaFlowLayout *>(this->m_channelWidget->layout()); ///< 获取布局
        if (!lay)
        {
            // @note 未使用，保留用于调试
            // qDebug() << " m_channelWidget 布局错误";
            STREAM_ERROR() << " m_channelWidget 布局错误"; ///< 记录错误日志
            return;
        }
        for (int i = 0; i < 7; ++i)
        {
            auto btn = new QToolButton(this->m_channelWidget.get()); ///< 创建工具按钮
            btn->setCursor(Qt::PointingHandCursor);      ///< 设置光标
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); ///< 设置文本在图标下方
            btn->setIconSize(QSize(IMAGE_WIDTH, IMAGE_WIDTH)); ///< 设置图标大小
            btn->setIcon(this->m_coverVector[i]);        ///< 设置封面
            QFont font(btn->font().family(), 10);        ///< 设置字体
            QFontMetrics fm(font);
            auto text = this->m_descVector[i];           ///< 获取描述
            auto elidedText = fm.elidedText(text, Qt::ElideRight, IMAGE_WIDTH); ///< 计算省略文本
            if (elidedText != text)
            {
                btn->setToolTip(text);                   ///< 设置完整文本为工具提示
            }
            btn->setText(elidedText);                    ///< 设置省略文本
            lay->addWidget(btn);                         ///< 添加到布局
        }
    };
    std::call_once(SearchFlag::flag4, handle);           ///< 单次初始化
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_channelWidget.get())); ///< 切换到频道页面
    ui->stackedWidget->setFixedHeight(this->m_channelWidget->height()); ///< 设置堆栈窗口高度
    this->m_currentBtn = ui->channel_pushButton;         ///< 更新当前按钮
    // @note 未使用，保留用于调试
    // qDebug() << "this->height() : " << this->height() << "this->m_channelWidget->height() : " << this->m_channelWidget->height();
}