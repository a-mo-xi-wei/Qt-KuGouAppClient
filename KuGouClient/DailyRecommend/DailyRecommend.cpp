/**
 * @file DailyRecommend.cpp
 * @brief 实现 DailyRecommend 类，提供每日推荐界面功能
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "DailyRecommend.h"
#include "ui_DailyRecommend.h"
#include "IconBesideTextToolButton.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"
#include "RefreshMask.h"

#include <QFile>
#include <QDateTime>
#include <QQueue>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化每日推荐界面
 * @param parent 父控件指针，默认为 nullptr
 */
DailyRecommend::DailyRecommend(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::DailyRecommend)
      , m_refreshMask(std::make_unique<RefreshMask>(this)) ///< 初始化刷新遮罩
{
    ui->setupUi(this); ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/daily.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll()); ///< 应用样式表
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
            return;
        }
    }
    QTimer::singleShot(0, this, [this] { initUi(); });
}

/**
 * @brief 析构函数，清理资源
 */
DailyRecommend::~DailyRecommend()
{
    delete ui;
}

/**
 * @brief 初始化界面
 * @note 初始化日期标签、按钮和歌曲列表
 */
void DailyRecommend::initUi()
{
    ui->under_label->setFont(QFont("TaiwanPearl"));
    ui->history_recommend_toolButton->setFont(QFont("TaiwanPearl"));
    ui->music_label->setFont(QFont("TaiwanPearl", 13));
    ui->desc_widget->setStyleSheet("font-family: 'TaiwanPearl';font-size: 13px;");
    this->m_refreshMask->keepLoading();
    ui->history_recommend_toolButton->setIconSize(QSize(10, 10)); ///< 设置历史推荐按钮图标大小
    ui->history_recommend_toolButton->setIcon(
        QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    ///< 设置默认图标
    ui->history_recommend_toolButton->setEnterIcon(
        QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
    ///< 设置悬停图标
    ui->history_recommend_toolButton->setLeaveIcon(
        QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
    ///< 设置离开图标
    ui->history_recommend_toolButton->setApproach(true); ///< 启用接近效果
    ui->history_recommend_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
    ///< 设置悬停字体颜色

    ui->play_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg")));
    ///< 设置播放按钮图标
    ui->play_toolButton->setText(QStringLiteral("播放")); ///< 设置播放按钮文本

    auto vip_toolButton_toolTip = new ElaToolTip(ui->vip_toolButton);           ///< 创建 VIP 按钮工具提示
    vip_toolButton_toolTip->setToolTip(QStringLiteral("威哥出品，不存在VIP"));          ///< 设置 VIP 工具提示内容
    auto collect_toolButton_toolTip = new ElaToolTip(ui->collect_toolButton);   ///< 创建收藏按钮工具提示
    collect_toolButton_toolTip->setToolTip(QStringLiteral("收藏"));               ///< 设置收藏工具提示内容
    auto download_toolButton_toolTip = new ElaToolTip(ui->download_toolButton); ///< 创建下载按钮工具提示
    download_toolButton_toolTip->setToolTip(QStringLiteral("下载"));              ///< 设置下载工具提示内容
    auto batch_toolButton_toolTip = new ElaToolTip(ui->batch_toolButton);       ///< 创建批量操作按钮工具提示
    batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));               ///< 设置批量操作工具提示内容

    ui->vip_toolButton->setIconSize(QSize(18, 18)); ///< 设置 VIP 按钮图标大小
    ui->vip_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/yellow-diamond.svg")));
    ///< 设置 VIP 按钮图标
    ui->vip_toolButton->setText(QStringLiteral("+30")); ///< 设置 VIP 按钮文本
    ui->vip_toolButton->setApproach(true);              ///< 启用接近效果

    ui->collect_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/like-gray.svg")));
    ///< 设置收藏按钮图标
    ui->download_toolButton->setIcon(
        QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/download-gray.svg"))); ///< 设置下载按钮图标
    ui->batch_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/batch-gray.svg")));
    ///< 设置批量操作按钮图标
    ui->count_label->setText(QStringLiteral("30")); ///< 设置歌曲数量标签
    ui->ico_label->setPixmap(
        QPixmap(QStringLiteral(":/TabIcon/Res/tabIcon/yellow-diamond.svg")).scaled(18, 18));
    ///< 设置图标标签

    QTimer::singleShot(0, this, [this] { initDateLab(); });      ///< 初始化日期标签
    QTimer::singleShot(10, this, [this] { initTableWidget(); }); ///< 初始化歌曲列表控件
}

/**
 * @brief 初始化日期标签
 * @note 设置当前日期并调整标签位置
 */
void DailyRecommend::initDateLab()
{
    auto monthLab = new QLabel(this);
    auto dayLab = new QLabel(this);
    monthLab->setObjectName("monthLab");                                            ///< 设置月份标签对象名称
    dayLab->setObjectName("dayLab");                                                ///< 设置日期标签对象名称
    const QDate currentDate = QDate::currentDate();                                 ///< 获取当前日期
    const QString monthStr = QString::number(currentDate.month()) + "月";            ///< 设置月份文本
    monthLab->setText(monthStr);                                                    ///< 应用月份文本
    const QString dayStr = QString("%1").arg(currentDate.day(), 2, 10, QChar('0')); ///< 设置日期文本（补零）
    dayLab->setFixedHeight(40);                                                     ///< 设置日期标签固定高度
    dayLab->setText(dayStr);                                                        ///< 应用日期文本
    monthLab->setScaledContents(true);                                              ///< 启用月份标签内容缩放
    dayLab->setScaledContents(true);                                                ///< 启用日期标签内容缩放
    const QPoint targetPos = ui->top_cover_label->pos();                            ///< 获取封面标签位置
    monthLab->move(targetPos.x() + 30, targetPos.y() + 30);                         ///< 移动月份标签
    dayLab->move(targetPos.x() + 30, targetPos.y() + 35 + monthLab->height());      ///< 移动日期标签
    monthLab->raise();                                                              ///< 提升月份标签层级
    dayLab->raise();                                                                ///< 提升日期标签层级
}

/**
 * @brief 初始化歌曲列表控件
 * @note 添加 30 首歌曲信息到垂直布局
 */
void DailyRecommend::initTableWidget()
{
    auto layout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!layout) {
        qWarning() << "布局不存在";
        return;
    }

    const int totalItems = 30;

    using Task = std::function<void()>;
    auto queue = std::make_shared<QQueue<Task>>();

    // 准备任务
    for (int i = 0; i < totalItems; ++i) {
        queue->enqueue([this, layout, i]() {
            SongInfor tempInformation;
            tempInformation.index = i;
            tempInformation.cover = QPixmap(QString(":/Res/tablisticon/pix%1.png").arg(i % 10 + 1));
            tempInformation.songName = "网络歌曲";
            tempInformation.singer = "网络歌手";
            tempInformation.duration = "未知时长";
            tempInformation.mediaPath = "未知路径";
            tempInformation.addTime = QDateTime::currentDateTime();
            tempInformation.playCount = 0;

            auto item = new MusicItemWidget(tempInformation, this);
            initMusicItem(item);
            layout->addWidget(item);

            // 最后一个执行完毕
            if (i == totalItems - 1) {
                m_refreshMask->hideLoading("");
                QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);
            }
        });
    }

    // 任务执行器
    auto runner = std::make_shared<std::function<void()>>();
    *runner = [queue, runner]() {
        if (queue->isEmpty())
            return;

        auto task = queue->dequeue();
        QTimer::singleShot(10,
                           nullptr,
                           [task, runner]() {
                               task();
                               (*runner)();
                           });
    };

    // 启动任务链
    (*runner)();
}

