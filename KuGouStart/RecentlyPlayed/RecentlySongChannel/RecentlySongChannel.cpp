/**
 * @file RecentlySongChannel.cpp
 * @brief 实现 RecentlySongChannel 类，管理最近歌曲频道界面
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#include "RecentlySongChannel.h"
#include "ui_RecentlySongChannel.h"
#include "RecentlyChannelBlock.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近歌曲频道界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlySongChannel::RecentlySongChannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlySongChannel)
    , m_searchAction(new QAction(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/song.css")); ///< 加载样式表
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
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
RecentlySongChannel::~RecentlySongChannel()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 初始化工具提示、搜索框和频道块
 */
void RecentlySongChannel::initUi()
{
    auto recently_share_toolButton_toolTip = new ElaToolTip(ui->recently_share_toolButton); ///< 分享按钮工具提示
    recently_share_toolButton_toolTip->setToolTip(QStringLiteral("分享"));
    auto recently_batch_toolButton_toolTip = new ElaToolTip(ui->recently_batch_toolButton); ///< 批量操作按钮工具提示
    recently_batch_toolButton_toolTip->setToolTip(QStringLiteral("批量操作"));
    ui->recently_play_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索动作图标
    this->m_searchAction->setIconVisibleInMenu(false); ///< 仅显示图标
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作到搜索框
    ui->search_lineEdit->setMaxWidth(150);              ///< 设置搜索框最大宽度
    ui->search_lineEdit->setBorderRadius(10);
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
    const auto lay = new QHBoxLayout(ui->table_widget);  ///< 创建频道块布局
    const auto block = new RecentlyChannelBlock(ui->table_widget); ///< 创建频道块
    block->setCoverPix(QStringLiteral(":/Res/tabIcon/guess-you-love.jpg")); ///< 设置封面图片
    lay->addWidget(block);                               ///< 添加频道块
    lay->addStretch();                                   ///< 添加拉伸
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换搜索图标
 */
bool RecentlySongChannel::eventFilter(QObject *watched, QEvent *event)
{
    if (const auto button = qobject_cast<QToolButton *>(watched); button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置搜索按钮悬停图标
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置搜索按钮默认图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 播放按钮点击槽函数
 * @note 显示无音乐提示
 */
void RecentlySongChannel::on_recently_play_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 分享按钮点击槽函数
 * @note 显示功能未实现提示
 */
void RecentlySongChannel::on_recently_share_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示信息
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示功能未实现提示
 */
void RecentlySongChannel::on_recently_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "批量操作 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示信息
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索频道信号
 */
void RecentlySongChannel::on_search_pushButton_clicked()
{
    emit find_more_channel();                           ///< 触发搜索频道信号
}