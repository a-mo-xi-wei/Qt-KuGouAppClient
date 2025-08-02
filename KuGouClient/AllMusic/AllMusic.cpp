/**
 * @file AllMusic.cpp
 * @brief 实现 AllMusic 类，提供音乐管理界面功能
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "AllMusic.h"
#include "ui_AllMusic.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐管理界面
 * @param parent 父控件指针，默认为 nullptr
 */
AllMusic::AllMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AllMusic)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/all.css"));
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

    auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this);
    m_sortOptMenu = menu->getMenu<SortOptionMenu>();

    QTimer::singleShot(100,this,[this] {initUi();});

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] { enableButton(true); });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
AllMusic::~AllMusic()
{
    delete ui;
}

/**
 * @brief 创建页面
 * @param id 页面索引
 * @return 创建的页面控件
 */
QWidget* AllMusic::createPage(int id)
{
    QWidget* page = nullptr;
    switch (id) {
        case 0:
            if (!m_allWidget) {
                m_allWidget = std::make_unique<AllWidget>(ui->stackedWidget);
                connect(m_allWidget.get(), &AllWidget::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allWidget.get();
            break;
        case 1:
            if (!m_allLove) {
                m_allLove = std::make_unique<AllLove>(ui->stackedWidget);
                connect(m_allLove.get(), &AllLove::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allLove.get();
            break;
        case 2:
            if (!m_allSongList) {
                m_allSongList = std::make_unique<AllSongList>(ui->stackedWidget);
                connect(m_allSongList.get(), &AllSongList::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allSongList.get();
            break;
        case 3:
            if (!m_allRecent) {
                m_allRecent = std::make_unique<AllRecent>(ui->stackedWidget);
                connect(m_allRecent.get(), &AllRecent::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allRecent.get();
            break;
        case 4:
            if (!m_allLocal) {
                m_allLocal = std::make_unique<AllLocal>(ui->stackedWidget);
                connect(m_allLocal.get(), &AllLocal::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allLocal.get();
            break;
        case 5:
            if (!m_allPaid) {
                m_allPaid = std::make_unique<AllPaid>(ui->stackedWidget);
                connect(m_allPaid.get(), &AllPaid::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allPaid.get();
            break;
        case 6:
            if (!m_allCloudDisk) {
                m_allCloudDisk = std::make_unique<AllCloudDisk>(ui->stackedWidget);
                connect(m_allCloudDisk.get(), &AllCloudDisk::find_more_music, this, &AllMusic::find_more_music);
            }
            page = m_allCloudDisk.get();
            break;
        default:
            qWarning() << "[WARNING] Invalid page ID:" << id;
            return nullptr;
    }
    return page;
}

/**
 * @brief 初始化界面
 */
void AllMusic::initUi()
{
    auto all_download_toolButton_toolTip = new ElaToolTip(ui->all_download_toolButton);
    all_download_toolButton_toolTip->setToolTip(QStringLiteral("下载"));
    auto all_share_toolButton_toolTip = new ElaToolTip(ui->all_share_toolButton);
    all_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));
    auto all_batch_toolButton_toolTip = new ElaToolTip(ui->all_batch_toolButton);
    all_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    auto all_sort_toolButton_toolTip = new ElaToolTip(ui->all_sort_toolButton);
    all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));

    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        Q_UNUSED(down);
        onDefaultSort();
        all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        onAddTimeSort(down);
        all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        onSongNameSort(down);
        all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        onSingerSort(down);
        all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        onDurationSort(down);
        all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, all_sort_toolButton_toolTip](bool down) {
        onPlayCountSort(down);
        all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序"));
    });
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, all_sort_toolButton_toolTip] {
        onRandomSort();
        all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机"));
    });

    ui->all_play_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg")));
    ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/download-gray.svg")));
    ui->all_download_toolButton->installEventFilter(this);

    m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    m_searchAction->setIconVisibleInMenu(false);
    ui->search_lineEdit->addAction(m_searchAction, QLineEdit::TrailingPosition);
    ui->search_lineEdit->setMaxWidth(150);
    ui->search_lineEdit->setBorderRadius(10);
    auto font = QFont("AaSongLiuKaiTi");
    font.setWeight(QFont::Bold);
    ui->search_lineEdit->setFont(font);

    QToolButton *searchButton = nullptr;
    for (auto *btn : ui->search_lineEdit->findChildren<QToolButton *>()) {
        if (btn->defaultAction() == m_searchAction) {
            searchButton = btn;
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton);
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));
            break;
        }
    }
    if (searchButton) {
        searchButton->installEventFilter(this);
    }

    QTimer::singleShot(100,this,[this]{initIndexLab();});
    QTimer::singleShot(200,this,[this] {
        initStackedWidget();
        ui->all_pushButton->click();
        ui->stackedWidget->setAnimation(QEasingCurve::OutQuart);
        ui->stackedWidget->setSpeed(400);
        ui->stackedWidget->setContentsMargins(0, 0, 0, 0);
        QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);
    });
}

/**
 * @brief 初始化索引标签
 */
void AllMusic::initIndexLab()
{
    QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab, ui->idx6_lab, ui->idx7_lab };
    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4, ui->guide_widget5, ui->guide_widget6, ui->guide_widget7 };
    QLabel* numLabels[] = { ui->all_label, ui->love_label, ui->song_list_label, ui->recent_label, ui->local_label, ui->paid_label, ui->cloud_disk_label };

    for (int i = 0; i < 7; ++i) {
        idxLabels[i]->setPixmap(QPixmap(":/Res/window/index_lab.svg"));
        guideWidgets[i]->installEventFilter(this);
        numLabels[i]->setStyleSheet(i == 0 ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        idxLabels[i]->setVisible(i == 0);
    }
}