/**
 * @brief 初始化音乐项控件
 * @param item 音乐项控件指针
 */
void DailyRecommend::initMusicItem(MusicItemWidget *item)
{
    item->setFillColor(QColor(QStringLiteral("#B0EDF6"))); ///< 设置填充颜色
    item->setRadius(12);                                   ///< 设置圆角半径
    item->setInterval(1);                                  ///< 设置间距
}

/**
 * @brief 历史推荐按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_history_recommend_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 功能未实现 敬请期待").arg(
                                   ui->history_recommend_toolButton->text().left(
                                       ui->history_recommend_toolButton->text().size() - 2)),
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 播放按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_play_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               "本界面播放 功能未实现 敬请期待",
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief VIP 按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_vip_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               "VIP 功能未实现 敬请期待",
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 收藏按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_collect_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("收藏 功能未实现 敬请期待").arg(
                                   ui->history_recommend_toolButton->text().left(
                                       ui->history_recommend_toolButton->text().size() - 2)),
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 下载按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_download_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               "下载 功能未实现 敬请期待",
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示功能未实现提示
 */
void DailyRecommend::on_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               "批量操作 功能未实现 敬请期待",
                               1000,
                               this->window()); ///< 显示提示
}

void DailyRecommend::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    auto r = rect();
    r.setRight(rect().right() - 4);
    m_refreshMask->setGeometry(r);
    m_refreshMask->raise(); // 确保遮罩在最上层
}

void DailyRecommend::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    auto r = rect();
    r.setRight(rect().right() - 4);
    m_refreshMask->setGeometry(r);
    m_refreshMask->raise(); // 确保遮罩在最上层
}
