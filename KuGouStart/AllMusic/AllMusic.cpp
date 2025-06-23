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
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/all.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
    }

    const auto menu = new MyMenu(MyMenu::MenuKind::SortOption, this); ///< 初始化排序菜单
    this->m_sortOptMenu = menu->getMenu<SortOptionMenu>(); ///< 获取排序选项菜单

    initUi();                                            ///< 初始化界面

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true);                              ///< 动画结束时启用按钮
    });
    enableButton(true);                                  ///< 初始启用按钮
}

/**
 * @brief 析构函数，清理资源
 */
AllMusic::~AllMusic()
{
    delete ui;
}

/**
 * @brief 初始化界面
 */
void AllMusic::initUi()
{
    {
        auto all_download_toolButton_toolTip = new ElaToolTip(ui->all_download_toolButton); ///< 创建下载按钮工具提示
        all_download_toolButton_toolTip->setToolTip(QStringLiteral("下载"));

        auto all_share_toolButton_toolTip = new ElaToolTip(ui->all_share_toolButton); ///< 创建分享按钮工具提示
        all_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));

        auto all_sort_toolButton_toolTip = new ElaToolTip(ui->all_sort_toolButton); ///< 创建排序按钮工具提示
        all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序"));
        {
            connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                Q_UNUSED(down);
                onDefaultSort();                         ///< 调用默认排序
                all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                onAddTimeSort(down);                     ///< 调用添加时间排序
                all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                onSongNameSort(down);                    ///< 调用歌曲名称排序
                all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                onSingerSort(down);                      ///< 调用歌手排序
                all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                onDurationSort(down);                    ///< 调用时长排序
                all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, all_sort_toolButton_toolTip](const bool &down) {
                onPlayCountSort(down);                   ///< 调用播放次数排序
                all_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序")); ///< 更新工具提示
            });

            connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, all_sort_toolButton_toolTip] {
                onRandomSort();                          ///< 调用随机排序
                all_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机")); ///< 更新工具提示
            });
        }

        auto all_batch_toolButton_toolTip = new ElaToolTip(ui->all_batch_toolButton); ///< 创建批量操作按钮工具提示
        all_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    }
    initStackedWidget();                                 ///< 初始化堆栈窗口
    initIndexLab();                                      ///< 初始化索引标签
    ui->all_pushButton->clicked();                       ///< 默认触发全部按钮
    ui->all_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg"))); ///< 设置下载按钮图标
    ui->all_download_toolButton->installEventFilter(this); ///< 安装下载按钮事件过滤器

    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false);   ///< 仅显示图标
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作到搜索框
    ui->search_lineEdit->setMaxWidth(150);               ///< 设置搜索框最大宽度
    ui->search_lineEdit->setBorderRadius(10);
    auto font = QFont("AaSongLiuKaiTi");                 ///< 设置搜索框字体
    font.setWeight(QFont::Bold);
    ui->search_lineEdit->setFont(font);
    QToolButton *searchButton = nullptr;
    foreach (QToolButton *btn, ui->search_lineEdit->findChildren<QToolButton *>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建搜索按钮工具提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索"));
            break;
        }
    }

    if (searchButton) {
        searchButton->installEventFilter(this);          ///< 安装搜索按钮事件过滤器
    }

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart); ///< 设置堆栈窗口动画曲线
    ui->stackedWidget->setSpeed(400);                    ///< 设置动画速度（400ms）
}

/**
 * @brief 初始化索引标签
 */
void AllMusic::initIndexLab()
{
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签1图片
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签2图片
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签3图片
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签4图片
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签5图片
    ui->idx6_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签6图片
    ui->idx7_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg"))); ///< 设置索引标签7图片
    ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置全部标签样式
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->guide_widget1->installEventFilter(this);         ///< 安装导航控件1事件过滤器
    ui->guide_widget2->installEventFilter(this);         ///< 安装导航控件2事件过滤器
    ui->guide_widget3->installEventFilter(this);         ///< 安装导航控件3事件过滤器
    ui->guide_widget4->installEventFilter(this);         ///< 安装导航控件4事件过滤器
    ui->guide_widget5->installEventFilter(this);         ///< 安装导航控件5事件过滤器
    ui->guide_widget6->installEventFilter(this);         ///< 安装导航控件6事件过滤器
    ui->guide_widget7->installEventFilter(this);         ///< 安装导航控件7事件过滤器
}

/**
 * @brief 初始化堆栈窗口
 */
