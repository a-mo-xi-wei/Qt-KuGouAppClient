/**
 * @file Video.cpp
 * @brief 实现 Video 类，提供视频界面管理功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "Video.h"
#include "ui_Video.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QButtonGroup>
#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
Video::Video(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Video)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css")); ///< 加载样式表
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
    }
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initUi();                                            ///< 初始化界面

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true);                              ///< 动画结束时启用按钮
    });
    enableButton(true);                                  ///< 初始启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
Video::~Video()
{
    delete ui;
}

/**
 * @brief 初始化界面
 */
void Video::initUi() const
{
    {
        auto video_channel_toolTip = new ElaToolTip(ui->video_channel_pushButton); ///< 创建视频频道工具提示
        video_channel_toolTip->setToolTip(QStringLiteral("视频频道"));

        auto mv_toolTip = new ElaToolTip(ui->MV_pushButton); ///< 创建 MV 工具提示
        mv_toolTip->setToolTip(QStringLiteral("MV"));

        auto video_toolTip = new ElaToolTip(ui->video_pushButton); ///< 创建视频工具提示
        video_toolTip->setToolTip(QStringLiteral("视频"));
    }

    ui->index_label1->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签1图片
    ui->index_label2->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签2图片
    ui->index_label3->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签3图片
    ui->index_label2->hide();                            ///< 隐藏索引标签2
    ui->index_label3->hide();                            ///< 隐藏索引标签3

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置堆栈窗口动画曲线
    ui->stackedWidget->setSpeed(400);                    ///< 设置动画速度（400ms）
}

/**
 * @brief 初始化堆栈窗口
 */
void Video::initStackedWidget()
{
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);   ///< 设置堆栈窗口边距
    initVideoChannelWidget();                            ///< 初始化视频频道控件
    initMVWidget();                                      ///< 初始化 MV 控件
    initVideoWidget();                                   ///< 初始化视频控件
    m_buttonGroup->addButton(ui->video_channel_pushButton); ///< 添加视频频道按钮
    m_buttonGroup->addButton(ui->MV_pushButton);         ///< 添加 MV 按钮
    m_buttonGroup->addButton(ui->video_pushButton);      ///< 添加视频按钮
    m_buttonGroup->setExclusive(true);                   ///< 设置按钮组互斥
}

/**
 * @brief 初始化视频频道控件
 */
void Video::initVideoChannelWidget()
{
    this->m_videoChannelWidget = std::make_unique<VideoChannelWidget>(ui->stackedWidget); ///< 创建视频频道控件
    ui->stackedWidget->addWidget(this->m_videoChannelWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化 MV 控件
 */
void Video::initMVWidget()
{
    this->m_MVWidget = std::make_unique<MVWidget>(ui->stackedWidget); ///< 创建 MV 控件
    ui->stackedWidget->addWidget(this->m_MVWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化视频控件
 */
void Video::initVideoWidget()
{
    this->m_videoWidget = std::make_unique<VideoWidget>(ui->stackedWidget); ///< 创建视频控件
    ui->stackedWidget->addWidget(this->m_videoWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void Video::enableButton(const bool &flag) const
{
    ui->video_channel_pushButton->setEnabled(flag);      ///< 设置视频频道按钮状态
    ui->MV_pushButton->setEnabled(flag);                ///< 设置 MV 按钮状态
    ui->video_pushButton->setEnabled(flag);             ///< 设置视频按钮状态
}

/**
 * @brief 处理视频频道按钮点击
 */
void Video::on_video_channel_pushButton_clicked()
{
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_videoChannelWidget.get())); ///< 切换到视频频道
    STREAM_INFO() << "切换 videoChannelWidget 界面";      ///< 记录切换日志
    ui->index_label1->show();                            ///< 显示索引标签1
    ui->index_label2->hide();                            ///< 隐藏索引标签2
    ui->index_label3->hide();                            ///< 隐藏索引标签3
}

/**
 * @brief 处理 MV 按钮点击
 */
void Video::on_MV_pushButton_clicked()
{
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_MVWidget.get())); ///< 切换到 MV
    STREAM_INFO() << "切换 MVWidget 界面";               ///< 记录切换日志
    ui->index_label1->hide();                            ///< 隐藏索引标签1
    ui->index_label2->show();                            ///< 显示索引标签2
    ui->index_label3->hide();                            ///< 隐藏索引标签3
}

/**
 * @brief 处理视频按钮点击
 */
void Video::on_video_pushButton_clicked()
{
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_videoWidget.get())); ///< 切换到视频
    STREAM_INFO() << "切换 videoWidget 界面";             ///< 记录切换日志
    ui->index_label1->hide();                            ///< 隐藏索引标签1
    ui->index_label2->hide();                            ///< 隐藏索引标签2
    ui->index_label3->show();                            ///< 显示索引标签3
}