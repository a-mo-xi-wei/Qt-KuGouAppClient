/**
 * @file SongList.cpp
 * @brief 实现 SongList 类，管理歌曲列表界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "SongList.h"
#include "ui_SongList.h"
#include "SongBlock.h"
#include "MyFlowLayout.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "Async.h"
#include "RefreshMask.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <chrono>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化歌曲列表
 * @param parent 父控件指针，默认为 nullptr
 */
SongList::SongList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SongList)
    , m_refreshMask(std::make_unique<RefreshMask>(this))                 ///< 初始化刷新遮罩
{
    ui->setupUi(this);                                   ///< 设置 UI 布局
    QFile file(GET_CURRENT_DIR + QStringLiteral("/list.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }

    QTimer::singleShot(100,this,[this]{initUi();}); ///< 初始化界面

    auto menu = new MyMenu(MyMenu::MenuKind::ListOption, this); ///< 创建选项菜单
    m_menu = menu->getMenu<ListOptionMenu>();            ///< 获取菜单
    connect(m_menu, &ListOptionMenu::clickedFuncName, this, &SongList::onMenuFuncClicked); ///< 连接菜单点击信号
    connect(ui->all_toolButton, &QToolButton::clicked, this, &SongList::on_all_toolButton_clicked); ///< 连接全部按钮点击信号
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
SongList::~SongList()
{
    delete ui;                                           ///< 释放 UI 界面
}

/**
 * @brief 初始化界面
 * @note 设置全部按钮、布局和歌曲块
 */
void SongList::initUi()
{
    this->m_refreshMask->keepLoading();
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this] {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/desc.json")); ///< 加载描述文件
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading desc.json"; ///< 记录警告日志
            STREAM_WARN() << "Could not open file for reading desc.json";
            return true;
        }
        auto obj = QJsonDocument::fromJson(file.readAll()); ///< 解析 JSON
        auto arr = obj.array();
        for (const auto &item : arr)
        {
            QString title = item.toObject().value("desc").toString(); ///< 获取描述
            this->m_descVector.emplace_back(title);          ///< 添加描述
        }
        file.close();
        std::sort(m_descVector.begin(), m_descVector.end()); ///< 排序
        auto last = std::unique(m_descVector.begin(), m_descVector.end()); ///< 去重
        m_descVector.erase(last, m_descVector.end());       ///< 删除重复项

        for (int i = 1; i <= 210; ++i)
        {
            this->m_coverVector.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i)); ///< 添加封面图片
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 使用当前时间作为随机种子
        std::shuffle(this->m_coverVector.begin(), this->m_coverVector.end(), std::default_random_engine(seed)); ///< 打乱封面列表
        std::shuffle(this->m_descVector.begin(), this->m_descVector.end(), std::default_random_engine(seed)); ///< 打乱描述列表

        return true;
    });
    Async::onResultReady(future, this, [this](bool flag) {
        auto lay = new MyFlowLayout(ui->table_widget, true, 0);         ///< 创建流动布局
        lay->setContentsMargins(0, 20, 0, 20);               ///< 设置边距
        ui->table_widget->setLayout(lay);                    ///< 设置布局
        const auto size = std::min(this->m_coverVector.size(), this->m_descVector.size()); ///< 获取最小大小
        for (int i = 0; i < size; ++i)
        {
            auto block = new SongBlock(this);         ///< 创建歌曲块
            block->setCoverPix(this->m_coverVector[i]);      ///< 设置封面
            block->setShowTip();                             ///< 显示提示
            block->setDescText(this->m_descVector[i]);       ///< 设置描述
            lay->addWidget(block);                           ///< 添加到布局
        }
        ui->all_toolButton->setMouseTracking(true);          ///< 启用鼠标跟踪
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置默认图标
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置悬停图标
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置离开图标
        ui->all_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF"))); ///< 设置悬停字体颜色
        ui->all_toolButton->setApproach(true);               ///< 启用接近效果
        ui->all_toolButton->setChangeSize(true);             ///< 启用动态大小
        ui->all_toolButton->setEnterIconSize(QSize(10, 10)); ///< 设置悬停图标大小
        ui->all_toolButton->setLeaveIconSize(QSize(10, 10)); ///< 设置离开图标大小

        QList<QToolButton *> buttons = ui->widget->findChildren<QToolButton *>(); ///< 获取所有工具按钮
        for (const auto &button : buttons)
        {
            connect(button, &QToolButton::clicked, this, [this, button] {
                ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                                           QString("%1 功能未实现 敬请期待").arg(button->text()), 1000, this->window()); ///< 显示未实现提示
            });
        }
        this->m_refreshMask->hideLoading("");
    });

}

/**
 * @brief 全部按钮点击槽函数
 * @note 显示菜单并切换图标
 */
void SongList::on_all_toolButton_clicked()
{
    if (ui->all_toolButton->isChecked())
    {
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg"))); ///< 设置向上图标
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg"))); ///< 设置悬停向上图标
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg"))); ///< 设置离开向上图标

        const QPoint globalPos = ui->all_toolButton->mapToGlobal(
            QPoint(ui->all_toolButton->width() * 2 - m_menu->width(),
                   ui->all_toolButton->height() + 10)); ///< 计算菜单位置
        // @note 未使用，保留用于调试
        // const int yPos = ui->all_toolButton->mapToGlobal(QPoint(0,0)).y() + ui->all_toolButton->height() + 10;
        // const int xPos = this->window()->geometry().right() - this->m_menu->width() - 20;
        // this->m_menu->setGeometry(xPos,yPos, this->m_menu->width(), this->m_menu->height());

        m_menu->setFocusPolicy(Qt::NoFocus);            ///< 设置菜单不夺取焦点
        m_menu->setAttribute(Qt::WA_TransparentForMouseEvents, false); ///< 启用鼠标事件

        connect(m_menu, &QMenu::aboutToHide, this, [this] {
            ui->all_toolButton->setChecked(false);       ///< 取消按钮选中状态
            ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 恢复向下图标
            ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 恢复悬停向下图标
            ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 恢复离开向下图标
        });

        m_menu->exec(globalPos);                        ///< 显示菜单
    }
    else
    {
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置向下图标
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg"))); ///< 设置悬停向下图标
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg"))); ///< 设置离开向下图标
    }
}

/**
 * @brief 菜单功能点击槽函数
 * @param funcName 功能名称
 * @note 显示未实现提示
 */
void SongList::onMenuFuncClicked(const QString &funcName)
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(funcName), 1000, this->window()); ///< 显示未实现提示
}

void SongList::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise();  // 确保遮罩在最上层
}

void SongList::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise();  // 确保遮罩在最上层
}