void AllMusic::initStackedWidget()
{
    initAllWidget();                                     ///< 初始化全部音乐控件
    initAllLove();                                       ///< 初始化收藏音乐控件
    initAllSongList();                                   ///< 初始化歌单控件
    initAllRecent();                                     ///< 初始化最近播放控件
    initAllLocal();                                      ///< 初始化本地音乐控件
    initAllPaid();                                       ///< 初始化已购音乐控件
    initAllCloudDisk();                                  ///< 初始化云盘音乐控件
    this->m_buttonGroup->addButton(ui->all_pushButton);  ///< 添加全部按钮
    this->m_buttonGroup->addButton(ui->love_pushButton); ///< 添加收藏按钮
    this->m_buttonGroup->addButton(ui->song_list_pushButton); ///< 添加歌单按钮
    this->m_buttonGroup->addButton(ui->recent_pushButton); ///< 添加最近按钮
    this->m_buttonGroup->addButton(ui->local_pushButton); ///< 添加本地按钮
    this->m_buttonGroup->addButton(ui->paid_pushButton); ///< 添加已购按钮
    this->m_buttonGroup->addButton(ui->cloud_disk_pushButton); ///< 添加云盘按钮
    this->m_buttonGroup->setExclusive(true);             ///< 设置按钮组互斥
}

/**
 * @brief 初始化全部音乐控件
 */
void AllMusic::initAllWidget()
{
    this->m_allWidget = std::make_unique<AllWidget>(ui->stackedWidget); ///< 创建全部音乐控件
    connect(this->m_allWidget.get(), &AllWidget::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allWidget.get()); ///< 添加到堆栈窗口
    ui->stackedWidget->setCurrentWidget(this->m_allWidget.get()); ///< 设置默认控件
}

/**
 * @brief 初始化收藏音乐控件
 */
