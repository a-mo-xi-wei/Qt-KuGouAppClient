//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AllMusic.h" resolved

#include "AllMusic.h"
#include "ui_AllMusic.h"
#include "MyMenu.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

AllMusic::AllMusic(QWidget *parent) :
    QWidget(parent), ui(new Ui::AllMusic)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/all.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();

    //动画结束，恢复可交互
    connect(ui->stackedWidget,&SlidingStackedWidget::animationFinished,[this]{enableButton(true);});
    enableButton(true);
}

AllMusic::~AllMusic() {
    delete ui;
}

void AllMusic::initUi() {
    initStackedWidget();
    initIndexLab();
    ui->all_pushButton->clicked();
    ui->all_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
    ui->all_download_toolButton->installEventFilter(this);

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->all_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->all_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->all_search_lineEdit->findChildren<QToolButton*>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            break;
        }
    }

    // 安装事件过滤器
    if (searchButton) {
        searchButton->installEventFilter(this);
    }

    //初始化menu
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this);
    this->m_sortOptMenu = menu->getMenu<SortOptionMenu>();

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart);
    ui->stackedWidget->setSpeed(400);
}

void AllMusic::initIndexLab() {
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx6_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx7_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
    ui->guide_widget3->installEventFilter(this);
    ui->guide_widget4->installEventFilter(this);
    ui->guide_widget5->installEventFilter(this);
    ui->guide_widget6->installEventFilter(this);
    ui->guide_widget7->installEventFilter(this);
}

void AllMusic::initStackedWidget() {
    initAllWidget();
    initAllLove();
    initAllSongList();
    initAllRecent();
    initAllLocal();
    initAllPaid();
    initAllCloudDisk();
    this->m_buttonGroup->addButton(ui->all_pushButton);
    this->m_buttonGroup->addButton(ui->love_pushButton);
    this->m_buttonGroup->addButton(ui->song_list_pushButton);
    this->m_buttonGroup->addButton(ui->recent_pushButton);
    this->m_buttonGroup->addButton(ui->local_pushButton);
    this->m_buttonGroup->addButton(ui->paid_pushButton);
    this->m_buttonGroup->addButton(ui->cloud_disk_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void AllMusic::initAllWidget() {
    this->m_allWidget = std::make_unique<AllWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allWidget.get());
    ui->stackedWidget->setCurrentWidget(this->m_allWidget.get());
}

void AllMusic::initAllLove() {
    this->m_allLove = std::make_unique<AllLove>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allLove.get());
}

void AllMusic::initAllSongList() {
    this->m_allSongList = std::make_unique<AllSongList>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allSongList.get());
}

void AllMusic::initAllRecent() {
    this->m_allRecent = std::make_unique<AllRecent>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allRecent.get());
}

void AllMusic::initAllLocal() {
    this->m_allLocal = std::make_unique<AllLocal>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allLocal.get());
}

void AllMusic::initAllPaid() {
    this->m_allPaid = std::make_unique<AllPaid>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allPaid.get());
}

void AllMusic::initAllCloudDisk() {
    this->m_allCloudDisk = std::make_unique<AllCloudDisk>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_allCloudDisk.get());
}

void AllMusic::getMenuPosition(const QPoint &pos)  {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    const int menuRightPos = pos.x() + m_sortOptMenu->width();
    const int menuBottomPos = pos.y() + m_sortOptMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
}

void AllMusic::enableButton(const bool &flag) const {
    ui->all_pushButton->setEnabled(flag);
    ui->love_pushButton->setEnabled(flag);
    ui->song_list_pushButton->setEnabled(flag);
    ui->recent_pushButton->setEnabled(flag);
    ui->local_pushButton->setEnabled(flag);
    ui->paid_pushButton->setEnabled(flag);
    ui->cloud_disk_pushButton->setEnabled(flag);
}

void AllMusic::on_all_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allWidget.get())return;
    ui->all_pushButton->setChecked(true);
    STREAM_INFO()<<"切换全部界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allWidget.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allWidget.get()));
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_love_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allLove.get())return;
    ui->love_pushButton->setChecked(true);
    STREAM_INFO()<<"切换喜欢界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allLove.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allLove.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_song_list_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allSongList.get())return;
    ui->song_list_pushButton->setChecked(true);
    STREAM_INFO()<<"切换歌单界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allSongList.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allSongList.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_recent_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allRecent.get())return;
    ui->recent_pushButton->setChecked(true);
    STREAM_INFO()<<"切换最近界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allRecent.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allRecent.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_local_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allLocal.get())return;
    ui->local_pushButton->setChecked(true);
    STREAM_INFO()<<"切换本地界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allLocal.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allLocal.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->show();
    ui->idx6_lab->hide();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_paid_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allPaid.get())return;
    ui->paid_pushButton->setChecked(true);
    STREAM_INFO()<<"切换已购界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allPaid.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allPaid.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->show();
    ui->idx7_lab->hide();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->cloud_disk_label->setStyleSheet("");
}

