/**
 * @file PurchasedMusic.cpp
 * @brief 实现 PurchasedMusic 类，管理付费音乐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "PurchasedMusic.h"
#include "ui_PurchasedMusic.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QEasingCurve>
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化付费音乐界面
 * @param parent 父控件指针，默认为 nullptr
 */
PurchasedMusic::PurchasedMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PurchasedMusic)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/purchased.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 连接动画完成信号
    enableButton(true);                                  ///< 启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
PurchasedMusic::~PurchasedMusic()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 初始化索引标签、堆栈窗口和默认付费单曲界面
 */
void PurchasedMusic::initUi()
{
    initIndexLab();                                      ///< 初始化索引标签
    initStackedWidget();                                 ///< 初始化堆栈窗口
    ui->paid_single_pushButton->clicked();              ///< 默认点击付费单曲按钮
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画曲线
    ui->stackedWidget->setSpeed(400);                   ///< 设置动画速度
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片和事件过滤器
 */
void PurchasedMusic::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置付费单曲索引图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置已购专辑索引图片
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置已购视频索引图片
    ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置付费单曲标签样式
    ui->idx2_lab->hide();                                ///< 隐藏已购专辑索引
    ui->idx3_lab->hide();                                ///< 隐藏已购视频索引
    ui->guide_widget1->installEventFilter(this);         ///< 安装付费单曲事件过滤器
    ui->guide_widget2->installEventFilter(this);         ///< 安装已购专辑事件过滤器
    ui->guide_widget3->installEventFilter(this);         ///< 安装已购视频事件过滤器
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面并设置按钮互斥
 */
void PurchasedMusic::initStackedWidget()
{
    initPaidSingle();                                    ///< 初始化付费单曲界面
    initPurchasedAlbums();                               ///< 初始化已购专辑界面
    initPurchasedVideos();                               ///< 初始化已购视频界面
    this->m_buttonGroup->addButton(ui->paid_single_pushButton); ///< 添加付费单曲按钮
    this->m_buttonGroup->addButton(ui->purchased_albums_pushButton); ///< 添加已购专辑按钮
    this->m_buttonGroup->addButton(ui->purchased_video_pushButton); ///< 添加已购视频按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮互斥
}

/**
 * @brief 初始化付费单曲界面
 * @note 创建付费单曲界面并连接信号
 */
void PurchasedMusic::initPaidSingle()
{
    this->m_paidSingle = std::make_unique<PaidSingle>(ui->stackedWidget); ///< 创建付费单曲界面
    connect(this->m_paidSingle.get(), &PaidSingle::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_paidSingle.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_paidSingle.get()); ///< 设置当前界面
}

/**
 * @brief 初始化已购专辑界面
 * @note 创建已购专辑界面并连接信号
 */
void PurchasedMusic::initPurchasedAlbums()
{
    this->m_purchasedAlbums = std::make_unique<PurchasedAlbums>(ui->stackedWidget); ///< 创建已购专辑界面
    connect(this->m_purchasedAlbums.get(), &PurchasedAlbums::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_purchasedAlbums.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化已购视频界面
 * @note 创建已购视频界面并连接信号
 */
void PurchasedMusic::initPurchasedVideos()
{
    this->m_purchasedVideos = std::make_unique<PurchasedVideos>(ui->stackedWidget); ///< 创建已购视频界面
    connect(this->m_purchasedVideos.get(), &PurchasedVideos::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_purchasedVideos.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void PurchasedMusic::enableButton(const bool &flag) const
{
    ui->paid_single_pushButton->setEnabled(flag);        ///< 设置付费单曲按钮状态
    ui->purchased_albums_pushButton->setEnabled(flag);   ///< 设置已购专辑按钮状态
    ui->purchased_video_pushButton->setEnabled(flag);    ///< 设置已购视频按钮状态
}

/**
 * @brief 付费单曲按钮点击槽函数
 * @note 切换到付费单曲界面
 */
void PurchasedMusic::on_paid_single_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_paidSingle.get()) {
        return;                                          ///< 当前已是付费单曲界面
    }
    ui->paid_single_pushButton->setChecked(true);        ///< 设置付费单曲按钮选中
    STREAM_INFO() << "切换付费单曲界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_paidSingle.get())); ///< 滑动到付费单曲界面
    ui->idx1_lab->show();                               ///< 显示付费单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏已购专辑索引
    ui->idx3_lab->hide();                               ///< 隐藏已购视频索引
    ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置付费单曲标签样式
    ui->purchased_albums_number_label->setStyleSheet(""); ///< 重置已购专辑标签样式
    ui->purchased_video_number_label->setStyleSheet(""); ///< 重置已购视频标签样式
}

/**
 * @brief 已购专辑按钮点击槽函数
 * @note 切换到已购专辑界面
 */
void PurchasedMusic::on_purchased_albums_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_purchasedAlbums.get()) {
        return;                                          ///< 当前已是已购专辑界面
    }
    ui->purchased_albums_pushButton->setChecked(true);   ///< 设置已购专辑按钮选中
    STREAM_INFO() << "切换已购专辑界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_purchasedAlbums.get())); ///< 滑动到已购专辑界面
    ui->idx1_lab->hide();                               ///< 隐藏付费单曲索引
    ui->idx2_lab->show();                               ///< 显示已购专辑索引
    ui->idx3_lab->hide();                               ///< 隐藏已购视频索引
    ui->paid_single_number_label->setStyleSheet("");     ///< 重置付费单曲标签样式
    ui->purchased_albums_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置已购专辑标签样式
    ui->purchased_video_number_label->setStyleSheet(""); ///< 重置已购视频标签样式
}

/**
 * @brief 已购视频按钮点击槽函数
 * @note 切换到已购视频界面
 */
void PurchasedMusic::on_purchased_video_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_purchasedVideos.get()) {
        return;                                          ///< 当前已是已购视频界面
    }
    ui->purchased_video_pushButton->setChecked(true);    ///< 设置已购视频按钮选中
    STREAM_INFO() << "切换已购视频界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_purchasedVideos.get())); ///< 滑动到已购视频界面
    ui->idx1_lab->hide();                               ///< 隐藏付费单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏已购专辑索引
    ui->idx3_lab->show();                               ///< 显示已购视频索引
    ui->paid_single_number_label->setStyleSheet("");     ///< 重置付费单曲标签样式
    ui->purchased_albums_number_label->setStyleSheet(""); ///< 重置已购专辑标签样式
    ui->purchased_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置已购视频标签样式
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换按钮和标签样式
 */
