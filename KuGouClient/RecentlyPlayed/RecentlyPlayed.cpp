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
#include <QTimer>

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
    QFile file(GET_CURRENT_DIR + QStringLiteral("/recently.css"));
    if (file.open(QIODevice::ReadOnly))
    {
        setStyleSheet(file.readAll());
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    QTimer::singleShot(0,this,[this] {initUi();});
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
RecentlyPlayed::~RecentlyPlayed()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* RecentlyPlayed::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            if (!m_singleSong) {
                m_singleSong = std::make_unique<RecentlySingleSong>(ui->stackedWidget);
                connect(m_singleSong.get(), &RecentlySingleSong::find_more_music, this, &RecentlyPlayed::find_more_music);
            }
            page = m_singleSong.get();
            break;
        case 1:
            if (!m_songList) {
                m_songList = std::make_unique<RecentlySongList>(ui->stackedWidget);
                connect(m_songList.get(), &RecentlySongList::find_more_music, this, &RecentlyPlayed::find_more_music);
            }
            page = m_songList.get();
            break;
        case 2:
            if (!m_videoWidget) {
                m_videoWidget = std::make_unique<RecentlyVideoWidget>(ui->stackedWidget);
                connect(m_videoWidget.get(), &RecentlyVideoWidget::find_more_music, this, &RecentlyPlayed::find_more_music);
            }
            page = m_videoWidget.get();
            break;
        case 3:
            if (!m_songChannel) {
                m_songChannel = std::make_unique<RecentlySongChannel>(ui->stackedWidget);
                connect(m_songChannel.get(), &RecentlySongChannel::find_more_channel, this, &RecentlyPlayed::find_more_channel);
            }
            page = m_songChannel.get();
            break;
        case 4:
            if (!m_mvChannel) {
                m_mvChannel = std::make_unique<RecentlyMVChannel>(ui->stackedWidget);
                connect(m_mvChannel.get(), &RecentlyMVChannel::find_more_channel, this, &RecentlyPlayed::find_more_channel);
            }
            page = m_mvChannel.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口、索引标签和默认单曲界面
 */
void RecentlyPlayed::initUi()
{
    QTimer::singleShot(0,this,[this]{initIndexLab();});
    QTimer::singleShot(100,this,[this] {
        initStackedWidget();
        ui->single_song_pushButton->click();
        ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
        ui->stackedWidget->setSpeed(400);
        ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
        QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);
    });
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片、隐藏非当前索引和数字标签样式
 */
void RecentlyPlayed::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4, ui->guide_widget5 };
    QLabel* numLabels[] = { ui->single_song_number_label, ui->song_list_number_label, ui->video_number_label,
                            ui->song_channel_number_label, ui->MV_channel_number_label };

    for (int i = 0; i < 5; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Res/window/index_lab.svg"));
        guideWidgets[i]->installEventFilter(this);
        numLabels[i]->setStyleSheet(i == 0 ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        idxLabels[i]->setVisible(i == 0);
    }
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面和按钮组
 */
void RecentlyPlayed::initStackedWidget()
{
    // 设置按钮组
    m_buttonGroup->addButton(ui->single_song_pushButton, 0);
    m_buttonGroup->addButton(ui->song_list_pushButton, 1);
    m_buttonGroup->addButton(ui->video_pushButton, 2);
    m_buttonGroup->addButton(ui->song_channel_pushButton, 3);
    m_buttonGroup->addButton(ui->MV_channel_pushButton, 4);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 5; ++i) {
        auto* placeholder = new QWidget;
        auto* layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    // 创建并添加默认页面（单曲）
    m_pages[0]->layout()->addWidget(createPage(0));
    ui->stackedWidget->setCurrentIndex(0);
    ui->check_box_widget->show();

    // 按钮点击处理
    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](int id) {
        if (m_currentIdx == id) {
            return;
        }

        enableButton(false);

        // 清理目标 placeholder 内旧的控件
        QWidget* placeholder = m_pages[m_currentIdx];
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }

        QLayout* layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem* item = layout->takeAt(0)) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater();
                }
                delete item;
            }
            switch (m_currentIdx) {
                case 0: m_singleSong.reset();break;
                case 1: m_songList.reset();break;
                case 2: m_videoWidget.reset();break;
                case 3: m_songChannel.reset();break;
                case 4: m_mvChannel.reset();break;
                default: break;
            }
        }

        placeholder = m_pages[id];
        layout = placeholder->layout();
        // 创建新页面
        QWidget* realPage = createPage(id);
        if (!realPage) {
            qWarning() << "[WARNING] Failed to create page at index:" << id;
        } else {
            layout->addWidget(realPage);
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;

        // 更新标签和复选框
        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab };
        QLabel* numLabels[] = { ui->single_song_number_label, ui->song_list_number_label, ui->video_number_label,
                                ui->song_channel_number_label, ui->MV_channel_number_label };
        for (int i = 0; i < 5; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        }
        ui->check_box_widget->setVisible(id == 0);

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void RecentlyPlayed::enableButton(bool flag) const
{
    ui->single_song_pushButton->setEnabled(flag);
    ui->song_list_pushButton->setEnabled(flag);
    ui->video_pushButton->setEnabled(flag);
    ui->song_channel_pushButton->setEnabled(flag);
    ui->MV_channel_pushButton->setEnabled(flag);
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool RecentlyPlayed::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4, ui->guide_widget5 };
    QPushButton* buttons[] = { ui->single_song_pushButton, ui->song_list_pushButton, ui->video_pushButton,
                               ui->song_channel_pushButton, ui->MV_channel_pushButton };
    QLabel* numLabels[] = { ui->single_song_number_label, ui->song_list_number_label, ui->video_number_label,
                            ui->song_channel_number_label, ui->MV_channel_number_label };

    for (int i = 0; i < 5; ++i) {
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
                    "color:#26a1ff;font-size:16px;font-weight:bold;" :
                    "color:#26a1ff;");
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
                    "color:#26a1ff;font-size:16px;font-weight:bold;" :
                    "");
            }
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 */
void RecentlyPlayed::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->single_song_number_label, ui->song_list_number_label, ui->video_number_label,
                                ui->song_channel_number_label, ui->MV_channel_number_label };
        QPushButton* buttons[] = { ui->single_song_pushButton, ui->song_list_pushButton, ui->video_pushButton,
                                   ui->song_channel_pushButton, ui->MV_channel_pushButton };

        for (int i = 0; i < 5; ++i) {
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
 * @brief 窗口显示事件
 * @param event 显示事件
 */
void RecentlyPlayed::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}