/**
 * @brief 初始化堆栈窗口
 */
void AllMusic::initStackedWidget()
{
    m_buttonGroup->addButton(ui->all_pushButton, 0);
    m_buttonGroup->addButton(ui->love_pushButton, 1);
    m_buttonGroup->addButton(ui->song_list_pushButton, 2);
    m_buttonGroup->addButton(ui->recent_pushButton, 3);
    m_buttonGroup->addButton(ui->local_pushButton, 4);
    m_buttonGroup->addButton(ui->paid_pushButton, 5);
    m_buttonGroup->addButton(ui->cloud_disk_pushButton, 6);
    m_buttonGroup->setExclusive(true);

    for (int i = 0; i < 7; ++i) {
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
                case 0: m_allWidget.reset();break;
                case 1: m_allLove.reset();break;
                case 2: m_allSongList.reset();break;
                case 3: m_allRecent.reset();break;
                case 4: m_allLocal.reset();break;
                case 5: m_allPaid.reset();break;
                case 6: m_allCloudDisk.reset();break;
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

        QLabel* idxLabels[] = { ui->idx1_lab, ui->idx2_lab, ui->idx3_lab, ui->idx4_lab, ui->idx5_lab, ui->idx6_lab, ui->idx7_lab };
        QLabel* numLabels[] = { ui->all_label, ui->love_label, ui->song_list_label, ui->recent_label, ui->local_label, ui->paid_label, ui->cloud_disk_label };
        for (int i = 0; i < 7; ++i) {
            idxLabels[i]->setVisible(i == id);
            numLabels[i]->setStyleSheet(i == id ? "color:#26a1ff;font-size:16px;font-weight:bold;" : "");
        }

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString() << " 界面";
    });
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void AllMusic::enableButton(bool flag) const
{
    ui->all_pushButton->setEnabled(flag);
    ui->love_pushButton->setEnabled(flag);
    ui->song_list_pushButton->setEnabled(flag);
    ui->recent_pushButton->setEnabled(flag);
    ui->local_pushButton->setEnabled(flag);
    ui->paid_pushButton->setEnabled(flag);
    ui->cloud_disk_pushButton->setEnabled(flag);
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 */
bool AllMusic::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->all_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->all_download_toolButton->setIcon(QIcon(":/MenuIcon/Res/menuIcon/download-blue.svg"));
        } else if (event->type() == QEvent::Leave) {
            ui->all_download_toolButton->setIcon(QIcon(":/TabIcon/Res/tabIcon/download-gray.svg"));
        }
    }
    if (auto button = qobject_cast<QToolButton *>(watched); button && button->defaultAction() == m_searchAction) {
        if (event->type() == QEvent::Enter) {
            m_searchAction->setIcon(QIcon(":/MenuIcon/Res/menuIcon/search-blue.svg"));
        } else if (event->type() == QEvent::Leave) {
            m_searchAction->setIcon(QIcon(":/MenuIcon/Res/menuIcon/search-black.svg"));
        }
    }

    QWidget* guideWidgets[] = { ui->guide_widget1, ui->guide_widget2, ui->guide_widget3, ui->guide_widget4, ui->guide_widget5, ui->guide_widget6, ui->guide_widget7 };
    QPushButton* buttons[] = { ui->all_pushButton, ui->love_pushButton, ui->song_list_pushButton, ui->recent_pushButton, ui->local_pushButton, ui->paid_pushButton, ui->cloud_disk_pushButton };
    QLabel* numLabels[] = { ui->all_label, ui->love_label, ui->song_list_label, ui->recent_label, ui->local_label, ui->paid_label, ui->cloud_disk_label };

    for (int i = 0; i < 7; ++i) {
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
void AllMusic::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QLabel* numLabels[] = { ui->all_label, ui->love_label, ui->song_list_label, ui->recent_label, ui->local_label, ui->paid_label, ui->cloud_disk_label };
        QPushButton* buttons[] = { ui->all_pushButton, ui->love_pushButton, ui->song_list_pushButton, ui->recent_pushButton, ui->local_pushButton, ui->paid_pushButton, ui->cloud_disk_pushButton };

        for (int i = 0; i < 7; ++i) {
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
 * @brief 处理全部播放按钮点击
 */
void AllMusic::on_all_play_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 处理下载按钮点击
 */
void AllMusic::on_all_download_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "下载 功能暂未实现 敬请期待", 1000, window());
}

/**
 * @brief 处理分享按钮点击
 */
void AllMusic::on_all_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "分享 功能暂未实现 敬请期待", 1000, window());
}

/**
 * @brief 处理批量操作按钮点击
 */
void AllMusic::on_all_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, window());
}

/**
 * @brief 处理排序按钮点击
 */
void AllMusic::on_all_sort_toolButton_clicked()
{
    m_sortOptMenu->exec(QCursor::pos());
}

/**
 * @brief 默认排序
 */
void AllMusic::onDefaultSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 按添加时间排序
 * @param down 是否降序
 */
void AllMusic::onAddTimeSort(bool down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 按歌曲名称排序
 * @param down 是否降序
 */
void AllMusic::onSongNameSort(bool down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 按歌手排序
 * @param down 是否降序
 */
void AllMusic::onSingerSort(bool down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 按时长排序
 * @param down 是否降序
 */
void AllMusic::onDurationSort(bool down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 按播放次数排序
 * @param down 是否降序
 */
void AllMusic::onPlayCountSort(bool down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}

/**
 * @brief 随机排序
 */
void AllMusic::onRandomSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, window());
}