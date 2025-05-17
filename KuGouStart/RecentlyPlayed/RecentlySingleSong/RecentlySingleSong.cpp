/**
 * @file RecentlySingleSong.cpp
 * @brief 实现 RecentlySingleSong 类，管理最近单曲界面
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#include "RecentlySingleSong.h"
#include "ui_RecentlySingleSong.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QScreen>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近单曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlySingleSong::RecentlySingleSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlySingleSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/single.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this); ///< 初始化排序菜单
    this->m_sortOptMenu = menu->getMenu<SortOptionMenu>(); ///< 获取排序菜单
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
RecentlySingleSong::~RecentlySingleSong()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 初始化工具按钮、搜索框、工具提示和事件过滤器
 */
void RecentlySingleSong::initUi()
{
    auto recently_download_toolTip = new ElaToolTip(ui->recently_download_toolButton); ///< 下载按钮工具提示
    recently_download_toolTip->setToolTip(QStringLiteral("下载"));
    auto recently_share_toolButton_toolTip = new ElaToolTip(ui->recently_share_toolButton); ///< 分享按钮工具提示
    recently_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));
    auto recently_sort_toolButton_toolTip = new ElaToolTip(ui->recently_sort_toolButton); ///< 排序按钮工具提示
    recently_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        Q_UNUSED(down);
        onDefaultSort();                                 ///< 调用默认排序
        recently_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        onAddTimeSort(down);                             ///< 调用添加时间排序
        recently_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        onSongNameSort(down);                            ///< 调用歌曲名称排序
        recently_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        onSingerSort(down);                              ///< 调用歌手排序
        recently_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        onDurationSort(down);                            ///< 调用时长排序
        recently_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, recently_sort_toolButton_toolTip](const bool &down) {
        onPlayCountSort(down);                           ///< 调用播放次数排序
        recently_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序")); ///< 更新排序提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, recently_sort_toolButton_toolTip] {
        onRandomSort();                                  ///< 调用随机排序
        recently_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机")); ///< 更新排序提示
    });
    auto recently_batch_toolButton_toolTip = new ElaToolTip(ui->recently_batch_toolButton); ///< 批量操作按钮工具提示
    recently_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg"))); ///< 设置下载按钮图标
    ui->recently_download_toolButton->installEventFilter(this); ///< 安装下载按钮事件过滤器
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false); ///< 仅显示图标
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作到搜索框
    ui->search_lineEdit->setMaxWidth(150);              ///< 设置搜索框最大宽度
    auto font = QFont("AaSongLiuKaiTi");                ///< 设置搜索框字体
    font.setWeight(QFont::Bold);
    ui->search_lineEdit->setFont(font);
    QToolButton *searchButton = nullptr;
    foreach (QToolButton *btn, ui->search_lineEdit->findChildren<QToolButton *>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 搜索按钮工具提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));
            break;
        }
    }
    if (searchButton) {
        searchButton->installEventFilter(this);          ///< 安装搜索按钮事件过滤器
    }
}

/**
 * @brief 获取排序菜单位置
 * @param pos 鼠标位置
 * @note 调整菜单位置以适应屏幕边界
 */
void RecentlySingleSong::getMenuPosition(const QPoint &pos)
{
    this->m_menuPosition = pos;
    const QScreen *screen = QGuiApplication::primaryScreen(); ///< 获取主屏幕
    const QRect screenGeometry = screen->geometry();     ///< 获取屏幕尺寸
    const int menuRightPos = pos.x() + m_sortOptMenu->width(); ///< 计算菜单右侧位置
    const int menuBottomPos = pos.y() + m_sortOptMenu->height(); ///< 计算菜单底部位置
    if (menuRightPos > screenGeometry.right()) {
        const int offset = menuRightPos - screenGeometry.right() + 5; ///< 计算右侧偏移
        m_menuPosition.setX(pos.x() - offset);           ///< 调整X坐标
    }
    if (menuBottomPos > screenGeometry.bottom()) {
        const int offset = menuBottomPos - screenGeometry.bottom() + 5; ///< 计算底部偏移
        m_menuPosition.setY(pos.y() - offset);           ///< 调整Y坐标
    }
}

/**
 * @brief 播放按钮点击槽函数
 * @note 显示无音乐提示
 */
void RecentlySingleSong::on_recently_play_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 下载按钮点击槽函数
 * @note 显示功能未实现提示
 */
void RecentlySingleSong::on_recently_download_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "下载 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示信息
}

/**
 * @brief 分享按钮点击槽函数
 * @note 显示功能未实现提示
 */
void RecentlySingleSong::on_recently_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "分享 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示信息
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示功能未实现提示
 */
void RecentlySingleSong::on_recently_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示信息
}

/**
 * @brief 排序按钮点击槽函数
 * @note 弹出排序菜单
 */
void RecentlySingleSong::on_recently_sort_toolButton_clicked()
{
    getMenuPosition(QCursor::pos());                    ///< 获取菜单位置
    this->m_sortOptMenu->popup(this->m_menuPosition);   ///< 弹出排序菜单
}

/**
 * @brief 默认排序槽函数
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onDefaultSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 添加时间排序槽函数
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onAddTimeSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 歌曲名称排序槽函数
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onSongNameSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 歌手排序槽函数
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onSingerSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 时长排序槽函数
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onDurationSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 播放次数排序槽函数
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onPlayCountSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 随机排序槽函数
 * @note 显示无音乐提示
 */
void RecentlySingleSong::onRandomSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void RecentlySingleSong::on_search_pushButton_clicked()
{
    emit find_more_music();                             ///< 触发搜索信号
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换下载和搜索图标
 */
bool RecentlySingleSong::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->recently_download_toolButton) {
        if (event->type() == QEvent::Enter) {
            ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg"))); ///< 设置下载按钮悬停图标
        } else if (event->type() == QEvent::Leave) {
            ui->recently_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg"))); ///< 设置下载按钮默认图标
        }
    }
    if (const auto button = qobject_cast<QToolButton *>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置搜索按钮悬停图标
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索按钮默认图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}