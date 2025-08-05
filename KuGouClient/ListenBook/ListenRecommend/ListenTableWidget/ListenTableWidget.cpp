/**
 * @file ListenTableWidget.cpp
 * @brief 实现 ListenTableWidget 类，提供表格控件功能
 * @author WeiWang
 * @date 2025-02-07
 * @version 1.0
 */

#include "ListenTableWidget.h"
#include "ui_ListenTableWidget.h"

/**
 * @brief 构造函数，初始化表格控件
 * @param parent 父控件指针，默认为 nullptr
 */
ListenTableWidget::ListenTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ListenTableWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
ListenTableWidget::~ListenTableWidget()
{
    delete ui;
}

/**
 * @brief 设置标题
 * @param title 标题文本
 */
void ListenTableWidget::setTitle(const QString &title) const
{
    ui->title_label->setText(title);                     ///< 设置标题文本
}

/**
 * @brief 获取画廊控件
 * @return 画廊控件指针
 */
GalleryWidget* ListenTableWidget::getGalleryWidget() const
{
    return ui->gallery_widget;                           ///< 返回画廊控件
}

/**
 * @brief 设置计数
 * @param cnt 计数值
 */
void ListenTableWidget::setCnt(const int& cnt)
{
    this->m_cnt = cnt;                                   ///< 设置计数值
}

/**
 * @brief 获取计数
 * @return 计数值
 */
int ListenTableWidget::getCnt() const
{
    return this->m_cnt;                                  ///< 返回计数值
}

/**
 * @brief 获取标题
 * @return 标题文本
 */
QString ListenTableWidget::getTitle() const
{
    return ui->title_label->text();                      ///< 返回标题文本
}

/**
 * @brief 初始化界面
 * @note 设置刷新按钮图标和样式
 */
void ListenTableWidget::initUi()
{
    ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg"))); ///< 设置默认图标
    ui->toolButton->setStyleSheet(R"(
        QToolButton#toolButton{
            font-family: 'TaiwanPearl';
            font-size: 13px;
            background-color: transparent;
            color: black;
        }
        QToolButton#toolButton:hover{
            color: #26A1FF;
        }
    )");                                                ///< 设置按钮样式
    ui->toolButton->installEventFilter(this);            ///< 安装事件过滤器
}

/**
 * @brief 事件过滤器，处理刷新按钮的鼠标事件
 * @param watched 监视的对象
 * @param event 事件
 * @return 是否处理事件
 */
bool ListenTableWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->toolButton)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-blue.svg"))); ///< 设置悬停图标
            return true;
        }
        if (event->type() == QEvent::Leave)
        {
            ui->toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/refresh-gray.svg"))); ///< 恢复默认图标
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类事件过滤器
}

/**
 * @brief 刷新按钮点击槽函数
 * @note 触发刷新信号
 */
void ListenTableWidget::on_toolButton_clicked()
{
    emit toolBtnClicked();                               ///< 发出刷新信号
}