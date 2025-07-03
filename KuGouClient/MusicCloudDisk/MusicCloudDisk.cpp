/**
 * @file MusicCloudDisk.cpp
 * @brief 实现 MusicCloudDisk 类，管理云端歌曲（已上传和正在上传）界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "MusicCloudDisk.h"
#include "ui_MusicCloudDisk.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化云端歌曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
MusicCloudDisk::MusicCloudDisk(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicCloudDisk)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/cloud.css")); ///< 加载样式表
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
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true);                              ///< 动画结束恢复交互
    });
    enableButton(true);                                  ///< 初始启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
MusicCloudDisk::~MusicCloudDisk()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 初始化下标标签、新增歌曲按钮和堆栈窗口
 */
void MusicCloudDisk::initUi()
{
    initIndexLab();                                      ///< 初始化下标标签
    ui->new_add_toolButton->setIconSize(QSize(10, 10));  ///< 设置新增歌曲按钮图标大小
    ui->new_add_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg"))); ///< 设置默认图标
    ui->new_add_toolButton->setEnterIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-blue.svg"))); ///< 设置悬停图标
    ui->new_add_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/right-black.svg"))); ///< 设置离开图标
    ui->new_add_toolButton->setApproach(true);           ///< 启用接近模式
    ui->new_add_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF"))); ///< 设置悬停字体颜色
    initStackedWidget();                                 ///< 初始化堆栈窗口
    ui->uploaded_song_pushButton->clicked();             ///< 默认点击已上传歌曲
}

/**
 * @brief 初始化下标标签
 * @note 设置下标图片、默认样式和事件过滤器
 */
void MusicCloudDisk::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置下标图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;")); ///< 设置默认标签样式
    ui->idx2_lab->hide();                                ///< 隐藏下标 2
    ui->guide_widget1->installEventFilter(this);         ///< 安装事件过滤器
    ui->guide_widget2->installEventFilter(this);
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化已上传和正在上传歌曲界面并设置按钮互斥
 */
void MusicCloudDisk::initStackedWidget()
{
    initUploadedSong();                                  ///< 初始化已上传歌曲
    initUploadingSong();                                 ///< 初始化正在上传歌曲
    this->m_buttonGroup->addButton(ui->uploaded_song_pushButton); ///< 添加按钮到组
    this->m_buttonGroup->addButton(ui->uploading_song_pushButton);
    this->m_buttonGroup->setExclusive(true);             ///< 设置互斥
}

/**
 * @brief 初始化已上传歌曲界面
 */
void MusicCloudDisk::initUploadedSong()
{
    this->m_uploadedSong = std::make_unique<UploadedSong>(ui->stackedWidget); ///< 创建已上传歌曲界面
    connect(this->m_uploadedSong.get(), &UploadedSong::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_uploadedSong.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_uploadedSong.get()); ///< 设置当前界面
}

/**
 * @brief 初始化正在上传歌曲界面
 */
void MusicCloudDisk::initUploadingSong()
{
    this->m_uploadingSong = std::make_unique<UploadingSong>(ui->stackedWidget); ///< 创建正在上传歌曲界面
    connect(this->m_uploadingSong.get(), &UploadingSong::find_more_music, [this] {
        emit find_more_music();                          ///< 连接搜索信号
    });
    ui->stackedWidget->addWidget(this->m_uploadingSong.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void MusicCloudDisk::enableButton(const bool &flag) const
{
    ui->uploaded_song_pushButton->setEnabled(flag);      ///< 启用/禁用已上传歌曲按钮
    ui->uploading_song_pushButton->setEnabled(flag);     ///< 启用/禁用正在上传歌曲按钮
}

/**
 * @brief 已上传歌曲按钮点击槽函数
 * @note 切换到已上传歌曲界面，更新下标和标签样式
 */
void MusicCloudDisk::on_uploaded_song_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_uploadedSong.get())
        return;                                          ///< 页面已是目标页面，无需切换
    ui->uploaded_song_pushButton->setChecked(true);      ///< 设置选中状态
    STREAM_INFO() << "切换已上传歌曲界面";               ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_uploadedSong.get())); ///< 滑动切换界面
    ui->idx1_lab->show();                                ///< 显示下标 1
    ui->idx2_lab->hide();                                ///< 隐藏下标 2
    ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;")); ///< 设置标签样式
    ui->uploading_song_number_label->setStyleSheet("");  ///< 重置其他标签样式
}

/**
 * @brief 正在上传歌曲按钮点击槽函数
 * @note 切换到正在上传歌曲界面，更新下标和标签样式
 */
void MusicCloudDisk::on_uploading_song_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_uploadingSong.get())
        return;                                          ///< 页面已是目标页面，无需切换
    ui->uploading_song_pushButton->setChecked(true);     ///< 设置选中状态
    STREAM_INFO() << "切换正在上传界面";                ///< 记录日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_uploadingSong.get())); ///< 滑动切换界面
    ui->idx1_lab->hide();                                ///< 隐藏下标 1
    ui->idx2_lab->show();                                ///< 显示下标 2
    ui->uploaded_song_number_label->setStyleSheet("");   ///< 重置其他标签样式
    ui->uploading_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;")); ///< 设置标签样式
}

/**
 * @brief 新增歌曲按钮点击槽函数
 * @note 显示未实现提示
 */
void MusicCloudDisk::on_new_add_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->new_add_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理按钮和标签样式动态切换
 */
bool MusicCloudDisk::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->guide_widget1)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->uploaded_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");                                         ///< 设置悬停样式
            ui->uploaded_song_number_label->setStyleSheet(ui->uploaded_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 更新标签样式
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->uploaded_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");                                         ///< 设置离开样式
            ui->uploaded_song_number_label->setStyleSheet(ui->uploaded_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;") :
                QStringLiteral(""));                     ///< 更新标签样式
        }
    }
    if (watched == ui->guide_widget2)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->uploading_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");                                         ///< 设置悬停样式
            ui->uploading_song_number_label->setStyleSheet(ui->uploading_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;") :
                QStringLiteral("color:#26a1ff;"));       ///< 更新标签样式
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->uploading_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");                                         ///< 设置离开样式
            ui->uploading_song_number_label->setStyleSheet(ui->uploading_song_pushButton->isChecked() ?
                QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;") :
                QStringLiteral(""));                     ///< 更新标签样式
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 处理标签点击触发按钮点击
 */
void MusicCloudDisk::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        const auto labelRect1 = ui->uploaded_song_number_label->geometry(); ///< 获取标签区域
        const auto labelRect2 = ui->uploading_song_number_label->geometry();
        const QPoint clickPos1 = ui->uploaded_song_number_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换坐标
        const QPoint clickPos2 = ui->uploading_song_number_label->parentWidget()->mapFrom(this, event->pos());
        if (labelRect1.contains(clickPos1))
        {
            ui->uploaded_song_pushButton->clicked();     ///< 触发已上传歌曲按钮
        }
        if (labelRect2.contains(clickPos2))
        {
            ui->uploading_song_pushButton->clicked();    ///< 触发正在上传歌曲按钮
        }
    }
}