/**
 * @file RecentlyPlayed.cpp
 * @brief 实现 RecentlyPlayed 类，管理最近播放界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "RecentlyPlayed.h"
#include "ui_RecentlyPlayed.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近播放界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlyPlayed::RecentlyPlayed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlyPlayed)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/recently.css")); ///< 加载样式表
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
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 动画结束恢复按钮交互
    enableButton(true);                                  ///< 启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
RecentlyPlayed::~RecentlyPlayed()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口、索引标签和默认单曲界面
 */
void RecentlyPlayed::initUi()
{
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                      ///< 初始化索引标签
    ui->single_song_pushButton->clicked();               ///< 默认触发单曲按钮
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画缓动曲线
    ui->stackedWidget->setSpeed(400);                    ///< 设置动画速度
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片、隐藏非当前索引和数字标签样式
 */
void RecentlyPlayed::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置单曲索引图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置歌单索引图片
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置视频索引图片
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置歌曲频道索引图片
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置 MV 频道索引图片
    ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置单曲数字标签高亮
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏视频索引
    ui->idx4_lab->hide();                                ///< 隐藏歌曲频道索引
    ui->idx5_lab->hide();                                ///< 隐藏 MV 频道索引
    ui->guide_widget1->installEventFilter(this);          ///< 安装单曲引导控件事件过滤器
    ui->guide_widget2->installEventFilter(this);          ///< 安装歌单引导控件事件过滤器
    ui->guide_widget3->installEventFilter(this);          ///< 安装视频引导控件事件过滤器
    ui->guide_widget4->installEventFilter(this);          ///< 安装歌曲频道引导控件事件过滤器
    ui->guide_widget5->installEventFilter(this);          ///< 安装 MV 频道引导控件事件过滤器
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面和按钮组
 */
void RecentlyPlayed::initStackedWidget()
{
    initSingleSong();                                    ///< 初始化单曲界面
    initSongList();                                      ///< 初始化歌单界面
    initVideoWidget();                                   ///< 初始化视频界面
    initSongChannel();                                   ///< 初始化歌曲频道界面
    initMVChannel();                                     ///< 初始化 MV 频道界面
    this->m_buttonGroup->addButton(ui->single_song_pushButton); ///< 添加单曲按钮
    this->m_buttonGroup->addButton(ui->song_list_pushButton); ///< 添加歌单按钮
    this->m_buttonGroup->addButton(ui->video_pushButton); ///< 添加视频按钮
    this->m_buttonGroup->addButton(ui->song_channel_pushButton); ///< 添加歌曲频道按钮
    this->m_buttonGroup->addButton(ui->MV_channel_pushButton); ///< 添加 MV 频道按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮组互斥
}

/**
 * @brief 初始化单曲界面
 * @note 创建单曲界面并添加到堆栈窗口
 */
void RecentlyPlayed::initSingleSong()
{
    this->m_singleSong = std::make_unique<RecentlySingleSong>(ui->stackedWidget); ///< 创建单曲界面
    connect(this->m_singleSong.get(), &RecentlySingleSong::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索音乐信号
    ui->stackedWidget->addWidget(this->m_singleSong.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get()); ///< 设置为当前界面
}

/**
 * @brief 初始化歌单界面
 * @note 创建歌单界面并添加到堆栈窗口
 */
void RecentlyPlayed::initSongList()
{
    this->m_songList = std::make_unique<RecentlySongList>(ui->stackedWidget); ///< 创建歌单界面
    connect(this->m_songList.get(), &RecentlySongList::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索音乐信号
    ui->stackedWidget->addWidget(this->m_songList.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化视频界面
 * @note 创建视频界面并添加到堆栈窗口
 */
void RecentlyPlayed::initVideoWidget()
{
    this->m_videoWidget = std::make_unique<RecentlyVideoWidget>(ui->stackedWidget); ///< 创建视频界面
    connect(this->m_videoWidget.get(), &RecentlyVideoWidget::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索音乐信号
    ui->stackedWidget->addWidget(this->m_videoWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化歌曲频道界面
 * @note 创建歌曲频道界面并添加到堆栈窗口
 */
void RecentlyPlayed::initSongChannel()
{
    this->m_songChannel = std::make_unique<RecentlySongChannel>(ui->stackedWidget); ///< 创建歌曲频道界面
    connect(this->m_songChannel.get(), &RecentlySongChannel::find_more_channel, [this] { emit find_more_channel(); }); ///< 连接搜索频道信号
    ui->stackedWidget->addWidget(this->m_songChannel.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化 MV 频道界面
 * @note 创建 MV 频道界面并添加到堆栈窗口
 */
void RecentlyPlayed::initMVChannel()
{
    this->m_mvChannel = std::make_unique<RecentlyMVChannel>(ui->stackedWidget); ///< 创建 MV 频道界面
    connect(this->m_mvChannel.get(), &RecentlyMVChannel::find_more_channel, [this] { emit find_more_channel(); }); ///< 连接搜索频道信号
    ui->stackedWidget->addWidget(this->m_mvChannel.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void RecentlyPlayed::enableButton(const bool &flag) const
{
    ui->single_song_pushButton->setEnabled(flag);        ///< 启用/禁用单曲按钮
    ui->song_list_pushButton->setEnabled(flag);          ///< 启用/禁用歌单按钮
    ui->video_pushButton->setEnabled(flag);              ///< 启用/禁用视频按钮
    ui->song_channel_pushButton->setEnabled(flag);       ///< 启用/禁用歌曲频道按钮
    ui->MV_channel_pushButton->setEnabled(flag);         ///< 启用/禁用 MV 频道按钮
}

/**
 * @brief 单曲按钮点击槽函数
 * @note 切换到单曲界面，显示复选框控件
 */
void RecentlyPlayed::on_single_song_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_singleSong.get()) return;
    ui->single_song_pushButton->setChecked(true);        ///< 选中单曲按钮
    STREAM_INFO() << "切换单曲界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_singleSong.get())); ///< 滑动切换到单曲界面
    ui->idx1_lab->show();                                ///< 显示单曲索引
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏视频索引
    ui->idx4_lab->hide();                                ///< 隐藏歌曲频道索引
    ui->idx5_lab->hide();                                ///< 隐藏 MV 频道索引
    ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 高亮单曲数字标签
    ui->song_list_number_label->setStyleSheet("");        ///< 重置歌单数字标签
    ui->video_number_label->setStyleSheet("");            ///< 重置视频数字标签
    ui->song_channel_number_label->setStyleSheet("");     ///< 重置歌曲频道数字标签
    ui->MV_channel_number_label->setStyleSheet("");       ///< 重置 MV 频道数字标签
    ui->check_box_widget->show();                        ///< 显示复选框控件
}

/**
 * @brief 歌单按钮点击槽函数
 * @note 切换到歌单界面，隐藏复选框控件
 */
void RecentlyPlayed::on_song_list_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_songList.get()) return;
    ui->song_list_pushButton->setChecked(true);          ///< 选中歌单按钮
    STREAM_INFO() << "切换歌单界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_songList.get())); ///< 滑动切换到歌单界面
    ui->idx1_lab->hide();                                ///< 隐藏单曲索引
    ui->idx2_lab->show();                                ///< 显示歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏视频索引
    ui->idx4_lab->hide();                                ///< 隐藏歌曲频道索引
    ui->idx5_lab->hide();                                ///< 隐藏 MV 频道索引
    ui->single_song_number_label->setStyleSheet("");      ///< 重置单曲数字标签
    ui->song_list_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 高亮歌单数字标签
    ui->video_number_label->setStyleSheet("");            ///< 重置视频数字标签
    ui->song_channel_number_label->setStyleSheet("");     ///< 重置歌曲频道数字标签
    ui->MV_channel_number_label->setStyleSheet("");       ///< 重置 MV 频道数字标签
    ui->check_box_widget->hide();                        ///< 隐藏复选框控件
}

/**
 * @brief 视频按钮点击槽函数
 * @note 切换到视频界面，隐藏复选框控件
 */
void RecentlyPlayed::on_video_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_videoWidget.get()) return;
    ui->video_pushButton->setChecked(true);              ///< 选中视频按钮
    STREAM_INFO() << "切换视频界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_videoWidget.get())); ///< 滑动切换到视频界面
    ui->idx1_lab->hide();                                ///< 隐藏单曲索引
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->show();                                ///< 显示视频索引
    ui->idx4_lab->hide();                                ///< 隐藏歌曲频道索引
    ui->idx5_lab->hide();                                ///< 隐藏 MV 频道索引
    ui->single_song_number_label->setStyleSheet("");      ///< 重置单曲数字标签
    ui->song_list_number_label->setStyleSheet("");        ///< 重置歌单数字标签
    ui->video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 高亮视频数字标签
    ui->song_channel_number_label->setStyleSheet("");     ///< 重置歌曲频道数字标签
    ui->MV_channel_number_label->setStyleSheet("");       ///< 重置 MV 频道数字标签
    ui->check_box_widget->hide();                        ///< 隐藏复选框控件
}

/**
 * @brief 歌曲频道按钮点击槽函数
 * @note 切换到歌曲频道界面，隐藏复选框控件
 */
void RecentlyPlayed::on_song_channel_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_songChannel.get()) return;
    ui->song_channel_pushButton->setChecked(true);       ///< 选中歌曲频道按钮
    STREAM_INFO() << "切换音乐频道界面";                 ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_songChannel.get())); ///< 滑动切换到歌曲频道界面
    ui->idx1_lab->hide();                                ///< 隐藏单曲索引
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏视频索引
    ui->idx4_lab->show();                                ///< 显示歌曲频道索引
    ui->idx5_lab->hide();                                ///< 隐藏 MV 频道索引
    ui->single_song_number_label->setStyleSheet("");      ///< 重置单曲数字标签
    ui->song_list_number_label->setStyleSheet("");        ///< 重置歌单数字标签
    ui->video_number_label->setStyleSheet("");            ///< 重置视频数字标签
    ui->song_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 高亮歌曲频道数字标签
    ui->MV_channel_number_label->setStyleSheet("");       ///< 重置 MV 频道数字标签
    ui->check_box_widget->hide();                        ///< 隐藏复选框控件
}

/**
 * @brief MV 频道按钮点击槽函数
 * @note 切换到 MV 频道界面，隐藏复选框控件
 */
void RecentlyPlayed::on_MV_channel_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_mvChannel.get()) return;
    ui->MV_channel_pushButton->setChecked(true);         ///< 选中 MV 频道按钮
    STREAM_INFO() << "切换MV频道界面";                   ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_mvChannel.get())); ///< 滑动切换到 MV 频道界面
    ui->idx1_lab->hide();                                ///< 隐藏单曲索引
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏视频索引
    ui->idx4_lab->hide();                                ///< 隐藏歌曲频道索引
    ui->idx5_lab->show();                                ///< 显示 MV 频道索引
    ui->single_song_number_label->setStyleSheet("");      ///< 重置单曲数字标签
    ui->song_list_number_label->setStyleSheet("");        ///< 重置歌单数字标签
    ui->video_number_label->setStyleSheet("");            ///< 重置视频数字标签
    ui->song_channel_number_label->setStyleSheet("");     ///< 重置歌曲频道数字标签
    ui->MV_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 高亮 MV 频道数字标签
    ui->check_box_widget->hide();                        ///< 隐藏复选框控件
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换按钮和数字标签样式
 */
