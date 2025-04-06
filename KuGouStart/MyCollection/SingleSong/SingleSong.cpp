//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleSong.h" resolved

#include "SingleSong.h"
#include "ui_SingleSong.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QScreen>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

SingleSong::SingleSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SingleSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/single.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

SingleSong::~SingleSong() {
    delete ui;
}

void SingleSong::initUi() {
    //设置toolTip
    {
        //collect_search_lineEdit
        auto collect_search_lineEdit_toolTip = new ElaToolTip(ui->collect_search_lineEdit);
        collect_search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));

        // 设置 collect_download_toolButton 的 tooltip
        auto collect_download_toolButton_toolTip = new ElaToolTip(ui->collect_download_toolButton);
        collect_download_toolButton_toolTip->setToolTip(QStringLiteral("下载"));

        // 设置 collect_share_toolButton 的 tooltip
        auto collect_share_toolButton_toolTip = new ElaToolTip(ui->collect_share_toolButton);
        collect_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        // 设置 collect_zhuanji_toolButton 的 tooltip
        auto collect_zhuanji_toolButton_toolTip = new ElaToolTip(ui->collect_zhuanji_toolButton);
        collect_zhuanji_toolButton_toolTip->setToolTip(QStringLiteral("专辑"));

        // 设置 collect_sort_toolButton 的 tooltip
        auto collect_sort_toolButton_toolTip = new ElaToolTip(ui->collect_sort_toolButton);
        collect_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));

        // 设置 collect_batch_toolButton 的 tooltip
        auto collect_batch_toolButton_toolTip = new ElaToolTip(ui->collect_batch_toolButton);
        collect_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }

    ui->collect_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->collect_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
    ui->collect_download_toolButton->installEventFilter(this);

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->collect_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->collect_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->collect_search_lineEdit->findChildren<QToolButton*>()) {
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
}

void SingleSong::getMenuPosition(const QPoint &pos) {
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

void SingleSong::on_collect_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->move(this->m_menuPosition);
    this->m_sortOptMenu->show();
}

bool SingleSong::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->collect_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->collect_download_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            ui->collect_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
        }
    }
    if (const auto button = qobject_cast<QToolButton*>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }

    return QObject::eventFilter(watched, event);
}