void AllMusic::initAllLove()
{
    this->m_allLove = std::make_unique<AllLove>(ui->stackedWidget); ///< 创建收藏音乐控件
    connect(this->m_allLove.get(), &AllLove::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allLove.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化歌单控件
 */
void AllMusic::initAllSongList()
{
    this->m_allSongList = std::make_unique<AllSongList>(ui->stackedWidget); ///< 创建歌单控件
    connect(this->m_allSongList.get(), &AllSongList::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allSongList.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化最近播放控件
 */
void AllMusic::initAllRecent()
{
    this->m_allRecent = std::make_unique<AllRecent>(ui->stackedWidget); ///< 创建最近播放控件
    connect(this->m_allRecent.get(), &AllRecent::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allRecent.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化本地音乐控件
 */
void AllMusic::initAllLocal()
{
    this->m_allLocal = std::make_unique<AllLocal>(ui->stackedWidget); ///< 创建本地音乐控件
    connect(this->m_allLocal.get(), &AllLocal::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allLocal.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化已购音乐控件
 */
void AllMusic::initAllPaid()
{
    this->m_allPaid = std::make_unique<AllPaid>(ui->stackedWidget); ///< 创建已购音乐控件
    connect(this->m_allPaid.get(), &AllPaid::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allPaid.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 初始化云盘音乐控件
 */
void AllMusic::initAllCloudDisk()
{
    this->m_allCloudDisk = std::make_unique<AllCloudDisk>(ui->stackedWidget); ///< 创建云盘音乐控件
    connect(this->m_allCloudDisk.get(), &AllCloudDisk::find_more_music, this, [this] { emit find_more_music(); }); ///< 连接搜索信号
    ui->stackedWidget->addWidget(this->m_allCloudDisk.get()); ///< 添加到堆栈窗口
}

/**
 * @brief 获取菜单显示位置
 * @param pos 鼠标位置
 */
void AllMusic::getMenuPosition(const QPoint &pos)
{
    this->m_menuPosition = pos;                          ///< 记录菜单位置
    const QScreen *screen = QGuiApplication::primaryScreen(); ///< 获取主屏幕
    const QRect screenGeometry = screen->geometry();     ///< 获取屏幕尺寸

    const int menuRightPos = pos.x() + m_sortOptMenu->width(); ///< 计算菜单右侧位置
    const int menuBottomPos = pos.y() + m_sortOptMenu->height(); ///< 计算菜单底部位置
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        const int offset = menuRightPos - screenGeometry.right() + 5; ///< 计算右侧超出的偏移
        m_menuPosition.setX(pos.x() - offset);           ///< 调整菜单X坐标
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        const int offset = menuBottomPos - screenGeometry.bottom() + 5; ///< 计算底部超出的偏移
        m_menuPosition.setY(pos.y() - offset);           ///< 调整菜单Y坐标
    }
}

/**
 * @brief 启用或禁用按钮
 * @param flag 是否启用
 */
void AllMusic::enableButton(const bool &flag) const
{
    ui->all_pushButton->setEnabled(flag);                ///< 设置全部按钮状态
    ui->love_pushButton->setEnabled(flag);               ///< 设置收藏按钮状态
    ui->song_list_pushButton->setEnabled(flag);          ///< 设置歌单按钮状态
    ui->recent_pushButton->setEnabled(flag);             ///< 设置最近按钮状态
    ui->local_pushButton->setEnabled(flag);              ///< 设置本地按钮状态
    ui->paid_pushButton->setEnabled(flag);               ///< 设置已购按钮状态
    ui->cloud_disk_pushButton->setEnabled(flag);         ///< 设置云盘按钮状态
}

/**
 * @brief 处理全部按钮点击
 */
void AllMusic::on_all_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allWidget.get())
        return;
    ui->all_pushButton->setChecked(true);                ///< 设置按钮选中
    STREAM_INFO() << "切换全部界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allWidget.get())); ///< 切换到全部控件
    ui->idx1_lab->show();                                ///< 显示索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理收藏按钮点击
 */
void AllMusic::on_love_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allLove.get())
        return;
    ui->love_pushButton->setChecked(true);               ///< 设置按钮选中
    STREAM_INFO() << "切换喜欢界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allLove.get())); ///< 切换到收藏控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->show();                                ///< 显示索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理歌单按钮点击
 */
void AllMusic::on_song_list_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allSongList.get())
        return;
    ui->song_list_pushButton->setChecked(true);          ///< 设置按钮选中
    STREAM_INFO() << "切换歌单界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allSongList.get())); ///< 切换到歌单控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->show();                                ///< 显示索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理最近按钮点击
 */
void AllMusic::on_recent_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allRecent.get())
        return;
    ui->recent_pushButton->setChecked(true);             ///< 设置按钮选中
    STREAM_INFO() << "切换最近界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allRecent.get())); ///< 切换到最近控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->show();                                ///< 显示索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理本地按钮点击
 */
void AllMusic::on_local_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allLocal.get())
        return;
    ui->local_pushButton->setChecked(true);              ///< 设置按钮选中
    STREAM_INFO() << "切换本地界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allLocal.get())); ///< 切换到本地控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->show();                                ///< 显示索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理已购按钮点击
 */
void AllMusic::on_paid_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allPaid.get())
        return;
    ui->paid_pushButton->setChecked(true);               ///< 设置按钮选中
    STREAM_INFO() << "切换已购界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allPaid.get())); ///< 切换到已购控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->show();                                ///< 显示索引标签6
    ui->idx7_lab->hide();                                ///< 隐藏索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置已购标签样式
    ui->cloud_disk_label->setStyleSheet("");             ///< 清除云盘标签样式
}

/**
 * @brief 处理云盘按钮点击
 */
void AllMusic::on_cloud_disk_pushButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == this->m_allCloudDisk.get())
        return;
    ui->cloud_disk_pushButton->setChecked(true);         ///< 设置按钮选中
    STREAM_INFO() << "切换云盘界面";                     ///< 记录切换日志
    enableButton(false);                                 ///< 禁用按钮
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_allCloudDisk.get())); ///< 切换到云盘控件
    ui->idx1_lab->hide();                                ///< 隐藏索引标签1
    ui->idx2_lab->hide();                                ///< 隐藏索引标签2
    ui->idx3_lab->hide();                                ///< 隐藏索引标签3
    ui->idx4_lab->hide();                                ///< 隐藏索引标签4
    ui->idx5_lab->hide();                                ///< 隐藏索引标签5
    ui->idx6_lab->hide();                                ///< 隐藏索引标签6
    ui->idx7_lab->show();                                ///< 显示索引标签7
    ui->all_label->setStyleSheet("");                    ///< 清除全部标签样式
    ui->love_label->setStyleSheet("");                   ///< 清除收藏标签样式
    ui->song_list_label->setStyleSheet("");              ///< 清除歌单标签样式
    ui->recent_label->setStyleSheet("");                 ///< 清除最近标签样式
    ui->local_label->setStyleSheet("");                  ///< 清除本地标签样式
    ui->paid_label->setStyleSheet("");                   ///< 清除已购标签样式
    ui->cloud_disk_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;")); ///< 设置云盘标签样式
}