bool PurchasedMusic::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->paid_single_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置付费单曲按钮悬停样式
            ui->paid_single_number_label->setStyleSheet(ui->paid_single_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置付费单曲标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->paid_single_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置付费单曲按钮离开样式
            ui->paid_single_number_label->setStyleSheet(ui->paid_single_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置付费单曲标签样式
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->purchased_albums_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置已购专辑按钮悬停样式
            ui->purchased_albums_number_label->setStyleSheet(ui->purchased_albums_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置已购专辑标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->purchased_albums_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置已购专辑按钮离开样式
            ui->purchased_albums_number_label->setStyleSheet(ui->purchased_albums_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置已购专辑标签样式
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->purchased_video_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置已购视频按钮悬停样式
            ui->purchased_video_number_label->setStyleSheet(ui->purchased_video_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置已购视频标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->purchased_video_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");                                         ///< 设置已购视频按钮离开样式
            ui->purchased_video_number_label->setStyleSheet(ui->purchased_video_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置已购视频标签样式
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 点击标签切换界面
 */
void PurchasedMusic::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->paid_single_number_label->geometry(); ///< 获取付费单曲标签区域
        const auto labelRect2 = ui->purchased_albums_number_label->geometry(); ///< 获取已购专辑标签区域
        const auto labelRect3 = ui->purchased_video_number_label->geometry(); ///< 获取已购视频标签区域
        const QPoint clickPos1 = ui->paid_single_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换付费单曲点击坐标
        const QPoint clickPos2 = ui->purchased_albums_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换已购专辑点击坐标
        const QPoint clickPos3 = ui->purchased_video_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换已购视频点击坐标
        if (labelRect1.contains(clickPos1)) {
            ui->paid_single_pushButton->clicked();          ///< 触发付费单曲按钮点击
        }
        if (labelRect2.contains(clickPos2)) {
            ui->purchased_albums_pushButton->clicked();     ///< 触发已购专辑按钮点击
        }
        if (labelRect3.contains(clickPos3)) {
            ui->purchased_video_pushButton->clicked();      ///< 触发已购视频按钮点击
        }
    }
    QWidget::mousePressEvent(event);                     ///< 调用父类事件
}