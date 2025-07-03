/**
 * @file ListenMyDownload.cpp
 * @brief 实现 ListenMyDownload 类，提供下载管理界面功能
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#include "ListenMyDownload.h"
#include "ui_ListenMyDownload.h"
#include "logger.hpp"

#include <QFile>
#include <QButtonGroup>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化下载管理界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenMyDownload::ListenMyDownload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenMyDownload)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/download.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
    }
    initUi();                                            ///< 初始化界面
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 连接动画完成信号
    enableButton(true);                                  ///< 启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
ListenMyDownload::~ListenMyDownload()
{
    delete ui;
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化已下载和下载中控件并设置按钮互斥
 */
void ListenMyDownload::initStackedWidget()
{
    initDownloadedWidget();                              ///< 初始化已下载控件
    initDownloadingWidget();                             ///< 初始化下载中控件
    this->m_buttonGroup->addButton(ui->downloaded_pushButton); ///< 添加已下载按钮
    this->m_buttonGroup->addButton(ui->downloading_pushButton); ///< 添加下载中按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮组互斥
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口和下标标签
 */
void ListenMyDownload::initUi()
{
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                      ///< 初始化下标标签
    ui->downloaded_pushButton->clicked();                ///< 触发已下载按钮
}

/**
 * @brief 初始化下标标签
 * @note 设置下标图片、样式和事件过滤器
 */
void ListenMyDownload::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置下标 1 图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置下标 2 图片
    ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置已下载数量标签样式
    ui->idx2_lab->hide();                                ///< 隐藏下标 2
    ui->guide_widget1->installEventFilter(this);         ///< 安装事件过滤器 1
    ui->guide_widget2->installEventFilter(this);         ///< 安装事件过滤器 2
}

/**
 * @brief 初始化已下载控件
 */
void ListenMyDownload::initDownloadedWidget()
{
    this->m_downloaded = std::make_unique<DownloadedWidget>(ui->stackedWidget); ///< 创建已下载控件
    ui->stackedWidget->addWidget(this->m_downloaded.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_downloaded.get()); ///< 设置当前控件
    connect(this->m_downloaded.get(), &DownloadedWidget::find_more_audio_book, [this] { emit switch_to_listen_recommend(); }); ///< 连接查找信号
}

/**
 * @brief 初始化下载中控件
 */
void ListenMyDownload::initDownloadingWidget()
{
    this->m_downloading = std::make_unique<DownloadingWidget>(ui->stackedWidget); ///< 创建下载中控件
    ui->stackedWidget->addWidget(this->m_downloading.get()); ///< 添加到堆栈窗口
    connect(this->m_downloading.get(), &DownloadingWidget::find_more_audio_book, [this] { emit switch_to_listen_recommend(); }); ///< 连接查找信号
}

/**
 * @brief 启用或禁用按钮
 * @param flag 启用标志
 */
void ListenMyDownload::enableButton(const bool &flag) const
{
    ui->downloaded_pushButton->setEnabled(flag);         ///< 设置已下载按钮状态
    ui->downloading_pushButton->setEnabled(flag);        ///< 设置下载中按钮状态
}

/**
 * @brief 事件过滤器，处理鼠标悬停事件
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool ListenMyDownload::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->guide_widget1)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->downloaded_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置悬停样式
            if (ui->downloaded_pushButton->isChecked())
                ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置选中样式
            else
                ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;")); ///< 设置未选中样式
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->downloaded_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置离开样式
            if (ui->downloaded_pushButton->isChecked())
                ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置选中样式
            else
                ui->downloaded_number_label->setStyleSheet(""); ///< 清除样式
        }
    }
    if (watched == ui->guide_widget2)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->downloading_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置悬停样式
            if (ui->downloading_pushButton->isChecked())
                ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置选中样式
            else
                ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;")); ///< 设置未选中样式
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->downloading_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置离开样式
            if (ui->downloading_pushButton->isChecked())
                ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置选中样式
            else
                ui->downloading_number_label->setStyleSheet(""); ///< 清除样式
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类事件过滤器
}

/**
 * @brief 鼠标按下事件，处理标签点击
 * @param event 鼠标事件
 */
void ListenMyDownload::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);                     ///< 调用父类鼠标按下事件
    if (event->button() == Qt::LeftButton)
    {
        const auto labelRect1 = ui->downloaded_number_label->geometry(); ///< 获取已下载标签区域
        const auto labelRect2 = ui->downloading_number_label->geometry(); ///< 获取下载中标签区域
        const QPoint clickPos1 = ui->downloaded_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换点击坐标
        const QPoint clickPos2 = ui->downloading_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换点击坐标
        if (labelRect1.contains(clickPos1))
        {
            ui->downloaded_pushButton->clicked();            ///< 触发已下载按钮
        }
        if (labelRect2.contains(clickPos2))
        {
            ui->downloading_pushButton->clicked();           ///< 触发下载中按钮
        }
    }
}

/**
 * @brief 已下载按钮点击槽函数
 * @note 切换到已下载页面并更新样式
 */
void ListenMyDownload::on_downloaded_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_downloaded.get())
        return;                                          ///< 当前已是目标页面则返回
    enableButton(false);                                 ///< 禁用按钮
    ui->downloaded_pushButton->setChecked(true);         ///< 选中已下载按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_downloaded.get())); ///< 切换到已下载页面
    ui->idx1_lab->show();                                ///< 显示下标 1
    ui->idx2_lab->hide();                                ///< 隐藏下标 2
    ui->downloaded_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置已下载标签样式
    ui->downloading_number_label->setStyleSheet("");      ///< 清除下载中标签样式
}

/**
 * @brief 下载中按钮点击槽函数
 * @note 切换到下载中页面并更新样式
 */
void ListenMyDownload::on_downloading_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_downloading.get())
        return;                                          ///< 当前已是目标页面则返回
    enableButton(false);                                 ///< 禁用按钮
    ui->downloading_pushButton->setChecked(true);        ///< 选中下载中按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_downloading.get())); ///< 切换到下载中页面
    ui->idx1_lab->hide();                                ///< 隐藏下标 1
    ui->idx2_lab->show();                                ///< 显示下标 2
    ui->downloaded_number_label->setStyleSheet("");       ///< 清除已下载标签样式
    ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置下载中标签样式
}