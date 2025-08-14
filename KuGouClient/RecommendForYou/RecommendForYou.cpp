/**
 * @file RecommendForYou.cpp
 * @brief 实现 RecommendForYou 类，管理推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "RecommendForYou.h"
#include "ui_RecommendForYou.h"
#include "TableWidget.h"
#include "logger.hpp"

#include <QDir>
#include <QQueue>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化推荐界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecommendForYou::RecommendForYou(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::RecommendForYou) ///< 初始化 UI 界面
{
    ui->setupUi(this); ///< 设置 UI 布局
    setFont(QFont("TaiwanPearl"));
    QFile file(GET_CURRENT_DIR + QStringLiteral("/recommend.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll()); ///< 应用样式表
    } else {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
        return;
    }

    {
        using Task = std::function<void()>;
        QVector<Task> tasks;

        tasks << [this] { initAdvertiseBoard(); };
        tasks << [this] { initClassifyWidget(); };
        tasks << [this] {
            initTabWidget();
            QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);
        };

        auto queue = std::make_shared<QQueue<Task>>();
        for (const auto &task : tasks)
            queue->enqueue(task);

        auto runner = std::make_shared<std::function<void()>>();
        *runner = [queue, runner]() {
            if (queue->isEmpty())
                return;

            auto task = queue->dequeue();
            QTimer::singleShot(0,
                               nullptr,
                               [task, runner]() {
                                   task();
                                   (*runner)();
                               });
        };

        (*runner)();
    }
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
RecommendForYou::~RecommendForYou()
{
    delete ui; ///< 释放 UI 界面
}

/**
 * @brief 初始化广告轮播
 * @note 加载海报图片并添加到广告板
 */
void RecommendForYou::initAdvertiseBoard() const
{
    QDir dir(__FILE__); ///< 获取当前文件目录
    dir.cdUp();         ///< 向上两级目录
    dir.cdUp();
    dir.cdUp();
    // @note 未使用，保留用于调试
    // qDebug() << "当前目录：" << dir.dirName();
    dir.cd("Res_Qrc/Res/recommend/poster"); ///< 进入海报目录
    // @note 未使用，保留用于调试
    // qDebug() << "当前目录：" << dir.dirName();

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size(); ///< 获取文件数量
    // @note 未使用，保留用于调试
    // qDebug() << "共有: " << s << " 条数据";
    for (auto i = 1; i <= s; ++i) {
        QString path = QString(":/RecommendForYou/Res/recommend/poster/%1.jpg").arg(i); ///< 构造海报路径
        // @note 未使用，保留用于调试
        // qDebug() << "图片路径为：" << path;
        // const QPixmap pix(path);
        // if (pix.isNull()) qDebug() << "图像错误";
        ui->advertise_board_widget->addPoster(path); ///< 添加海报
    }
}

/**
 * @brief 初始化分类按钮
 * @note 设置分类按钮的图标和动态大小
 */
void RecommendForYou::initClassifyWidget() const
{
    // @note 未使用，保留用于调试
    // QIcon ico = QIcon(":/RecommendForYou/recommend/tabIcon/rili.svg");
    // if (ico.isNull()) qDebug() << "++++++++++ico 为空++++++++++++++";
    // QFile file(":/RecommendForYou/recommend/tabIcon/rili.svg");
    // qDebug() << "File Exists:" << file.exists();

    ui->recommend_toolButton->setChangeSize(true);     ///< 设置推荐按钮可变大小
    ui->ranking_list_toolButton->setChangeSize(true);  ///< 设置排行榜按钮可变大小
    ui->classify_toolButton->setChangeSize(true);      ///< 设置分类按钮可变大小
    ui->scene_music_toolButton->setChangeSize(true);   ///< 设置场景音乐按钮可变大小
    ui->music_quality_toolButton->setChangeSize(true); ///< 设置音乐品质按钮可变大小

    ui->recommend_toolButton->setIcon(
        QIcon(QStringLiteral(":/RecommendForYou/Res/recommend/tabIcon/rili.svg"))); ///< 设置推荐按钮图标
    ui->recommend_toolButton->setEnterIconSize(QSize(35, 35));                      ///< 设置悬停图标大小
    ui->recommend_toolButton->setLeaveIconSize(QSize(30, 30));                      ///< 设置离开图标大小

    ui->ranking_list_toolButton->setIcon(
        QIcon(QStringLiteral(":/RecommendForYou/Res/recommend/tabIcon/rank.svg"))); ///< 设置排行榜按钮图标
    ui->ranking_list_toolButton->setEnterIconSize(QSize(40, 40));                   ///< 设置悬停图标大小
    ui->ranking_list_toolButton->setLeaveIconSize(QSize(35, 35));                   ///< 设置离开图标大小

    ui->classify_toolButton->setIcon(
        QIcon(QStringLiteral(":/RecommendForYou/Res/recommend/tabIcon/classification.svg")));
    ///< 设置分类按钮图标
    ui->classify_toolButton->setEnterIconSize(QSize(40, 40)); ///< 设置悬停图标大小
    ui->classify_toolButton->setLeaveIconSize(QSize(35, 35)); ///< 设置离开图标大小

    ui->scene_music_toolButton->setIcon(
        QIcon(QStringLiteral(":/RecommendForYou/Res/recommend/tabIcon/shafa.svg"))); ///< 设置场景音乐按钮图标
    ui->scene_music_toolButton->setEnterIconSize(QSize(45, 45));                     ///< 设置悬停图标大小
    ui->scene_music_toolButton->setLeaveIconSize(QSize(40, 40));                     ///< 设置离开图标大小
    ui->scene_music_toolButton->setEnterFontSize(13);                                ///< 设置悬停字体大小

    ui->music_quality_toolButton->setIcon(
        QIcon(QStringLiteral(":/RecommendForYou/Res/recommend/tabIcon/dish.svg"))); ///< 设置音乐品质按钮图标
    ui->music_quality_toolButton->setEnterIconSize(QSize(40, 40));                  ///< 设置悬停图标大小
    ui->music_quality_toolButton->setLeaveIconSize(QSize(35, 35));                  ///< 设置离开图标大小
}

/**
 * @brief 初始化推荐表格
 * @note 添加推荐表格到布局
 */
void RecommendForYou::initTabWidget()
{
    const auto layout = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取垂直布局
    if (!layout) {
        return; ///< 空指针保护
    }
    QTimer::singleShot(100,
                       this,
                       [this, layout] {
                           layout->insertWidget(layout->count(),
                                                new TableWidget(
                                                    QStringLiteral(" 今日专属推荐"),
                                                    TableWidget::KIND::BlockList,
                                                    this)); ///< 添加今日专属推荐
                       });
    QTimer::singleShot(200,
                       this,
                       [this, layout] {
                           layout->insertWidget(layout->count(),
                                                new TableWidget(
                                                    QStringLiteral("潮流音乐站 "),
                                                    TableWidget::KIND::ItemList,
                                                    this)); ///< 添加潮流音乐站
                       });
    QTimer::singleShot(300,
                       this,
                       [this, layout] {
                           layout->insertWidget(layout->count(),
                                                new TableWidget(
                                                    QStringLiteral("热门好歌精选 "),
                                                    TableWidget::KIND::ItemList,
                                                    this)); ///< 添加热门好歌精选
                       });
    QTimer::singleShot(400,
                       this,
                       [this, layout] {
                           layout->insertWidget(layout->count(),
                                                new TableWidget(
                                                    QStringLiteral("私人专属好歌 "),
                                                    TableWidget::KIND::ItemList,
                                                    this)); ///< 添加私人专属好歌
                       });
}