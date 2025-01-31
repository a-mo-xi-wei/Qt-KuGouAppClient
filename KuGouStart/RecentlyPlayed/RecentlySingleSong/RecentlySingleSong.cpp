//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySingleSong.h" resolved

#include "RecentlySingleSong.h"
#include "ui_RecentlySingleSong.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecentlySingleSong::RecentlySingleSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlySingleSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/single.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

RecentlySingleSong::~RecentlySingleSong() {
    delete ui;
}

void RecentlySingleSong::initUi() {
    ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
    ui->recently_download_toolButton->installEventFilter(this);

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->recently_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->recently_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->recently_search_lineEdit->findChildren<QToolButton*>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            break;
        }
    }

    // 安装事件过滤器
    if (searchButton) {
        searchButton->installEventFilter(this);
    }
}

bool RecentlySingleSong::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->recently_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/download-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
        }
    }
    if (const auto button = qobject_cast<QToolButton*>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/search-black.svg")));
        }
    }

    return QObject::eventFilter(watched, event);
}
