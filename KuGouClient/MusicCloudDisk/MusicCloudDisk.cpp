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
#include <QTimer>

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
    QFile file(GET_CURRENT_DIR + QStringLiteral("/cloud.css"));
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
    QTimer::singleShot(100,this,[this] {initUi();});
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
MusicCloudDisk::~MusicCloudDisk()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* MusicCloudDisk::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            if (!m_uploadedSong) {
                m_uploadedSong = std::make_unique<UploadedSong>(ui->stackedWidget);
                connect(m_uploadedSong.get(), &UploadedSong::find_more_music, this, &MusicCloudDisk::find_more_music);
            }
            page = m_uploadedSong.get();
            break;
        case 1:
            if (!m_uploadingSong) {
                m_uploadingSong = std::make_unique<UploadingSong>(ui->stackedWidget);
                connect(m_uploadingSong.get(), &UploadingSong::find_more_music, this, &MusicCloudDisk::find_more_music);
            }
            page = m_uploadingSong.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 * @note 初始化下标标签、新增歌曲按钮和堆栈窗口
 */
void MusicCloudDisk::initUi()
{
    QTimer::singleShot(100,this,[this]{initIndexLab();});
    QTimer::singleShot(200,this,[this] {
        initStackedWidget();
        ui->new_add_toolButton->setIconSize(QSize(10, 10));
        ui->new_add_toolButton->setIcon(QIcon(":/MenuIcon/Res/menuIcon/right-black.svg"));
        ui->new_add_toolButton->setEnterIcon(QIcon(":/MenuIcon/Res/menuIcon/right-blue.svg"));
        ui->new_add_toolButton->setLeaveIcon(QIcon(":/MenuIcon/Res/menuIcon/right-black.svg"));
        ui->new_add_toolButton->setApproach(true);
        ui->new_add_toolButton->setHoverFontColor(QColor("#3AA1FF"));
        ui->uploaded_song_pushButton->click();
        ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
        ui->stackedWidget->setSpeed(400);
        ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
   });
}

/**
 * @brief 初始化下标标签
 * @note 设置下标图片、默认样式和事件过滤器
 */
void MusicCloudDisk::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2 };
    QLabel* numLabels[] = { ui->uploaded_song_number_label, ui->uploading_song_number_label };

    for (int i = 0; i < 2; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Res/window/index_lab.svg"));
        guideWidgets[i]->installEventFilter(this);
        numLabels[i]->setStyleSheet(i == 0 ? "color:#26a1ff;font-size:14px;font-weight:bold;" : "");
        idxLabels[i]->setVisible(i == 0);
    }
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化已上传和正在上传歌曲界面并设置按钮互斥
 */
void MusicCloudDisk::initStackedWidget()
{
    // 设置按钮组
    m_buttonGroup->addButton(ui->uploaded_song_pushButton, 0);
    m_buttonGroup->addButton(ui->uploading_song_pushButton, 1);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 2; ++i) {
        auto* placeholder = new QWidget;
        auto* layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    // 创建并添加默认页面（已上传歌曲）
    m_pages[0]->layout()->addWidget(createPage(0));
    ui->stackedWidget->setCurrentIndex(0);

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
                case 0: m_uploadedSong.reset();break;
                case 1: m_uploadingSong.reset();break;
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

        // 更新标签
        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab };
        QLabel* numLabels[] = { ui->uploaded_song_number_label, ui->uploading_song_number_label };
        for (int i = 0; i < 2; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? "color:#26a1ff;font-size:14px;font-weight:bold;" : "");
        }

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void MusicCloudDisk::enableButton(const bool& flag) const
{
    ui->uploaded_song_pushButton->setEnabled(flag);
    ui->uploading_song_pushButton->setEnabled(flag);
}

/**
 * @brief 新增歌曲按钮点击槽函数
 * @note 显示未实现提示
 */
void MusicCloudDisk::on_new_add_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->new_add_toolButton->text()),
                               1000, window());
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool MusicCloudDisk::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2 };
    QPushButton* buttons[] = { ui->uploaded_song_pushButton, ui->uploading_song_pushButton };
    QLabel* numLabels[] = { ui->uploaded_song_number_label, ui->uploading_song_number_label };

    for (int i = 0; i < 2; ++i) {
        if (watched == guideWidgets[i]) {
            if (event->type() == QEvent::Enter) {
                buttons[i]->setStyleSheet(R"(
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
                )");
                numLabels[i]->setStyleSheet(buttons[i]->isChecked() ?
                    "color:#26a1ff;font-size:14px;font-weight:bold;" :
                    "color:#26a1ff;");
            } else if (event->type() == QEvent::Leave) {
                buttons[i]->setStyleSheet(R"(
                    QPushButton {
                        color:black;
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
                )");
                numLabels[i]->setStyleSheet(buttons[i]->isChecked() ?
                    "color:#26a1ff;font-size:14px;font-weight:bold;" :
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
void MusicCloudDisk::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->uploaded_song_number_label, ui->uploading_song_number_label };
        QPushButton* buttons[] = { ui->uploaded_song_pushButton, ui->uploading_song_pushButton };

        for (int i = 0; i < 2; ++i) {
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