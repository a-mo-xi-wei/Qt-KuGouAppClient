/**
 * @file LocalDownload.cpp
 * @brief 实现 LocalDownload 类，管理本地歌曲、已下载歌曲、已下载视频和正在下载界面
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.0
 */

#include "LocalDownload.h"
#include "ui_LocalDownload.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QFile>
#include <QButtonGroup>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化本地下载界面
 * @param parent 父控件指针，默认为 nullptr
 */
LocalDownload::LocalDownload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocalDownload)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/local.css")); ///< 加载样式表
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
    connect(this->m_localSong.get(), &LocalSong::playMusic, this, [this](const QString &localPath) {
        emit playMusic(localPath);                       ///< 中转播放音乐信号
    });
    connect(this->m_localSong.get(), &LocalSong::updateCountLabel, this, &LocalDownload::local_music_label_changed); ///< 连接数量标签更新
    connect(this->m_localSong.get(), &LocalSong::cancelLoopPlay, this, [this] {
        emit cancelLoopPlay();                           ///< 中转取消循环信号
    });
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true);                              ///< 动画结束恢复交互
    });
    enableButton(true);                                  ///< 初始启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
LocalDownload::~LocalDownload()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 音频播放结束处理
 * @note 转发给 LocalSong 处理
 */
void LocalDownload::audioFinished()
{
    this->m_localSong->onAudioFinished();                ///< 转发音频播放结束
}

/**
 * @brief 播放下一首本地歌曲
 */
void LocalDownload::playLocalSongNextSong()
{
    this->m_localSong->playNextSong();                   ///< 播放下一首
}

/**
 * @brief 播放上一首本地歌曲
 */
void LocalDownload::playLocalSongPrevSong()
{
    this->m_localSong->playPrevSong();                   ///< 播放上一首
}

/**
 * @brief 最大化屏幕处理
 * @note 更新当前播放高亮
 */