bool RecentlyPlayed::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->single_song_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置单曲按钮悬停样式
            ui->single_song_number_label->setStyleSheet(ui->single_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置单曲数字标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->single_song_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置单曲按钮默认样式
            ui->single_song_number_label->setStyleSheet(ui->single_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : ""); ///< 重置单曲数字标签
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌单按钮悬停样式
            ui->song_list_number_label->setStyleSheet(ui->song_list_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置歌单数字标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌单按钮默认样式
            ui->song_list_number_label->setStyleSheet(ui->song_list_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : ""); ///< 重置歌单数字标签
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->video_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置视频按钮悬停样式
            ui->video_number_label->setStyleSheet(ui->video_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置视频数字标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->video_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置视频按钮默认样式
            ui->video_number_label->setStyleSheet(ui->video_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : ""); ///< 重置视频数字标签
        }
    }
    if (watched == ui->guide_widget4) {
        if (event->type() == QEvent::Enter) {
            ui->song_channel_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌曲频道按钮悬停样式
            ui->song_channel_number_label->setStyleSheet(ui->song_channel_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置歌曲频道数字标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->song_channel_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌曲频道按钮默认样式
            ui->song_channel_number_label->setStyleSheet(ui->song_channel_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : ""); ///< 重置歌曲频道数字标签
        }
    }
    if (watched == ui->guide_widget5) {
        if (event->type() == QEvent::Enter) {
            ui->MV_channel_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置 MV 频道按钮悬停样式
            ui->MV_channel_number_label->setStyleSheet(ui->MV_channel_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置 MV 频道数字标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->MV_channel_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置 MV 频道按钮默认样式
            ui->MV_channel_number_label->setStyleSheet(ui->MV_channel_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : ""); ///< 重置 MV 频道数字标签
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 处理数字标签点击以切换界面
 */
void RecentlyPlayed::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->single_song_number_label->geometry(); ///< 获取单曲数字标签区域
        const auto labelRect2 = ui->song_list_number_label->geometry(); ///< 获取歌单数字标签区域
        const auto labelRect3 = ui->video_number_label->geometry(); ///< 获取视频数字标签区域
        const auto labelRect4 = ui->song_channel_number_label->geometry(); ///< 获取歌曲频道数字标签区域
        const auto labelRect5 = ui->MV_channel_number_label->geometry(); ///< 获取 MV 频道数字标签区域
        const QPoint clickPos1 = ui->single_song_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换单曲标签点击坐标
        const QPoint clickPos2 = ui->song_list_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换歌单标签点击坐标
        const QPoint clickPos3 = ui->video_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换视频标签点击坐标
        const QPoint clickPos4 = ui->song_channel_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换歌曲频道标签点击坐标
        const QPoint clickPos5 = ui->MV_channel_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换 MV 频道标签点击坐标
        if (labelRect1.contains(clickPos1)) {
            ui->single_song_pushButton->clicked();       ///< 触发单曲按钮点击
        }
        if (labelRect2.contains(clickPos2)) {
            ui->song_list_pushButton->clicked();         ///< 触发歌单按钮点击
        }
        if (labelRect3.contains(clickPos3)) {
            ui->video_pushButton->clicked();             ///< 触发视频按钮点击
        }
        if (labelRect4.contains(clickPos4)) {
            ui->song_channel_pushButton->clicked();      ///< 触发歌曲频道按钮点击
        }
        if (labelRect5.contains(clickPos5)) {
            ui->MV_channel_pushButton->clicked();        ///< 触发 MV 频道按钮点击
        }
    }
}

/**
 * @brief 窗口显示事件
 * @param event 显示事件
 * @note 处理窗口显示逻辑，悬停事件代码被注释掉
 */
void RecentlyPlayed::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}