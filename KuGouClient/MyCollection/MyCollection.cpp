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
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* MyCollection::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            m_singleSong = std::make_unique<SingleSong>(ui->stackedWidget);
            connect(m_singleSong.get(), &SingleSong::find_more_music, this, &MyCollection::find_more_music);
            page = m_singleSong.get();
            break;
        case 1:
            m_songList = std::make_unique<SongListWidget>(ui->stackedWidget);
            page = m_songList.get();
            break;
        case 2:
            m_specialAlbum = std::make_unique<SpecialAlbum>(ui->stackedWidget);
            connect(m_specialAlbum.get(), &SpecialAlbum::find_more_music, this, &MyCollection::find_more_music);
            page = m_specialAlbum.get();
            break;
        case 3:
            m_collectVideo = std::make_unique<CollectVideo>(ui->stackedWidget);
            connect(m_collectVideo.get(), &CollectVideo::find_more_music, this, &MyCollection::find_more_music);
            page = m_collectVideo.get();
            break;
        case 4:
            m_singerWidget = std::make_unique<SingerWidget>(ui->stackedWidget);
            connect(m_singerWidget.get(), &SingerWidget::find_more_music, this, &MyCollection::find_more_music);
            page = m_singerWidget.get();
            break;
        case 5:
            m_deviceWidget = std::make_unique<DeviceWidget>(ui->stackedWidget);
            connect(m_deviceWidget.get(), &DeviceWidget::find_more_music, this, &MyCollection::find_more_music);
            page = m_deviceWidget.get();
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
void MyCollection::initStackedWidget()
{
    // 设置按钮组（互斥）
    m_buttonGroup->addButton(ui->singleSong_pushButton, 0);
    m_buttonGroup->addButton(ui->songList_pushButton, 1);
    m_buttonGroup->addButton(ui->specialAlbum_pushButton, 2);
    m_buttonGroup->addButton(ui->collectVideo_pushButton, 3);
    m_buttonGroup->addButton(ui->singer_pushButton, 4);
    m_buttonGroup->addButton(ui->device_pushButton, 5);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 6; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    // 创建并添加默认页面（单曲）
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
                    // qDebug()<<"删除旧的控件";
                    widget->deleteLater();

                }
                delete item;
            }
            switch (m_currentIdx) {
                case 0: m_singleSong.reset(); break;
                case 1: m_songList.reset(); break;
                case 2: m_specialAlbum.reset(); break;
                case 3: m_collectVideo.reset(); break;
                case 4: m_singerWidget.reset(); break;
                case 5: m_deviceWidget.reset(); break;
                default: break;
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

        // 更新索引标签
        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab, ui->idx6_lab };
        QLabel* numLabels[] = { ui->singleSong_number_label, ui->songList_number_label, ui->specialAlbum_number_label,
                                ui->collectVideo_number_label, ui->singer_number_label, ui->device_number_label };
        for (int i = 0; i < 6; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;") : QString());
        }

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口、索引标签和默认单曲界面
 */
void MyCollection::initUi()
{
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                     ///< 初始化索引标签
    ui->singleSong_pushButton->click();                ///< 默认点击单曲按钮
    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置动画曲线
    ui->stackedWidget->setSpeed(400);                  ///< 设置动画速度
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0); ///< 设置边距
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片和事件过滤器
 */
void MyCollection::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab, ui->idx6_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3,
                               ui->guide_widget4, ui->guide_widget5, ui->guide_widget6 };
    QPushButton* buttons[] = { ui->singleSong_pushButton, ui->songList_pushButton, ui->specialAlbum_pushButton,
                              ui->collectVideo_pushButton, ui->singer_pushButton, ui->device_pushButton };
    QLabel* numLabels[] = { ui->singleSong_number_label, ui->songList_number_label, ui->specialAlbum_number_label,
                           ui->collectVideo_number_label, ui->singer_number_label, ui->device_number_label };

    for (int i = 0; i < 6; ++i) {
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
void MyCollection::enableButton(const bool &flag) const
{
    ui->singleSong_pushButton->setEnabled(flag);
    ui->songList_pushButton->setEnabled(flag);
    ui->specialAlbum_pushButton->setEnabled(flag);
    ui->collectVideo_pushButton->setEnabled(flag);
    ui->singer_pushButton->setEnabled(flag);
    ui->device_pushButton->setEnabled(flag);
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
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3,
                               ui->guide_widget4, ui->guide_widget5, ui->guide_widget6 };
    QPushButton* buttons[] = { ui->singleSong_pushButton, ui->songList_pushButton, ui->specialAlbum_pushButton,
                              ui->collectVideo_pushButton, ui->singer_pushButton, ui->device_pushButton };
    QLabel* numLabels[] = { ui->singleSong_number_label, ui->songList_number_label, ui->specialAlbum_number_label,
                           ui->collectVideo_number_label, ui->singer_number_label, ui->device_number_label };

    for (int i = 0; i < 6; ++i) {
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
void MyCollection::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->singleSong_number_label, ui->songList_number_label, ui->specialAlbum_number_label,
                               ui->collectVideo_number_label, ui->singer_number_label, ui->device_number_label };
        QPushButton* buttons[] = { ui->singleSong_pushButton, ui->songList_pushButton, ui->specialAlbum_pushButton,
                                  ui->collectVideo_pushButton, ui->singer_pushButton, ui->device_pushButton };

        for (int i = 0; i < 6; ++i) {
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