/**
 * @file LocalDownload.cpp
 * @brief 实现 LocalDownload 类，管理本地歌曲、已下载歌曲、已下载视频和正在下载界面
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.1
 */

#include "LocalDownload.h"
#include "ui_LocalDownload.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QFile>
#include <QButtonGroup>
#include <QMouseEvent>

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
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); }); ///< 连接动画完成信号
    connect(this->m_localSong.get(), &LocalSong::playMusic, this, [this](const QString &localPath) {
        emit playMusic(localPath);                       ///< 中转播放音乐信号
    });
    connect(this->m_localSong.get(), &LocalSong::updateCountLabel, this, &LocalDownload::local_music_label_changed); ///< 连接数量标签更新
    connect(this->m_localSong.get(), &LocalSong::cancelLoopPlay, this, [this] {
        emit cancelLoopPlay();                           ///< 中转取消循环信号
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
    if (m_localSong) {
        m_localSong->onAudioFinished();                  ///< 转发音频播放结束
    }
}

/**
 * @brief 播放下一首本地歌曲
 */
void LocalDownload::playLocalSongNextSong()
{
    if (m_localSong) {
        m_localSong->playNextSong();                     ///< 播放下一首
    }
}

/**
 * @brief 播放上一首本地歌曲
 */
void LocalDownload::playLocalSongPrevSong()
{
    if (m_localSong) {
        m_localSong->playPrevSong();                     ///< 播放上一首
    }
}

/**
 * @brief 最大化屏幕处理
 * @note 更新当前播放高亮
 */
void LocalDownload::onMaxScreenHandle()
{
    if (m_localSong) {
        m_localSong->onMaxScreenHandle();                ///< 转发最大化处理
    }
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* LocalDownload::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            m_localSong = std::make_unique<LocalSong>(ui->stackedWidget);
            connect(m_localSong.get(), &LocalSong::find_more_music, this, &LocalDownload::find_more_music);
            connect(m_localSong.get(), &LocalSong::playMusic, this, &LocalDownload::playMusic);
            connect(m_localSong.get(), &LocalSong::updateCountLabel, this, &LocalDownload::local_music_label_changed);
            connect(m_localSong.get(), &LocalSong::cancelLoopPlay, this, &LocalDownload::cancelLoopPlay);
            page = m_localSong.get();
            break;
        case 1:
            m_downloadedSong = std::make_unique<DownloadedSong>(ui->stackedWidget);
            connect(m_downloadedSong.get(), &DownloadedSong::find_more_music, this, &LocalDownload::find_more_music);
            page = m_downloadedSong.get();
            break;
        case 2:
            m_downloadedVideo = std::make_unique<DownloadedVideo>(ui->stackedWidget);
            connect(m_downloadedVideo.get(), &DownloadedVideo::find_more_music, this, &LocalDownload::find_more_music);
            page = m_downloadedVideo.get();
            break;
        case 3:
            m_downloading = std::make_unique<Downloading>(ui->stackedWidget);
            connect(m_downloading.get(), &Downloading::find_more_music, this, &LocalDownload::find_more_music);
            page = m_downloading.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面并设置按钮互斥
 */
void LocalDownload::initStackedWidget()
{
    // 设置按钮组（互斥）
    m_buttonGroup->addButton(ui->local_music_pushButton, 0);
    m_buttonGroup->addButton(ui->downloaded_music_pushButton, 1);
    m_buttonGroup->addButton(ui->downloaded_video_pushButton, 2);
    m_buttonGroup->addButton(ui->downloading_pushButton, 3);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 4; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    // 创建并添加默认页面（本地歌曲）
    m_pages[0]->layout()->addWidget(createPage(0));

    ui->stackedWidget->setCurrentIndex(0);

    // 响应按钮点击事件
    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](const int& id) {
        if (m_currentIdx == id) {
            return;
        }

        enableButton(false);

        // 清理目标 placeholder 内旧的控件
        QWidget *placeholder = m_pages[m_currentIdx];
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }

        QLayout *layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem* item = layout->takeAt(0)) {
                if (QWidget* widget = item->widget()) {
                    // qDebug()<<"删除旧控件";
                    widget->deleteLater();
                    switch (m_currentIdx) {
                        case 0: m_localSong.reset(); break;
                        case 1: m_downloadedSong.reset(); break;
                        case 2: m_downloadedVideo.reset(); break;
                        case 3: m_downloading.reset(); break;
                        default: break;
                    }
                }
                delete item;
            }
        }

        placeholder = m_pages[id];
        layout = placeholder->layout();
        // 创建新页面
        QWidget *realPage = createPage(id);

        if (!realPage) {
            qWarning() << "[WARNING] Failed to create page at index:" << id;
        } else {
            layout->addWidget(realPage);
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;

        // 更新索引标签和样式
        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab };
        QLabel* numLabels[] = { ui->local_music_number_label, ui->downloaded_music_number_label,
                               ui->downloaded_video_number_label, ui->downloading_number_label };
        for (int i = 0; i < 4; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : QString());
        }

        // 处理下载历史按钮
        ui->download_history_toolButton->setVisible(id == 1);

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口、索引标签和默认本地歌曲界面
 */
