/**
 * @file DownloadedSong.cpp
 * @brief 实现 DownloadedSong 类，提供已下载歌曲界面功能
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#include "DownloadedSong.h"
#include "ui_DownloadedSong.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QScreen>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化已下载歌曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
DownloadedSong::DownloadedSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadedSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/downloadedsong.css")); ///< 加载样式表
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
    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this); ///< 创建排序菜单
    this->m_sortOptMenu = menu->getMenu<SortOptionMenu>(); ///< 获取排序选项菜单
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
DownloadedSong::~DownloadedSong()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 设置工具提示、图标和搜索动作
 */
void DownloadedSong::initUi()
{
    auto local_sort_toolButton_toolTip = new ElaToolTip(ui->local_sort_toolButton); ///< 创建排序按钮工具提示
    local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 设置默认排序提示
    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        Q_UNUSED(down);
        onDefaultSort();                                 ///< 调用默认排序
        local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onAddTimeSort(down);                             ///< 调用添加时间排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onSongNameSort(down);                            ///< 调用歌曲名称排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onSingerSort(down);                              ///< 调用歌手排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onDurationSort(down);                            ///< 调用时长排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, local_sort_toolButton_toolTip](const bool &down) {
        onPlayCountSort(down);                           ///< 调用播放次数排序
        local_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, local_sort_toolButton_toolTip] {
        onRandomSort();                                  ///< 调用随机排序
        local_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机")); ///< 更新提示
    });
    auto local_batch_toolButton_toolTip = new ElaToolTip(ui->local_batch_toolButton); ///< 创建批量操作按钮工具提示
    local_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作")); ///< 设置批量操作提示
    ui->local_play_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false);   ///< 仅显示图标
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作
    ui->search_lineEdit->setMaxWidth(150);               ///< 设置搜索框最大宽度
    ui->search_lineEdit->setBorderRadius(10);
    auto font = QFont("AaSongLiuKaiTi");                 ///< 设置字体
    font.setWeight(QFont::Bold);
    font.setPointSize(12);                         ///< 设置粗体
    ui->search_lineEdit->setFont(font);                  ///< 应用字体
    QToolButton *searchButton = nullptr;                 ///< 搜索按钮
    foreach (QToolButton *btn, ui->search_lineEdit->findChildren<QToolButton *>())
    {
        if (btn->defaultAction() == this->m_searchAction)
        {
            searchButton = btn;                          ///< 找到搜索按钮
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建搜索按钮工具提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索")); ///< 设置搜索提示
            break;
        }
    }
    if (searchButton)
    {
        searchButton->installEventFilter(this);          ///< 安装事件过滤器
        auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建搜索按钮工具提示
        search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索")); ///< 设置搜索提示
    }
}

/**
 * @brief 获取菜单位置
 * @param pos 鼠标位置
 * @note 动态调整菜单位置以适应屏幕
 */
void DownloadedSong::getMenuPosition(const QPoint &pos)
{
    this->m_menuPosition = pos;                          ///< 设置菜单位置
    const QScreen *screen = QGuiApplication::primaryScreen(); ///< 获取主屏幕
    const QRect screenGeometry = screen->geometry();      ///< 获取屏幕几何
    const int menuRightPos = pos.x() + m_sortOptMenu->width(); ///< 计算菜单右侧位置
    const int menuBottomPos = pos.y() + m_sortOptMenu->height(); ///< 计算菜单底部位置
    if (menuRightPos > screenGeometry.right())
    {
        const int offset = menuRightPos - screenGeometry.right() + 5; ///< 计算右侧偏移
        m_menuPosition.setX(pos.x() - offset);           ///< 调整 X 坐标
    }
    if (menuBottomPos > screenGeometry.bottom())
    {
        const int offset = menuBottomPos - screenGeometry.bottom() + 5; ///< 计算底部偏移
        m_menuPosition.setY(pos.y() - offset);           ///< 调整 Y 坐标
    }
}

/**
 * @brief 播放按钮点击槽函数
 * @note 显示暂无音乐提示
 */
void DownloadedSong::on_local_play_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 排序按钮点击槽函数
 * @note 显示排序菜单
 */
void DownloadedSong::on_local_sort_toolButton_clicked()
{
    getMenuPosition(QCursor::pos());                     ///< 获取菜单位置
    this->m_sortOptMenu->popup(this->m_menuPosition);    ///< 显示排序菜单
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示未实现提示
 */
void DownloadedSong::on_local_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 默认排序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onDefaultSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 添加时间排序
 * @param down 是否降序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onAddTimeSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 歌曲名称排序
 * @param down 是否降序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onSongNameSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 歌手排序
 * @param down 是否降序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onSingerSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 时长排序
 * @param down 是否降序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onDurationSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 播放次数排序
 * @param down 是否降序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onPlayCountSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 随机排序
 * @note 显示暂无音乐提示
 */
void DownloadedSong::onRandomSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", QString("暂无音乐"), 1000, this->window()); ///< 显示警告
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void DownloadedSong::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 触发搜索信号
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理搜索图标的动态切换
 */
bool DownloadedSong::eventFilter(QObject *watched, QEvent *event)
{
    const auto button = qobject_cast<QToolButton *>(watched); ///< 转换为工具按钮
    if (button && button->defaultAction() == this->m_searchAction)
    {
        if (event->type() == QEvent::Enter)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置蓝色搜索图标
        }
        else if (event->type() == QEvent::Leave)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置黑色搜索图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}