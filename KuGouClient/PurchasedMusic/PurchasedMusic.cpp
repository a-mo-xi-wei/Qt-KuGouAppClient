/**
 * @file PurchasedMusic.cpp
 * @brief 实现 PurchasedMusic 类，管理付费音乐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "PurchasedMusic.h"
#include "ui_PurchasedMusic.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QEasingCurve>
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化付费音乐界面
 * @param parent 父控件指针，默认为 nullptr
 */
PurchasedMusic::PurchasedMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PurchasedMusic)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/purchased.css"));
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
    initUi();
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
PurchasedMusic::~PurchasedMusic()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* PurchasedMusic::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            if (!m_paidSingle) {
                m_paidSingle = std::make_unique<PaidSingle>(ui->stackedWidget);
                connect(m_paidSingle.get(), &PaidSingle::find_more_music, this, &PurchasedMusic::find_more_music);
            }
            page = m_paidSingle.get();
            break;
        case 1:
            if (!m_purchasedAlbums) {
                m_purchasedAlbums = std::make_unique<PurchasedAlbums>(ui->stackedWidget);
                connect(m_purchasedAlbums.get(), &PurchasedAlbums::find_more_music, this, &PurchasedMusic::find_more_music);
            }
            page = m_purchasedAlbums.get();
            break;
        case 2:
            if (!m_purchasedVideos) {
                m_purchasedVideos = std::make_unique<PurchasedVideos>(ui->stackedWidget);
                connect(m_purchasedVideos.get(), &PurchasedVideos::find_more_music, this, &PurchasedMusic::find_more_music);
            }
            page = m_purchasedVideos.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 * @note 初始化索引标签、堆栈窗口和默认付费单曲界面
 */
void PurchasedMusic::initUi()
{
    initIndexLab();
    initStackedWidget();
    ui->paid_single_pushButton->click();
    ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief 初始化索引标签
 * @note 设置索引图片和事件过滤器
 */
void PurchasedMusic::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3 };
    QLabel* numLabels[] = { ui->paid_single_number_label, ui->purchased_albums_number_label, ui->purchased_video_number_label };

    for (int i = 0; i < 3; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Res/window/index_lab.svg"));
        guideWidgets[i]->installEventFilter(this);
        numLabels[i]->setStyleSheet(i == 0 ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        idxLabels[i]->setVisible(i == 0);
    }
}

/**
 * @brief 初始化堆栈窗口
 * @note 初始化子界面并设置按钮互斥
 */
void PurchasedMusic::initStackedWidget()
{
    // 设置按钮组
    m_buttonGroup->addButton(ui->paid_single_pushButton, 0);
    m_buttonGroup->addButton(ui->purchased_albums_pushButton, 1);
    m_buttonGroup->addButton(ui->purchased_video_pushButton, 2);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 3; ++i) {
        auto* placeholder = new QWidget;
        auto* layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    // 创建并添加默认页面（付费单曲）
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
                    switch (m_currentIdx) {
                        case 0: m_paidSingle.reset();break;
                        case 1: m_purchasedAlbums.reset();break;
                        case 2: m_purchasedVideos.reset();break;
                        default: break;
                    }
                }
                delete item;
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
        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab };
        QLabel* numLabels[] = { ui->paid_single_number_label, ui->purchased_albums_number_label, ui->purchased_video_number_label };
        for (int i = 0; i < 3; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        }

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 启用/禁用按钮
 * @param flag 是否启用
 */
void PurchasedMusic::enableButton(bool flag) const
{
    ui->paid_single_pushButton->setEnabled(flag);
    ui->purchased_albums_pushButton->setEnabled(flag);
    ui->purchased_video_pushButton->setEnabled(flag);
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool PurchasedMusic::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3 };
    QPushButton* buttons[] = { ui->paid_single_pushButton, ui->purchased_albums_pushButton, ui->purchased_video_pushButton };
    QLabel* numLabels[] = { ui->paid_single_number_label, ui->purchased_albums_number_label, ui->purchased_video_number_label };

    for (int i = 0; i < 3; ++i) {
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
void PurchasedMusic::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->paid_single_number_label, ui->purchased_albums_number_label, ui->purchased_video_number_label };
        QPushButton* buttons[] = { ui->paid_single_pushButton, ui->purchased_albums_pushButton, ui->purchased_video_pushButton };

        for (int i = 0; i < 3; ++i) {
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