/**
 * @file ListenBook.cpp
 * @brief 实现 ListenBook 类，提供听书主界面功能
 * @author WeiWang
 * @date 2024-11-18
 * @version 1.0
 */

#include "ListenBook.h"
#include "ui_ListenBook.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化听书主界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenBook::ListenBook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenBook)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/listen.css")); ///< 加载样式表
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
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 动画结束启用按钮
    enableButton(true);                                  ///< 初始启用按钮
    ui->stackedWidget->setVerticalMode(true);            ///< 设置垂直动画模式
}

/**
 * @brief 析构函数，清理资源
 */
ListenBook::~ListenBook()
{
    delete ui;
}

/**
 * @brief 初始化界面
 * @note 设置按钮图标、文本和指示器样式
 */
void ListenBook::initUi()
{
    initStackedWidget();                                 ///< 初始化堆栈窗口
    // 设置图标
    ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg"))); ///< 推荐按钮图标
    ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg"))); ///< 下载按钮图标
    ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg"))); ///< 最近播放按钮图标
    ui->listen_recommend_toolButton->setIconSize(QSize(17, 17)); ///< 推荐按钮图标大小
    ui->listen_my_download_toolButton->setIconSize(QSize(21, 21)); ///< 下载按钮图标大小
    ui->recently_play_toolButton->setIconSize(QSize(19, 19)); ///< 最近播放按钮图标大小
    // 设置文本
    ui->listen_recommend_toolButton->setText(QStringLiteral("   推荐")); ///< 推荐按钮文本
    ui->listen_my_download_toolButton->setText(QStringLiteral("  我的下载")); ///< 下载按钮文本
    ui->recently_play_toolButton->setText(QStringLiteral("   最近播放")); ///< 最近播放按钮文本
    // 设置选中图标
    {
        connect(ui->listen_recommend_toolButton, &QToolButton::toggled, ui->listen_recommend_toolButton, [=](bool checked) {
            if (checked) {
                ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-black.svg")));
            } else {
                ui->listen_recommend_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recommend-gray.svg")));
            }
        });
        connect(ui->listen_my_download_toolButton, &QToolButton::toggled, ui->listen_my_download_toolButton, [=](bool checked) {
            if (checked) {
                ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-black.svg")));
            } else {
                ui->listen_my_download_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/download-gray.svg")));
            }
        });
        connect(ui->recently_play_toolButton, &QToolButton::toggled, ui->recently_play_toolButton, [=](bool checked) {
            if (checked) {
                ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-black.svg")));
            } else {
                ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/recent-gray.svg")));
            }
        });
    }

    //显示隐藏按钮
    {
        ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                        background-color:rgba(255,255,255,0);
                                                        border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                    }
                                                    QToolButton:hover{
                                                        border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                    })");

        connect(ui->indicator_toolButton, &QToolButton::toggled, ui->indicator_toolButton, [=](bool checked) {
                if (checked) {
                    ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                                background-color:rgba(255,255,255,0);
                                                                border-image:url(:/ListenBook/Res/listenbook/down-black.svg);
                                                            }
                                                            QToolButton:hover{
                                                                border-image:url(:/ListenBook/Res/listenbook/down-blue.svg);
                                                            })");
                } else {
                    ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
                                                                background-color:rgba(255,255,255,0);
                                                                border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
                                                            }
                                                            QToolButton:hover{
                                                                border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
                                                            })");
                }
        });
    }

}

/**
 * @brief 初始化堆栈窗口
 * @note 创建页面控件并设置按钮互斥
 */
void ListenBook::initStackedWidget()
{
    initListenRecommend();                               ///< 初始化推荐页面
    initListenMyDownload();                              ///< 初始化下载页面
    initListenRecentlyPlay();                            ///< 初始化最近播放页面
    this->m_buttonGroup->addButton(ui->listen_recommend_toolButton); ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->listen_my_download_toolButton); ///< 添加下载按钮
    this->m_buttonGroup->addButton(ui->recently_play_toolButton); ///< 添加最近播放按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮互斥
}

/**
 * @brief 初始化推荐页面
 * @note 创建推荐页面控件并添加到堆栈窗口
 */
void ListenBook::initListenRecommend()
{
    this->m_listenRecommend = std::make_unique<ListenRecommend>(ui->stackedWidget); ///< 创建推荐页面
    ui->stackedWidget->addWidget(this->m_listenRecommend.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_listenRecommend.get()); ///< 设置为当前页面
}

/**
 * @brief 初始化下载页面
 * @note 创建下载页面控件并连接切换信号
 */
void ListenBook::initListenMyDownload()
{
    this->m_listenMyDownload = std::make_unique<ListenMyDownload>(ui->stackedWidget); ///< 创建下载页面
    ui->stackedWidget->addWidget(this->m_listenMyDownload.get()); ///< 添加到堆栈窗口
    connect(this->m_listenMyDownload.get(), &ListenMyDownload::switch_to_listen_recommend, this, [this] {
        ui->listen_recommend_toolButton->clicked();      ///< 触发推荐按钮点击
        ui->listen_recommend_toolButton->setChecked(true); ///< 设置推荐按钮选中
    });
}

/**
 * @brief 初始化最近播放页面
 * @note 创建最近播放页面控件并连接切换信号
 */
void ListenBook::initListenRecentlyPlay()
{
    this->m_listenRecentlyPlay = std::make_unique<ListenRecentlyPlay>(ui->stackedWidget); ///< 创建最近播放页面
    ui->stackedWidget->addWidget(this->m_listenRecentlyPlay.get()); ///< 添加到堆栈窗口
    connect(this->m_listenRecentlyPlay.get(), &ListenRecentlyPlay::switch_to_listen_recommend, this, [this] {
        ui->listen_recommend_toolButton->clicked();      ///< 触发推荐按钮点击
        ui->listen_recommend_toolButton->setChecked(true); ///< 设置推荐按钮选中
    });
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void ListenBook::enableButton(const bool &flag) const
{
    ui->listen_recommend_toolButton->setEnabled(flag);   ///< 设置推荐按钮状态
    ui->listen_my_download_toolButton->setEnabled(flag); ///< 设置下载按钮状态
    ui->recently_play_toolButton->setEnabled(flag);      ///< 设置最近播放按钮状态
}

/**
 * @brief 推荐按钮点击槽函数
 * @note 切换到推荐页面并触发动画
 */
void ListenBook::on_listen_recommend_toolButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_listenRecommend.get())
        return;
    STREAM_INFO() << "切换推荐界面";                    ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_listenRecommend.get())); ///< 滑动到推荐页面
}

/**
 * @brief 下载按钮点击槽函数
 * @note 切换到下载页面并触发动画
 */
void ListenBook::on_listen_my_download_toolButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_listenMyDownload.get())
        return;
    STREAM_INFO() << "切换下载界面";                    ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_listenMyDownload.get())); ///< 滑动到下载页面
}

/**
 * @brief 最近播放按钮点击槽函数
 * @note 切换到最近播放页面并触发动画
 */
void ListenBook::on_recently_play_toolButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_listenRecentlyPlay.get())
        return;
    STREAM_INFO() << "切换最近界面";                    ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_listenRecentlyPlay.get())); ///< 滑动到最近播放页面
}