//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UploadedSong.h" resolved

#include "UploadedSong.h"
#include "ui_UploadedSong.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QScreen>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

UploadedSong::UploadedSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadedSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/uploaded.css"));
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

UploadedSong::~UploadedSong() {
    delete ui;
}

void UploadedSong::initUi() {
    //设置toolTip
    {
        // 设置 cloud_search_lineEdit 的 tooltip
        auto cloud_search_lineEdit_toolTip = new ElaToolTip(ui->cloud_search_lineEdit);
        cloud_search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));

        // 设置 cloud_share_toolButton 的 tooltip
        auto cloud_share_toolButton_toolTip = new ElaToolTip(ui->cloud_share_toolButton);
        cloud_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        // 设置 cloud_sort_toolButton 的 tooltip
        auto cloud_sort_toolButton_toolTip = new ElaToolTip(ui->cloud_sort_toolButton);
        cloud_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));

        // 设置 cloud_batch_toolButton 的 tooltip
        auto cloud_batch_toolButton_toolTip = new ElaToolTip(ui->cloud_batch_toolButton);
        cloud_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }
    ui->cloud_play_toolButton->setIcon(QIcon(QStringLiteral(":Res/tabIcon/play3-gray.svg")));
    ui->cloud_upload_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-white.svg")));
    ui->cloud_download_toolButton->setIcon(QIcon(QStringLiteral(":Res/tabIcon/download-gray.svg")));
    ui->cloud_delete_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-gray.svg")));

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->cloud_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->cloud_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->cloud_search_lineEdit->findChildren<QToolButton*>()) {
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

void UploadedSong::getMenuPosition(const QPoint &pos) {
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

void UploadedSong::on_cloud_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->move(this->m_menuPosition);
    this->m_sortOptMenu->show();
}

bool UploadedSong::eventFilter(QObject *watched, QEvent *event) {
    const auto button = qobject_cast<QToolButton*>(watched);
    if (button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }
    return QObject::eventFilter(watched, event);
}
