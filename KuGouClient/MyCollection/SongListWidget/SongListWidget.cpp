/**
 * @file SongListWidget.cpp
 * @brief 实现 SongListWidget 类，管理歌单界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "SongListWidget.h"
#include "ui_SongListWidget.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化歌单界面
 * @param parent 父控件指针，默认为 nullptr
 */
SongListWidget::SongListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SongListWidget)
    , m_searchAction(nullptr)
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
SongListWidget::~SongListWidget()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 设置批量操作按钮、搜索栏和事件过滤器
 */
void SongListWidget::initUi()
{
    ui->batch_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/batch-operation-gray.svg"))); ///< 设置批量操作图标
    ui->search_lineEdit->setPlaceholderText(QStringLiteral("搜索")); ///< 设置搜索栏占位文本
    ui->search_lineEdit->setMaxWidth(200);               ///< 设置搜索栏最大宽度
    auto font = QFont("AaSongLiuKaiTi");                ///< 设置搜索栏字体
    font.setWeight(QFont::Bold);
    font.setPointSize(12);                  ///< 设置字体加粗
    ui->search_lineEdit->setFont(font);                 ///< 应用字体
    ui->search_lineEdit->setBorderRadius(10);
    this->m_searchAction = new QAction(QIcon(":/MenuIcon/Res/menuIcon/search-black.svg"), "搜索", ui->search_lineEdit); ///< 创建搜索动作
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加搜索动作到右侧
    QToolButton *searchButton = nullptr;
    foreach (QToolButton *btn, ui->search_lineEdit->findChildren<QToolButton *>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;                          ///< 获取搜索按钮
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建搜索按钮工具提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索")); ///< 设置搜索提示
            break;
        }
    }
    if (searchButton) {
        searchButton->installEventFilter(this);          ///< 安装搜索按钮事件过滤器
    }
    initBlock();                                         ///< 初始化歌单块
}

/**
 * @brief 初始化歌单块
 * @note 添加歌单块到布局
 */
void SongListWidget::initBlock() const
{
    const auto lay = new QHBoxLayout(ui->table_widget);  ///< 创建水平布局
    lay->setSpacing(10);                                 ///< 设置间距
    ui->table_widget->setLayout(lay);                    ///< 设置布局
    const QString arr[] = {"我喜欢", "默认收藏", "默认列表"}; ///< 定义歌单标题
    for (int i = 0; i < 3; ++i) {
        const auto block = new SongListBlockWidget(ui->table_widget); ///< 创建歌单块
        block->setTitleText(arr[i]);                     ///< 设置标题
        if (i == 0) {
            block->setCoverPix(QStringLiteral(":/TabIcon/Res/tabIcon/like.png")); ///< 设置“我喜欢”封面
        } else {
            block->setCoverPix(QStringLiteral(":/TabIcon/Res/tabIcon/playlist.png")); ///< 设置其他封面
        }
        lay->addWidget(block);                           ///< 添加到布局
    }
    lay->addStretch();                                   ///< 添加拉伸项
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换搜索图标颜色
 */
bool SongListWidget::eventFilter(QObject *watched, QEvent *event)
{
    const auto button = qobject_cast<QToolButton *>(watched);
    if (button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 设置蓝色搜索图标
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置黑色搜索图标
        }
    }
    return QObject::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 批量操作按钮点击槽函数
 * @note 显示未实现提示
 */
void SongListWidget::on_batch_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->batch_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 导入按钮点击槽函数
 * @note 显示未实现提示
 */
void SongListWidget::on_import_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能暂未实现 敬请期待").arg(ui->import_toolButton->text()),
                               1000, this->window());    ///< 显示提示
}