/**
 * @file MyCollection.cpp
 * @brief 实现 MyCollection 类，管理收藏界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "MyCollection.h"
#include "ui_MyCollection.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化收藏界面
 * @param parent 父控件指针，默认为 nullptr
 */
MyCollection::MyCollection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyCollection)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/collection.css")); ///< 加载样式表
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
MyCollection::~MyCollection()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面并设置按钮互斥
 */
void MyCollection::initStackedWidget()
{
    initSingleSong();                                    ///< 初始化单曲界面
    initSongList();                                      ///< 初始化歌单界面
    initSpecialAlbum();                                  ///< 初始化专辑界面
    initCollectVideo();                                  ///< 初始化视频界面
    initSinger();                                        ///< 初始化歌手界面
    initDevice();                                        ///< 初始化设备界面
    this->m_buttonGroup->addButton(ui->singleSong_pushButton); ///< 添加单曲按钮
    this->m_buttonGroup->addButton(ui->songList_pushButton);   ///< 添加歌单按钮
    this->m_buttonGroup->addButton(ui->specialAlbum_pushButton); ///< 添加专辑按钮
    this->m_buttonGroup->addButton(ui->collectVideo_pushButton); ///< 添加视频按钮
    this->m_buttonGroup->addButton(ui->singer_pushButton);     ///< 添加歌手按钮
    this->m_buttonGroup->addButton(ui->device_pushButton);     ///< 添加设备按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮互斥
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口、索引标签和默认单曲界面
 */
void MyCollection::initUi()
{
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                      ///< 初始化索引标签
    ui->singleSong_pushButton->clicked();               ///< 默认点击单曲按钮
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画曲线
    ui->stackedWidget->setSpeed(400);                   ///< 设置动画速度
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);  ///< 设置边距
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片和事件过滤器
 */
void MyCollection::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置单曲索引图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置歌单索引图片
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置专辑索引图片
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置视频索引图片
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置歌手索引图片
    ui->idx6_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置设备索引图片
    ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置单曲标签样式
    ui->idx2_lab->hide();                                ///< 隐藏歌单索引
    ui->idx3_lab->hide();                                ///< 隐藏专辑索引
    ui->idx4_lab->hide();                                ///< 隐藏视频索引
    ui->idx5_lab->hide();                                ///< 隐藏歌手索引
    ui->idx6_lab->hide();                                ///< 隐藏设备索引
    ui->guide_widget1->installEventFilter(this);         ///< 安装单曲事件过滤器
    ui->guide_widget2->installEventFilter(this);         ///< 安装歌单事件过滤器
    ui->guide_widget3->installEventFilter(this);         ///< 安装专辑事件过滤器
    ui->guide_widget4->installEventFilter(this);         ///< 安装视频事件过滤器
    ui->guide_widget5->installEventFilter(this);         ///< 安装歌手事件过滤器
    ui->guide_widget6->installEventFilter(this);         ///< 安装设备事件过滤器
}

/**
 * @brief 初始化单曲界面
 * @note 创建单曲界面并连接信号
 */
void MyCollection::initSingleSong()
{
    this->m_singleSong = std::make_unique<SingleSong>(ui->stackedWidget); ///< 创建单曲界面
    connect(this->m_singleSong.get(), &SingleSong::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_singleSong.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get()); ///< 设置当前界面
}

/**
 * @brief 初始化歌单界面
 * @note 创建歌单界面
 */