void AllMusic::on_cloud_disk_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_allCloudDisk.get())return;
    ui->cloud_disk_pushButton->setChecked(true);
    STREAM_INFO()<<"切换云盘界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_allCloudDisk.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allCloudDisk.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->idx7_lab->show();
    ui->all_label->setStyleSheet("");
    ui->love_label->setStyleSheet("");
    ui->song_list_label->setStyleSheet("");
    ui->recent_label->setStyleSheet("");
    ui->local_label->setStyleSheet("");
    ui->paid_label->setStyleSheet("");
    ui->cloud_disk_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
}

void AllMusic::on_all_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->move(this->m_menuPosition);
    this->m_sortOptMenu->show();
}

bool AllMusic::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->all_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
        }
    }
    if (const auto button = qobject_cast<QToolButton*>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->all_pushButton->setStyleSheet(R"(
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
            if (ui->all_pushButton->isChecked())
                ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->all_pushButton->setStyleSheet(R"(
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
            if (ui->all_pushButton->isChecked())
                ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->all_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->love_pushButton->setStyleSheet(R"(
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
            if (ui->love_pushButton->isChecked())
                ui->love_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->love_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->love_pushButton->setStyleSheet(R"(
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
            if (ui->love_pushButton->isChecked())
                ui->love_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->love_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            if (ui->song_list_pushButton->isChecked())
                ui->song_list_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_list_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            if (ui->song_list_pushButton->isChecked())
                ui->song_list_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_list_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget4) {
        if (event->type() == QEvent::Enter) {
            ui->recent_pushButton->setStyleSheet(R"(
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
            if (ui->recent_pushButton->isChecked())
                ui->recent_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->recent_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->recent_pushButton->setStyleSheet(R"(
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
            if (ui->recent_pushButton->isChecked())
                ui->recent_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->recent_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget5) {
        if (event->type() == QEvent::Enter) {
            ui->local_pushButton->setStyleSheet(R"(
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
            if (ui->local_pushButton->isChecked())
                ui->local_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->local_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->local_pushButton->setStyleSheet(R"(
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
            if (ui->local_pushButton->isChecked())
                ui->local_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->local_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget6) {
        if (event->type() == QEvent::Enter) {
            ui->paid_pushButton->setStyleSheet(R"(
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
            if (ui->paid_pushButton->isChecked())
                ui->paid_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->paid_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->paid_pushButton->setStyleSheet(R"(
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
            if (ui->paid_pushButton->isChecked())
                ui->paid_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->paid_label->setStyleSheet("");
        }
    }
    else if (watched == ui->guide_widget7) {
        if (event->type() == QEvent::Enter) {
            ui->cloud_disk_pushButton->setStyleSheet(R"(
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
            if (ui->cloud_disk_pushButton->isChecked())
                ui->cloud_disk_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->cloud_disk_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->cloud_disk_pushButton->setStyleSheet(R"(
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
            if (ui->cloud_disk_pushButton->isChecked())
                ui->cloud_disk_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->cloud_disk_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void AllMusic::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        // 获取 label 的矩形区域
        const auto labelRect1 = ui->all_label          ->geometry();
        const auto labelRect2 = ui->love_label         ->geometry();
        const auto labelRect3 = ui->song_list_label    ->geometry();
        const auto labelRect4 = ui->recent_label       ->geometry();
        const auto labelRect5 = ui->local_label        ->geometry();
        const auto labelRect6 = ui->paid_label         ->geometry();
        const auto labelRect7 = ui->cloud_disk_label   ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->all_label         ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->love_label        ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos3 = ui->song_list_label   ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos4 = ui->recent_label      ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos5 = ui->local_label       ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos6 = ui->paid_label        ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos7 = ui->cloud_disk_label  ->parentWidget()->mapFrom(this, event->pos());

        if (labelRect1.contains(clickPos1)) {
            //qDebug() << "Label clicked!";
            ui->all_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->local_pushButton->clicked();
        }
        if (labelRect3.contains(clickPos3)) {
            ui->song_list_pushButton->clicked();
        }
        if (labelRect4.contains(clickPos4)) {
            ui->recent_pushButton->clicked();
        }
        if (labelRect5.contains(clickPos5)) {
            ui->local_pushButton->clicked();
        }
        if (labelRect6.contains(clickPos6)) {
            ui->paid_pushButton->clicked();
        }
        if (labelRect7.contains(clickPos7)) {
            ui->cloud_disk_pushButton->clicked();
        }
    }
}
