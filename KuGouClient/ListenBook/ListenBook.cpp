/**
 * @file ListenBook.cpp
 * @brief 实现 ListenBook 类，提供听书主界面功能
 * @author WeiWang
 * @date 2024-11-18
 * @version 1.0
 */

#include "ListenBook.h"
#include "ui_ListenBook.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化听书主界面
 * @param parent 父控件指针，默认为 nullptr
 */
ListenBook::ListenBook(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenBook)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/listen.css"));
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
    initStackedWidget();
    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
ListenBook::~ListenBook()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* ListenBook::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0: // Recommend
            if (!m_listenRecommend) {
                m_listenRecommend = std::make_unique<ListenRecommend>(ui->stackedWidget);
            }
            page = m_listenRecommend.get();
            break;
        case 1: // My Download
            if (!m_listenMyDownload) {
                m_listenMyDownload = std::make_unique<ListenMyDownload>(ui->stackedWidget);
                connect(m_listenMyDownload.get(), &ListenMyDownload::switch_to_listen_recommend, this, [this] {
                    ui->listen_recommend_toolButton->click();
                    ui->listen_recommend_toolButton->setChecked(true);
                });
            }
            page = m_listenMyDownload.get();
            break;
        case 2: // Recently Play
            if (!m_listenRecentlyPlay) {
                m_listenRecentlyPlay = std::make_unique<ListenRecentlyPlay>(ui->stackedWidget);
                connect(m_listenRecentlyPlay.get(), &ListenRecentlyPlay::switch_to_listen_recommend, this, [this] {
                    ui->listen_recommend_toolButton->click();
                    ui->listen_recommend_toolButton->setChecked(true);
                });
            }
            page = m_listenRecentlyPlay.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 * @note 设置按钮图标、文本和指示器样式
 */
void ListenBook::initUi()
{
    QToolButton* buttons[] = {
        ui->listen_recommend_toolButton,
        ui->listen_my_download_toolButton,
        ui->recently_play_toolButton
    };

    const QString iconPaths[][2] = {
        {":/ListenBook/Res/listenbook/recommend-black.svg", ":/ListenBook/Res/listenbook/recommend-gray.svg"},
        {":/ListenBook/Res/listenbook/download-black.svg", ":/ListenBook/Res/listenbook/download-gray.svg"},
        {":/ListenBook/Res/listenbook/recent-black.svg", ":/ListenBook/Res/listenbook/recent-gray.svg"}
    };
    const QSize iconSizes[] = { QSize(17, 17), QSize(21, 21), QSize(19, 19) };
    const QString texts[] = { "   推荐", "  我的下载", "   最近播放" };

    for (int i = 0; i < 3; ++i) {
        buttons[i]->setIcon(QIcon(iconPaths[i][1]));
        buttons[i]->setIconSize(iconSizes[i]);
        buttons[i]->setText(texts[i]);
        connect(buttons[i], &QToolButton::toggled, buttons[i], [=](bool checked) {
            buttons[i]->setIcon(QIcon(iconPaths[i][checked ? 0 : 1]));
        });
    }

    ui->indicator_toolButton->setStyleSheet(R"(QToolButton{
        background-color:transparent;
        border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
    }
    QToolButton:hover{
        border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
    })");
    connect(ui->indicator_toolButton, &QToolButton::toggled, ui->indicator_toolButton, [=](bool checked) {
        ui->indicator_toolButton->setStyleSheet(checked ?
            R"(QToolButton{
                background-color:transparent;
                border-image:url(:/ListenBook/Res/listenbook/down-black.svg);
            }
            QToolButton:hover{
                border-image:url(:/ListenBook/Res/listenbook/down-blue.svg);
            })" :
            R"(QToolButton{
                background-color:transparent;
                border-image:url(:/ListenBook/Res/listenbook/up-black.svg);
            }
            QToolButton:hover{
                border-image:url(:/ListenBook/Res/listenbook/up-blue.svg);
            })");
    });

    ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
    ui->stackedWidget->setVerticalMode(true);
}

/**
 * @brief 初始化堆栈窗口
 * @note 创建页面控件并设置按钮互斥
 */
void ListenBook::initStackedWidget()
{
    m_buttonGroup->addButton(ui->listen_recommend_toolButton, 0);
    m_buttonGroup->addButton(ui->listen_my_download_toolButton, 1);
    m_buttonGroup->addButton(ui->recently_play_toolButton, 2);
    m_buttonGroup->setExclusive(true);

    for (int i = 0; i < 3; ++i) {
        auto* placeholder = new QWidget;
        auto* layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    m_pages[0]->layout()->addWidget(createPage(0));
    ui->stackedWidget->setCurrentIndex(0);

    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](int id) {
        if (m_currentIdx == id) {
            return;
        }

        enableButton(false);

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
                case 0: m_listenRecommend.reset();break;
                case 1: m_listenMyDownload.reset();break;
                case 2: m_listenRecentlyPlay.reset();break;
                default: break;
            }
        }

        placeholder = m_pages[id];
        layout = placeholder->layout();

        QWidget* realPage = createPage(id);
        if (!realPage) {
            qWarning() << "[WARNING] Failed to create page at index:" << id;
        } else {
            layout->addWidget(realPage);
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;

        enableButton(true);
        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });

    QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);

    ui->listen_recommend_toolButton->click();
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void ListenBook::enableButton(bool flag) const
{
    ui->listen_recommend_toolButton->setEnabled(flag);
    ui->listen_my_download_toolButton->setEnabled(flag);
    ui->recently_play_toolButton->setEnabled(flag);
}