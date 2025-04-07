//
// Created by WeiWang on 24-11-15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PaidSingle.h" resolved

#include "PaidSingle.h"
#include "ui_PaidSingle.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QScreen>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PaidSingle::PaidSingle(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaidSingle)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/paid.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    //初始化menu
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this);
    this->m_sortOptMenu = menu->getMenu<SortOptionMenu>();
    initUi();
}

PaidSingle::~PaidSingle() {
    delete ui;
}

void PaidSingle::initUi() {
    //设置toolTip
    {
        // 设置 single_download_toolButton 的 tooltip
        auto single_download_toolButton_toolTip = new ElaToolTip(ui->single_download_toolButton);
        single_download_toolButton_toolTip->setToolTip(QStringLiteral("下载"));
        
        // 设置 paid_search_lineEdit 的 tooltip
        auto paid_search_lineEdit_toolTip = new ElaToolTip(ui->paid_search_lineEdit);
        paid_search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));

        // 设置 single_share_toolButton 的 tooltip
        auto single_share_toolButton_toolTip = new ElaToolTip(ui->single_share_toolButton);
        single_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        // 设置 single_sort_toolButton 的 tooltip
        auto single_sort_toolButton_toolTip = new ElaToolTip(ui->single_sort_toolButton);
        single_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
        {
            // 排序相关
            connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                Q_UNUSED(down);
                onDefaultSort();  // 调用原有的排序槽函数
                single_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                onAddTimeSort(down);  // 调用原有的排序槽函数
                if (down)
                    single_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：添加时间降序"));
                else
                    single_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：添加时间升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                onSongNameSort(down);  // 调用原本的排序槽
                single_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                onSingerSort(down);
                single_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                onDurationSort(down);
                single_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, single_sort_toolButton_toolTip](const bool& down) {
                onPlayCountSort(down);
                single_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序"));
            });

            connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, single_sort_toolButton_toolTip] {
                onRandomSort();
                single_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机"));
            });

        }

        // 设置 single_batch_toolButton 的 tooltip
        auto single_batch_toolButton_toolTip = new ElaToolTip(ui->single_batch_toolButton);
        single_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }
    ui->single_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg")));
    ui->single_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
    ui->single_download_toolButton->installEventFilter(this);
    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false); // 仅显示图标
    ui->paid_search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    ui->paid_search_lineEdit->setWidth(150);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->paid_search_lineEdit->findChildren<QToolButton*>()) {
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

void PaidSingle::getMenuPosition(const QPoint &pos) {
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

void PaidSingle::on_single_play_toolButton_clicked() {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::on_single_sort_toolButton_clicked() {
    getMenuPosition(QCursor::pos());
    this->m_sortOptMenu->move(this->m_menuPosition);
    this->m_sortOptMenu->show();
}

void PaidSingle::on_single_download_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                            "下载 功能暂未实现 敬请期待", 1000,this->window());
}

void PaidSingle::on_single_share_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
        QString("分享 功能暂未实现 敬请期待"),1000,this->window());
}

void PaidSingle::on_single_batch_toolButton_clicked() {
    ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
        QString("批量操作 功能暂未实现 敬请期待"),1000,this->window());
}

void PaidSingle::onDefaultSort() {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onAddTimeSort(const bool &down) {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onSongNameSort(const bool &down) {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onSingerSort(const bool &down) {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onDurationSort(const bool &down) {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onPlayCountSort(const bool &down) {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

void PaidSingle::onRandomSort() {
    ElaMessageBar::warning(ElaMessageBarType::BottomRight,"Warning",
        QString("暂无音乐"),1000,this->window());
}

bool PaidSingle::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->single_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->single_download_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            ui->single_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg")));
        }
    }
    if (const auto button = qobject_cast<QToolButton*>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }

    return QWidget::eventFilter(watched, event);
}
