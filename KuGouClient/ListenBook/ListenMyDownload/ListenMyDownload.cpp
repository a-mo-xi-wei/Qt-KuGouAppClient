/**
 * @file ListenMyDownload.cpp
 * @brief 实现 ListenMyDownload 类，提供下载管理界面功能
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#include "ListenMyDownload.h"
#include "ui_ListenMyDownload.h"
#include "logger.hpp"

#include <QFile>
#include <QButtonGroup>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化下载管理界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenMyDownload::ListenMyDownload(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::ListenMyDownload)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/download.css"));
    if (file.open(QIODevice::ReadOnly)) {
        setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    connect(ui->stackedWidget,
            &SlidingStackedWidget::animationFinished,
            [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
ListenMyDownload::~ListenMyDownload()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget *ListenMyDownload::createPage(int id)
{
    QWidget *page = nullptr;
    switch (id) {
    case 0: if (!m_downloaded) {
            m_downloaded = std::make_unique<DownloadedWidget>(ui->stackedWidget);
            connect(m_downloaded.get(),
                    &DownloadedWidget::find_more_audio_book,
                    this,
                    &ListenMyDownload::switch_to_listen_recommend);
        }
        page = m_downloaded.get();
        break;
    case 1: if (!m_downloading) {
            m_downloading = std::make_unique<DownloadingWidget>(ui->stackedWidget);
            connect(m_downloading.get(),
                    &DownloadingWidget::find_more_audio_book,
                    this,
                    &ListenMyDownload::switch_to_listen_recommend);
        }
        page = m_downloading.get();
        break;
    default:
        qWarning() << "[WARNING] Invalid page ID:" << id;
        return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 * @note 初始化堆栈窗口和下标标签
 */
void ListenMyDownload::initUi()
{
    ui->guide_widget->setStyleSheet("font-family: 'TaiwanPearl';");
    initIndexLab();
    initStackedWidget();
    ui->downloaded_pushButton->click();
    ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
}

/**
 * @brief 初始化下标标签
 * @note 设置下标图片、样式和事件过滤器
 */
void ListenMyDownload::initIndexLab()
{
    QLabel *idxLabels[] = {ui->idx1_lab, ui->idx2_lab};
    QWidget *guideWidgets[] = {ui->guide_widget1, ui->guide_widget2};
    QLabel *numLabels[] = {ui->downloaded_number_label, ui->downloading_number_label};

    for (int i = 0; i < 2; ++i) {
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
void ListenMyDownload::initStackedWidget()
{
    // 设置按钮组
    m_buttonGroup->addButton(ui->downloaded_pushButton, 0);
    m_buttonGroup->addButton(ui->downloading_pushButton, 1);
    m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 2; ++i) {
        ui->stackedWidget->insertWidget(i, createPage(i));
    }

    ui->stackedWidget->setCurrentIndex(0);

    // 按钮点击处理
    connect(m_buttonGroup.get(),
            &QButtonGroup::idClicked,
            this,
            [this](int id) {
                if (m_currentIdx == id) {
                    return;
                }

                enableButton(false);

                ui->stackedWidget->slideInIdx(id);
                m_currentIdx = id;

                // 更新标签
                QLabel *idxLabels[] = {ui->idx1_lab, ui->idx2_lab};
                QLabel *numLabels[] = {ui->downloaded_number_label, ui->downloading_number_label};
                for (int i = 0; i < 2; ++i) {
                    idxLabels[i]->setVisible(i == id);
                    numLabels[i]->setStyleSheet(
                        i == id ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
                }

                STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
            });
}

/**
 * @brief 启用或禁用按钮
 * @param flag 启用标志
 */
void ListenMyDownload::enableButton(bool flag) const
{
    ui->downloaded_pushButton->setEnabled(flag);
    ui->downloading_pushButton->setEnabled(flag);
}

/**
 * @brief 事件过滤器，处理鼠标悬停事件
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool ListenMyDownload::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *guideWidgets[] = {ui->guide_widget1, ui->guide_widget2};
    QPushButton *buttons[] = {ui->downloaded_pushButton, ui->downloading_pushButton};
    QLabel *numLabels[] = {ui->downloaded_number_label, ui->downloading_number_label};

    for (int i = 0; i < 2; ++i) {
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
                numLabels[i]->setStyleSheet(buttons[i]->isChecked()
                                                ? "color:#26a1ff;font-size:16px;font-weight:bold;"
                                                : "color:#26a1ff;");
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
                numLabels[i]->setStyleSheet(buttons[i]->isChecked()
                                                ? "color:#26a1ff;font-size:16px;font-weight:bold;"
                                                : "");
            }
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}

/**
 * @brief 鼠标按下事件，处理标签点击
 * @param event 鼠标事件
 */
void ListenMyDownload::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel *numLabels[] = {ui->downloaded_number_label, ui->downloading_number_label};
        QPushButton *buttons[] = {ui->downloaded_pushButton, ui->downloading_pushButton};

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