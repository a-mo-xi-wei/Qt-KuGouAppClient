//
// Created by WeiWang on 25-1-31.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlySongChannel.h" resolved

#include "RecentlySongChannel.h"
#include "ui_RecentlySongChannel.h"
#include "RecentlyChannelBlock.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecentlySongChannel::RecentlySongChannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlySongChannel)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/song.css"));
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

RecentlySongChannel::~RecentlySongChannel() {
    delete ui;
}

void RecentlySongChannel::initUi() {
    //设置toolTip
    {
        // 设置 recently_search_lineEdit 的 tooltip
        auto recently_search_lineEdit_toolTip = new ElaToolTip(ui->recently_search_lineEdit);
        recently_search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));

        // 设置 recently_share_toolButton 的 tooltip
        auto recently_share_toolButton_toolTip = new ElaToolTip(ui->recently_share_toolButton);
        recently_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        // 设置 recently_batch_toolButton 的 tooltip
        auto recently_batch_toolButton_toolTip = new ElaToolTip(ui->recently_batch_toolButton);
        recently_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }
    ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    //处理搜索栏
    {
        //使用 addAction 添加右侧图标
        this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
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
    //处理RecentlyChannelBlock
    const auto  lay = new QHBoxLayout(ui->table_widget);
    const auto block = new RecentlyChannelBlock(ui->table_widget);
    block->setCoverPix(QStringLiteral(":/Res/tabIcon/guess-you-love.jpg"));
    lay->addWidget(block);
    lay->addStretch();
}

bool RecentlySongChannel::eventFilter(QObject *watched, QEvent *event) {
    if (const auto button = qobject_cast<QToolButton*>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }

    return QObject::eventFilter(watched, event);
}