void LocalDownload::initUi()
{
    ui->download_history_toolButton->hide();             ///< 隐藏下载历史按钮
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                     ///< 初始化索引标签
    ui->local_music_pushButton->click();                ///< 默认点击本地音乐按钮
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画曲线
    ui->stackedWidget->setSpeed(400);                   ///< 设置动画速度
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);  ///< 设置边距
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片和事件过滤器
 */
void LocalDownload::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4 };
    QPushButton* buttons[] = { ui->local_music_pushButton, ui->downloaded_music_pushButton,
                              ui->downloaded_video_pushButton, ui->downloading_pushButton };
    QLabel* numLabels[] = { ui->local_music_number_label, ui->downloaded_music_number_label,
                           ui->downloaded_video_number_label, ui->downloading_number_label };

    for (int i = 0; i < 4; ++i) {
        idxLabels[i]->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
        guideWidgets[i]->installEventFilter(this);
        if (i == 0) {
            numLabels[i]->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
        } else {
            idxLabels[i]->hide();
            numLabels[i]->setStyleSheet(QString());
        }
    }
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void LocalDownload::enableButton(const bool &flag) const
{
    ui->local_music_pushButton->setEnabled(flag);
    ui->downloaded_music_pushButton->setEnabled(flag);
    ui->downloaded_video_pushButton->setEnabled(flag);
    ui->downloading_pushButton->setEnabled(flag);
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换按钮和标签样式
 */
bool LocalDownload::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4 };
    QPushButton* buttons[] = { ui->local_music_pushButton, ui->downloaded_music_pushButton,
                              ui->downloaded_video_pushButton, ui->downloading_pushButton };
    QLabel* numLabels[] = { ui->local_music_number_label, ui->downloaded_music_number_label,
                           ui->downloaded_video_number_label, ui->downloading_number_label };

    for (int i = 0; i < 4; ++i) {
        if (watched == guideWidgets[i]) {
            if (event->type() == QEvent::Enter) {
                buttons[i]->setStyleSheet(R"(
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
                )");
                numLabels[i]->setStyleSheet(buttons[i]->isChecked() ?
                    QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                    QStringLiteral("color:#26a1ff;"));
            } else if (event->type() == QEvent::Leave) {
                buttons[i]->setStyleSheet(R"(
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
                )");
                numLabels[i]->setStyleSheet(buttons[i]->isChecked() ?
                    QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") :
                    QString());
            }
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 点击标签切换界面
 */
void LocalDownload::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->local_music_number_label, ui->downloaded_music_number_label,
                               ui->downloaded_video_number_label, ui->downloading_number_label };
        QPushButton* buttons[] = { ui->local_music_pushButton, ui->downloaded_music_pushButton,
                                  ui->downloaded_video_pushButton, ui->downloading_pushButton };

        for (int i = 0; i < 4; ++i) {
            const auto labelRect = numLabels[i]->geometry();
            const QPoint clickPos = numLabels[i]->parentWidget()->mapFrom(this, event->pos());
            if (labelRect.contains(clickPos)) {
                buttons[i]->click();
                break;
            }
        }
    }
    QWidget::mousePressEvent(event);
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