void LocalDownload::onMaxScreenHandle()
{
    this->m_localSong->onMaxScreenHandle();              ///< 转发最大化处理
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化四个子界面并设置按钮互斥
 */
void LocalDownload::initStackedWidget()
{
    initLocalSong();                                     ///< 初始化本地歌曲
    initDownloadedSong();                                ///< 初始化已下载歌曲
    initDownloadedVideo();                               ///< 初始化已下载视频
    initDownloading();                                   ///< 初始化正在下载
    this->m_buttonGroup->addButton(ui->local_music_pushButton); ///< 添加按钮到组
    this->m_buttonGroup->addButton(ui->downloaded_music_pushButton);
    this->m_buttonGroup->addButton(ui->downloaded_video_pushButton);
    this->m_buttonGroup->addButton(ui->downloading_pushButton);
    this->m_buttonGroup->setExclusive(true);             ///< 设置互斥
}

/**
 * @brief 初始化界面
 * @note 隐藏下载历史按钮、设置下标图片、初始化堆栈窗口和默认点击本地音乐
 */
void LocalDownload::initUi()
{
    ui->download_history_toolButton->hide();             ///< 隐藏下载历史按钮
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置下标图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->hide();                                ///< 隐藏下标 2-4
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    initStackedWidget();                                 ///< 初始化堆栈窗口
    ui->local_music_pushButton->clicked();               ///< 默认点击本地音乐
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画曲线
    ui->stackedWidget->setSpeed(400);                    ///< 设置动画速度
}

/**
 * @brief 初始化本地歌曲界面
 */
void LocalDownload::initLocalSong()
{
    this->m_localSong = std::make_unique<LocalSong>(ui->stackedWidget); ///< 创建本地歌曲界面
    connect(this->m_localSong.get(), &LocalSong::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_localSong.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_localSong.get()); ///< 设置当前界面
}

/**
 * @brief 初始化已下载歌曲界面
 */
void LocalDownload::initDownloadedSong()
{
    this->m_downloadedSong = std::make_unique<DownloadedSong>(ui->stackedWidget); ///< 创建已下载歌曲界面
    connect(this->m_downloadedSong.get(), &DownloadedSong::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_downloadedSong.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化已下载视频界面
 */
void LocalDownload::initDownloadedVideo()
{
    this->m_downloadedVideo = std::make_unique<DownloadedVideo>(ui->stackedWidget); ///< 创建已下载视频界面
    connect(this->m_downloadedVideo.get(), &DownloadedVideo::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_downloadedVideo.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化正在下载界面
 */
void LocalDownload::initDownloading()
{
    this->m_downloading = std::make_unique<Downloading>(ui->stackedWidget); ///< 创建正在下载界面
    connect(this->m_downloading.get(), &Downloading::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_downloading.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void LocalDownload::enableButton(const bool &flag) const
{
    ui->local_music_pushButton->setEnabled(flag);        ///< 启用/禁用本地音乐按钮
    ui->downloaded_music_pushButton->setEnabled(flag);   ///< 启用/禁用已下载音乐按钮
    ui->downloaded_video_pushButton->setEnabled(flag);   ///< 启用/禁用已下载视频按钮
    ui->downloading_pushButton->setEnabled(flag);        ///< 启用/禁用正在下载按钮
}

/**
 * @brief 本地音乐按钮点击槽函数
 * @note 切换到本地歌曲界面，更新下标和标签样式
 */
void LocalDownload::on_local_music_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_localSong.get())
        return;
    if (!ui->download_history_toolButton->isHidden())
        ui->download_history_toolButton->hide();         ///< 隐藏下载历史按钮
    STREAM_INFO() << "切换本地歌曲界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_localSong.get())); ///< 滑动切换界面
    ui->idx1_lab->show();                                ///< 显示下标 1
    ui->idx2_lab->hide();                                ///< 隐藏下标 2-4
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->local_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置标签样式
    ui->downloaded_music_number_label->setStyleSheet(""); ///< 重置其他标签样式
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

/**
 * @brief 已下载音乐按钮点击槽函数
 * @note 切换到已下载歌曲界面，更新下标和标签样式
 */
void LocalDownload::on_downloaded_music_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_downloadedSong.get())
        return;
    ui->download_history_toolButton->show();             ///< 显示下载历史按钮
    STREAM_INFO() << "切换下载歌曲界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_downloadedSong.get())); ///< 滑动切换界面
    ui->idx1_lab->hide();                                ///< 隐藏下标 1, 3-4
    ui->idx2_lab->show();                                ///< 显示下标 2
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->downloaded_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置标签样式
    ui->local_music_number_label->setStyleSheet("");     ///< 重置其他标签样式
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

/**
 * @brief 已下载视频按钮点击槽函数
 * @note 切换到已下载视频界面，更新下标和标签样式
 */
void LocalDownload::on_downloaded_video_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_downloadedVideo.get())
        return;
    if (!ui->download_history_toolButton->isHidden())
        ui->download_history_toolButton->hide();         ///< 隐藏下载历史按钮
    STREAM_INFO() << "切换下载视频界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_downloadedVideo.get())); ///< 滑动切换界面
    ui->idx1_lab->hide();                                ///< 隐藏下标 1-2, 4
    ui->idx2_lab->hide();
    ui->idx3_lab->show();                                ///< 显示下标 3
    ui->idx4_lab->hide();
    ui->downloaded_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置标签样式
    ui->local_music_number_label->setStyleSheet("");     ///< 重置其他标签样式
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

/**
 * @brief 正在下载按钮点击槽函数
 * @note 切换到正在下载界面，更新下标和标签样式
 */
void LocalDownload::on_downloading_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_downloading.get())
        return;
    if (!ui->download_history_toolButton->isHidden())
        ui->download_history_toolButton->hide();         ///< 隐藏下载历史按钮
    STREAM_INFO() << "切换正在下载界面";                 ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_downloading.get())); ///< 滑动切换界面
    ui->idx1_lab->hide();                                ///< 隐藏下标 1-3
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();                                ///< 显示下标 4
    ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置标签样式
    ui->local_music_number_label->setStyleSheet("");     ///< 重置其他标签样式
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
}

/**
 * @brief 下载历史按钮点击槽函数
 * @note 显示未实现提示
 */
void LocalDownload::on_download_history_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->download_history_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 本地音乐数量标签变化槽函数
 * @param num 歌曲数量
 * @note 更新本地音乐数量标签
 */
void LocalDownload::local_music_label_changed(const int &num)
{
    ui->local_music_number_label->setText(QString::number(num)); ///< 更新数量标签
}