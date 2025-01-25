//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SingleSong.h" resolved

#include "SingleSong.h"
#include "ui_SingleSong.h"

#include <QFile>

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
            return;
        }
    }
    initUi();
}

SingleSong::~SingleSong() {
    delete ui;
}

void SingleSong::initUi() {
    ui->collect_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->collect_add_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/add-gray.svg")));
    ui->collect_upload_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/upload-cloud-gray.svg")));

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/titlebar/search-black.svg")));
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
}

bool SingleSong::eventFilter(QObject *watched, QEvent *event) {
    const auto button = qobject_cast<QToolButton*>(watched);
    if (button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/search-black.svg")));
        }
    }
    return QObject::eventFilter(watched, event);
}
