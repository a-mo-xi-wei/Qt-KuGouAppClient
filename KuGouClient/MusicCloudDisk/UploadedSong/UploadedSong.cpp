/**
 * @file UploadedSong.cpp
 * @brief 实现 UploadedSong 类，管理已上传云端歌曲界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "UploadedSong.h"
#include "ui_UploadedSong.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化已上传歌曲界面
 * @param parent 父控件指针，默认为 nullptr
 */
UploadedSong::UploadedSong(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadedSong)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/uploaded.css")); ///< 加载样式表
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
UploadedSong::~UploadedSong()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 设置工具提示、图标、搜索动作和排序连接
 */
void UploadedSong::initUi() {
    auto cloud_share_toolButton_toolTip = new ElaToolTip(ui->cloud_share_toolButton); ///< 创建分享按钮工具提示
    cloud_share_toolButton_toolTip->setToolTip(QStringLiteral("分享")); ///< 设置分享提示
    auto cloud_sort_toolButton_toolTip = new ElaToolTip(ui->cloud_sort_toolButton); ///< 创建排序按钮工具提示
    cloud_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 设置默认排序提示
    connect(m_sortOptMenu, &SortOptionMenu::defaultSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        Q_UNUSED(down);
        onDefaultSort();                                 ///< 调用默认排序
        cloud_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：默认排序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::addTimeSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        onAddTimeSort(down);                             ///< 调用添加时间排序
        cloud_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：添加时间降序") : QStringLiteral("当前排序方式：添加时间升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::songNameSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        onSongNameSort(down);                            ///< 调用歌曲名称排序
        cloud_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌曲名称降序") : QStringLiteral("当前排序方式：歌曲名称升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::singerSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        onSingerSort(down);                              ///< 调用歌手排序
        cloud_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：歌手降序") : QStringLiteral("当前排序方式：歌手升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::durationSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        onDurationSort(down);                            ///< 调用时长排序
        cloud_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：时长降序") : QStringLiteral("当前排序方式：时长升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::playCountSort, this, [this, cloud_sort_toolButton_toolTip](const bool &down) {
        onPlayCountSort(down);                           ///< 调用播放次数排序
        cloud_sort_toolButton_toolTip->setToolTip(down ? QStringLiteral("当前排序方式：播放次数降序") : QStringLiteral("当前排序方式：播放次数升序")); ///< 更新提示
    });
    connect(m_sortOptMenu, &SortOptionMenu::randomSort, this, [this, cloud_sort_toolButton_toolTip] {
        onRandomSort();                                  ///< 调用随机排序
        cloud_sort_toolButton_toolTip->setToolTip(QStringLiteral("当前排序方式：随机")); ///< 更新提示
    });
    auto cloud_batch_toolButton_toolTip = new ElaToolTip(ui->cloud_batch_toolButton); ///< 创建批量操作按钮工具提示
    cloud_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作")); ///< 设置批量操作提示
    ui->cloud_play_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-gray.svg"))); ///< 设置播放按钮图标
    ui->cloud_upload_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/upload-white.svg"))); ///< 设置上传按钮图标
    ui->cloud_download_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/download-gray.svg"))); ///< 设置下载按钮图标
    ui->cloud_delete_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-gray.svg"))); ///< 设置删除按钮图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false);   ///< 仅显示图标
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作
    ui->search_lineEdit->setMaxWidth(150);               ///< 设置搜索框最大宽度
    auto font = QFont("AaSongLiuKaiTi");                 ///< 设置字体
    font.setWeight(QFont::Bold);                         ///< 设置粗体
    ui->search_lineEdit->setFont(font);                  ///< 应用字体
    ui->search_lineEdit->setBorderRadius(10);
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
    }
}

/**
 * @brief 排序按钮点击槽函数
 * @note 显示排序菜单
 */
void UploadedSong::on_cloud_sort_toolButton_clicked()
{
    this->m_sortOptMenu->exec(QCursor::pos());           ///< 显示菜单
}

/**
 * @brief 上传按钮点击槽函数
 * @note 显示未实现提示
 */
void UploadedSong::on_cloud_upload_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->cloud_upload_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 下载按钮点击槽函数
 * @note 显示无音乐提示
 */
void UploadedSong::on_cloud_download_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           "暂无音乐", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 删除按钮点击槽函数
 * @note 显示无音乐提示
 */
void UploadedSong::on_cloud_delete_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           "暂无音乐", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 分享按钮点击槽函数
 * @note 显示未实现提示
 */
void UploadedSong::on_cloud_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "分享 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示未实现提示
 */
void UploadedSong::on_cloud_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 默认排序
 * @note 显示无音乐提示
 */
void UploadedSong::onDefaultSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 添加时间排序
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void UploadedSong::onAddTimeSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 歌曲名称排序
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void UploadedSong::onSongNameSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 歌手排序
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void UploadedSong::onSingerSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 时长排序
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void UploadedSong::onDurationSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 播放次数排序
 * @param down 是否降序
 * @note 显示无音乐提示
 */
void UploadedSong::onPlayCountSort(const bool &down)
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 随机排序
 * @note 显示无音乐提示
 */
void UploadedSong::onRandomSort()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning",
                           QString("暂无音乐"), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void UploadedSong::on_search_pushButton_clicked()
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
bool UploadedSong::eventFilter(QObject *watched, QEvent *event)
{
    const auto button = qobject_cast<QToolButton *>(watched); ///< 转换为工具按钮
    if (button && button->defaultAction() == this->m_searchAction)
    {
        if (event->type() == QEvent::Enter)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置蓝色图标
        }
        else if (event->type() == QEvent::Leave)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置黑色图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}