void MyCollection::initSongList()
{
    this->m_songList = std::make_unique<SongListWidget>(ui->stackedWidget); ///< 创建歌单界面
    ui->stackedWidget->addWidget(this->m_songList.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化专辑界面
 * @note 创建专辑界面并连接信号
 */
void MyCollection::initSpecialAlbum()
{
    this->m_specialAlbum = std::make_unique<SpecialAlbum>(ui->stackedWidget); ///< 创建专辑界面
    connect(this->m_specialAlbum.get(), &SpecialAlbum::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_specialAlbum.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化视频界面
 * @note 创建视频界面并连接信号
 */
void MyCollection::initCollectVideo()
{
    this->m_collectVideo = std::make_unique<CollectVideo>(ui->stackedWidget); ///< 创建视频界面
    connect(this->m_collectVideo.get(), &CollectVideo::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_collectVideo.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化歌手界面
 * @note 创建歌手界面并连接信号
 */
void MyCollection::initSinger()
{
    this->m_singerWidget = std::make_unique<SingerWidget>(ui->stackedWidget); ///< 创建歌手界面
    connect(this->m_singerWidget.get(), &SingerWidget::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_singerWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化设备界面
 * @note 创建设备界面并连接信号
 */
void MyCollection::initDevice()
{
    this->m_deviceWidget = std::make_unique<DeviceWidget>(ui->stackedWidget); ///< 创建设备界面
    connect(this->m_deviceWidget.get(), &DeviceWidget::find_more_music, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_deviceWidget.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void MyCollection::enableButton(const bool &flag) const
{
    ui->singleSong_pushButton->setEnabled(flag);         ///< 设置单曲按钮状态
    ui->songList_pushButton->setEnabled(flag);           ///< 设置歌单按钮状态
    ui->specialAlbum_pushButton->setEnabled(flag);       ///< 设置专辑按钮状态
    ui->collectVideo_pushButton->setEnabled(flag);       ///< 设置视频按钮状态
    ui->singer_pushButton->setEnabled(flag);             ///< 设置歌手按钮状态
    ui->device_pushButton->setEnabled(flag);             ///< 设置设备按钮状态
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换按钮和标签样式
 */
bool MyCollection::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->singleSong_pushButton->setStyleSheet(R"(
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
            ui->singleSong_number_label->setStyleSheet(ui->singleSong_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置单曲标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->singleSong_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置单曲按钮离开样式
            ui->singleSong_number_label->setStyleSheet(ui->singleSong_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置单曲标签样式
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->songList_pushButton->setStyleSheet(R"(
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
            ui->songList_number_label->setStyleSheet(ui->songList_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置歌单标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->songList_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌单按钮离开样式
            ui->songList_number_label->setStyleSheet(ui->songList_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置歌单标签样式
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->specialAlbum_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置专辑按钮悬停样式
            ui->specialAlbum_number_label->setStyleSheet(ui->specialAlbum_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置专辑标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->specialAlbum_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置专辑按钮离开样式
            ui->specialAlbum_number_label->setStyleSheet(ui->specialAlbum_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置专辑标签样式
        }
    }
    if (watched == ui->guide_widget4) {
        if (event->type() == QEvent::Enter) {
            ui->collectVideo_pushButton->setStyleSheet(R"(
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
            ui->collectVideo_number_label->setStyleSheet(ui->collectVideo_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置视频标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->collectVideo_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置视频按钮离开样式
            ui->collectVideo_number_label->setStyleSheet(ui->collectVideo_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置视频标签样式
        }
    }
    if (watched == ui->guide_widget5) {
        if (event->type() == QEvent::Enter) {
            ui->singer_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌手按钮悬停样式
            ui->singer_number_label->setStyleSheet(ui->singer_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置歌手标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->singer_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置歌手按钮离开样式
            ui->singer_number_label->setStyleSheet(ui->singer_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置歌手标签样式
        }
    }
    if (watched == ui->guide_widget6) {
        if (event->type() == QEvent::Enter) {
            ui->device_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置设备按钮悬停样式
            ui->device_number_label->setStyleSheet(ui->device_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 设置设备标签样式
        } else if (event->type() == QEvent::Leave) {
            ui->device_pushButton->setStyleSheet(R"(
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
            )");                                         ///< 设置设备按钮离开样式
            ui->device_number_label->setStyleSheet(ui->device_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                QString(""));                            ///< 设置设备标签样式
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 点击标签切换界面
 */
void MyCollection::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->singleSong_number_label->geometry(); ///< 获取单曲标签区域
        const auto labelRect2 = ui->songList_number_label->geometry();   ///< 获取歌单标签区域
        const auto labelRect3 = ui->specialAlbum_number_label->geometry(); ///< 获取专辑标签区域
        const auto labelRect4 = ui->collectVideo_number_label->geometry(); ///< 获取视频标签区域
        const auto labelRect5 = ui->singer_number_label->geometry();     ///< 获取歌手标签区域
        const auto labelRect6 = ui->device_number_label->geometry();     ///< 获取设备标签区域
        const QPoint clickPos1 = ui->singleSong_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换单曲点击坐标
        const QPoint clickPos2 = ui->songList_number_label->parentWidget()->mapFrom(this, event->pos());   ///< 转换歌单点击坐标
        const QPoint clickPos3 = ui->specialAlbum_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换专辑点击坐标
        const QPoint clickPos4 = ui->collectVideo_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换视频点击坐标
        const QPoint clickPos5 = ui->singer_number_label->parentWidget()->mapFrom(this, event->pos());     ///< 转换歌手点击坐标
        const QPoint clickPos6 = ui->device_number_label->parentWidget()->mapFrom(this, event->pos());     ///< 转换设备点击坐标
        if (labelRect1.contains(clickPos1)) {
            ui->singleSong_pushButton->clicked();           ///< 触发单曲按钮点击
        }
        if (labelRect2.contains(clickPos2)) {
            ui->songList_pushButton->clicked();             ///< 触发歌单按钮点击
        }
        if (labelRect3.contains(clickPos3)) {
            ui->specialAlbum_pushButton->clicked();         ///< 触发专辑按钮点击
        }
        if (labelRect4.contains(clickPos4)) {
            ui->collectVideo_pushButton->clicked();         ///< 触发视频按钮点击
        }
        if (labelRect5.contains(clickPos5)) {
            ui->singer_pushButton->clicked();               ///< 触发歌手按钮点击
        }
        if (labelRect6.contains(clickPos6)) {
            ui->device_pushButton->clicked();               ///< 触发设备按钮点击
        }
    }
    QWidget::mousePressEvent(event);                     ///< 调用父类事件
}

/**
 * @brief 单曲按钮点击槽函数
 * @note 切换到单曲界面
 */
void MyCollection::on_singleSong_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_singleSong.get()) {
        return;                                          ///< 当前已是单曲界面
    }
    ui->singleSong_pushButton->setChecked(true);         ///< 设置单曲按钮选中
    STREAM_INFO() << "切换单曲界面";                     ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_singleSong.get())); ///< 滑动到单曲界面
    ui->idx1_lab->show();                               ///< 显示单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏歌单索引
    ui->idx3_lab->hide();                               ///< 隐藏专辑索引
    ui->idx4_lab->hide();                               ///< 隐藏视频索引
    ui->idx5_lab->hide();                               ///< 隐藏歌手索引
    ui->idx6_lab->hide();                               ///< 隐藏设备索引
    ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置单曲标签样式
    ui->songList_number_label->setStyleSheet("");        ///< 重置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet("");    ///< 重置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet("");    ///< 重置视频标签样式
    ui->singer_number_label->setStyleSheet("");          ///< 重置歌手标签样式
    ui->device_number_label->setStyleSheet("");          ///< 重置设备标签样式
}

/**
 * @brief 歌单按钮点击槽函数
 * @note 切换到歌单界面
 */
void MyCollection::on_songList_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_songList.get()) {
        return;                                          ///< 当前已是歌单界面
    }
    ui->songList_pushButton->setChecked(true);           ///< 设置歌单按钮选中
    STREAM_INFO() << "切换歌单界面";                     ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_songList.get())); ///< 滑动到歌单界面
    ui->idx1_lab->hide();                               ///< 隐藏单曲索引
    ui->idx2_lab->show();                               ///< 显示歌单索引
    ui->idx3_lab->hide();                               ///< 隐藏专辑索引
    ui->idx4_lab->hide();                               ///< 隐藏视频索引
    ui->idx5_lab->hide();                               ///< 隐藏歌手索引
    ui->idx6_lab->hide();                               ///< 隐藏设备索引
    ui->singleSong_number_label->setStyleSheet("");      ///< 重置单曲标签样式
    ui->songList_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet("");    ///< 重置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet("");    ///< 重置视频标签样式
    ui->singer_number_label->setStyleSheet("");          ///< 重置歌手标签样式
    ui->device_number_label->setStyleSheet("");          ///< 重置设备标签样式
}

/**
 * @brief 专辑按钮点击槽函数
 * @note 切换到专辑界面
 */
void MyCollection::on_specialAlbum_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_specialAlbum.get()) {
        return;                                          ///< 当前已是专辑界面
    }
    ui->specialAlbum_pushButton->setChecked(true);       ///< 设置专辑按钮选中
    STREAM_INFO() << "切换专辑界面";                     ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_specialAlbum.get())); ///< 滑动到专辑界面
    ui->idx1_lab->hide();                               ///< 隐藏单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏歌单索引
    ui->idx3_lab->show();                               ///< 显示专辑索引
    ui->idx4_lab->hide();                               ///< 隐藏视频索引
    ui->idx5_lab->hide();                               ///< 隐藏歌手索引
    ui->idx6_lab->hide();                               ///< 隐藏设备索引
    ui->singleSong_number_label->setStyleSheet("");      ///< 重置单曲标签样式
    ui->songList_number_label->setStyleSheet("");        ///< 重置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet("");    ///< 重置视频标签样式
    ui->singer_number_label->setStyleSheet("");          ///< 重置歌手标签样式
    ui->device_number_label->setStyleSheet("");          ///< 重置设备标签样式
}

/**
 * @brief 视频按钮点击槽函数
 * @note 切换到视频界面
 */
void MyCollection::on_collectVideo_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_collectVideo.get()) {
        return;                                          ///< 当前已是视频界面
    }
    ui->collectVideo_pushButton->setChecked(true);       ///< 设置视频按钮选中
    STREAM_INFO() << "切换视频界面";                     ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_collectVideo.get())); ///< 滑动到视频界面
    ui->idx1_lab->hide();                               ///< 隐藏单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏歌单索引
    ui->idx3_lab->hide();                               ///< 隐藏专辑索引
    ui->idx4_lab->show();                               ///< 显示视频索引
    ui->idx5_lab->hide();                               ///< 隐藏歌手索引
    ui->idx6_lab->hide();                               ///< 隐藏设备索引
    ui->singleSong_number_label->setStyleSheet("");      ///< 重置单曲标签样式
    ui->songList_number_label->setStyleSheet("");        ///< 重置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet("");    ///< 重置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置视频标签样式
    ui->singer_number_label->setStyleSheet("");          ///< 重置歌手标签样式
    ui->device_number_label->setStyleSheet("");          ///< 重置设备标签样式
}

/**
 * @brief 歌手按钮点击槽函数
 * @note 切换到歌手界面
 */
void MyCollection::on_singer_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_singerWidget.get()) {
        return;                                          ///< 当前已是歌手界面
    }
    ui->singer_pushButton->setChecked(true);             ///< 设置歌手按钮选中
    STREAM_INFO() << "切换歌手界面";                     ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_singerWidget.get())); ///< 滑动到歌手界面
    ui->idx1_lab->hide();                               ///< 隐藏单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏歌单索引
    ui->idx3_lab->hide();                               ///< 隐藏专辑索引
    ui->idx4_lab->hide();                               ///< 隐藏视频索引
    ui->idx5_lab->show();                               ///< 显示歌手索引
    ui->idx6_lab->hide();                               ///< 隐藏设备索引
    ui->singleSong_number_label->setStyleSheet("");      ///< 重置单曲标签样式
    ui->songList_number_label->setStyleSheet("");        ///< 重置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet("");    ///< 重置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet("");    ///< 重置视频标签样式
    ui->singer_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置歌手标签样式
    ui->device_number_label->setStyleSheet("");          ///< 重置设备标签样式
}

/**
 * @brief 设备按钮点击槽函数
 * @note 切换到设备界面
 */
void MyCollection::on_device_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_deviceWidget.get()) {
        return;                                          ///< 当前已是设备界面
    }
    ui->device_pushButton->setChecked(true);              ///< 设置设备按钮选中
    STREAM_INFO() << "切换设备界面";                       ///< 记录日志
    enableButton(false);                           ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_deviceWidget.get())); ///< 滑动到设备界面
    ui->idx1_lab->hide();                               ///< 隐藏单曲索引
    ui->idx2_lab->hide();                               ///< 隐藏歌单索引
    ui->idx3_lab->hide();                               ///< 隐藏专辑索引
    ui->idx4_lab->hide();                               ///< 隐藏视频索引
    ui->idx5_lab->hide();                               ///< 隐藏歌手索引
    ui->idx6_lab->show();                               ///< 显示设备索引
    ui->singleSong_number_label->setStyleSheet("");      ///< 重置单曲标签样式
    ui->songList_number_label->setStyleSheet("");        ///< 重置歌单标签样式
    ui->specialAlbum_number_label->setStyleSheet("");    ///< 重置专辑标签样式
    ui->collectVideo_number_label->setStyleSheet("");    ///< 重置视频标签样式
    ui->singer_number_label->setStyleSheet("");          ///< 重置歌手标签样式
    ui->device_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置设备标签样式
}