/**
 * @brief 处理全部播放按钮点击
 */
void AllMusic::on_all_play_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 处理下载按钮点击
 */
void AllMusic::on_all_download_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "下载 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示功能未实现提示
}

/**
 * @brief 处理分享按钮点击
 */
void AllMusic::on_all_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "分享 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示功能未实现提示
}

/**
 * @brief 处理批量操作按钮点击
 */
void AllMusic::on_all_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示功能未实现提示
}

/**
 * @brief 处理排序按钮点击
 */
void AllMusic::on_all_sort_toolButton_clicked()
{
    getMenuPosition(QCursor::pos());                     ///< 获取菜单位置
    this->m_sortOptMenu->popup(this->m_menuPosition);    ///< 弹出排序菜单
}

/**
 * @brief 默认排序
 */
void AllMusic::onDefaultSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 按添加时间排序
 * @param down 是否降序
 */
void AllMusic::onAddTimeSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 按歌曲名称排序
 * @param down 是否降序
 */
void AllMusic::onSongNameSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 按歌手排序
 * @param down 是否降序
 */
void AllMusic::onSingerSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 按时长排序
 * @param down 是否降序
 */
void AllMusic::onDurationSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 按播放次数排序
 * @param down 是否降序
 */
void AllMusic::onPlayCountSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
}

/**
 * @brief 随机排序
 */
void AllMusic::onRandomSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示暂无音乐提示
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
            ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/download-blue.svg"))); ///< 设置下载按钮悬停图标
        } else if (event->type() == QEvent::Leave) {
            ui->all_download_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/download-gray.svg"))); ///< 恢复下载按钮默认图标
        }
    }
    if (const auto button = qobject_cast<QToolButton *>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置搜索按钮悬停图标
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 恢复搜索按钮默认图标
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
    return QWidget::eventFilter(watched, event);         ///< 调用父类事件过滤器
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 */
void AllMusic::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);                     ///< 调用父类鼠标按下事件
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->all_label->geometry(); ///< 获取全部标签区域
        const auto labelRect2 = ui->love_label->geometry(); ///< 获取收藏标签区域
        const auto labelRect3 = ui->song_list_label->geometry(); ///< 获取歌单标签区域
        const auto labelRect4 = ui->recent_label->geometry(); ///< 获取最近标签区域
        const auto labelRect5 = ui->local_label->geometry(); ///< 获取本地标签区域
        const auto labelRect6 = ui->paid_label->geometry(); ///< 获取已购标签区域
        const auto labelRect7 = ui->cloud_disk_label->geometry(); ///< 获取云盘标签区域
        const QPoint clickPos1 = ui->all_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换全部标签点击坐标
        const QPoint clickPos2 = ui->love_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换收藏标签点击坐标
        const QPoint clickPos3 = ui->song_list_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换歌单标签点击坐标
        const QPoint clickPos4 = ui->recent_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换最近标签点击坐标
        const QPoint clickPos5 = ui->local_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换本地标签点击坐标
        const QPoint clickPos6 = ui->paid_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换已购标签点击坐标
        const QPoint clickPos7 = ui->cloud_disk_label->parentWidget()->mapFrom(this, event->pos()); ///< 转换云盘标签点击坐标

        if (labelRect1.contains(clickPos1)) {
            ui->all_pushButton->clicked();               ///< 触发全部按钮点击
        }
        if (labelRect2.contains(clickPos2)) {
            ui->love_pushButton->clicked();              ///< 触发收藏按钮点击
        }
        if (labelRect3.contains(clickPos3)) {
            ui->song_list_pushButton->clicked();         ///< 触发歌单按钮点击
        }
        if (labelRect4.contains(clickPos4)) {
            ui->recent_pushButton->clicked();            ///< 触发最近按钮点击
        }
        if (labelRect5.contains(clickPos5)) {
            ui->local_pushButton->clicked();             ///< 触发本地按钮点击
        }
        if (labelRect6.contains(clickPos6)) {
            ui->paid_pushButton->clicked();              ///< 触发已购按钮点击
        }
        if (labelRect7.contains(clickPos7)) {
            ui->cloud_disk_pushButton->clicked();        ///< 触发云盘按钮点击
        }
    }
}