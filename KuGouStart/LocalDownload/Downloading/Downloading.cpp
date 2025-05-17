/**
 * @file Downloading.cpp
 * @brief 实现 Downloading 类，提供下载管理界面功能
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#include "Downloading.h"
#include "ui_Downloading.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化下载管理界面
 * @param parent 父控件指针，默认为 nullptr
 */
Downloading::Downloading(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Downloading)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/downloading.css")); ///< 加载样式表
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
Downloading::~Downloading()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化界面
 * @note 设置工具提示和图标
 */
void Downloading::initUi()
{
    auto setting_toolButton_toolTip = new ElaToolTip(ui->setting_toolButton); ///< 创建设置按钮工具提示
    setting_toolButton_toolTip->setToolTip(QStringLiteral("下载设置")); ///< 设置下载设置提示
    ui->start_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/play3-white.svg"))); ///< 设置开始按钮图标
    ui->stop_toolButton->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/stop-gray.svg"))); ///< 设置停止按钮图标
    ui->clear_toolButton->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg"))); ///< 设置清除按钮图标
}

/**
 * @brief 开始按钮点击槽函数
 * @note 显示暂无下载提示
 */
void Downloading::on_start_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无正在下载音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 停止按钮点击槽函数
 * @note 显示暂无下载提示
 */
void Downloading::on_stop_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无正在下载音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 清除按钮点击槽函数
 * @note 显示暂无下载提示
 */
void Downloading::on_clear_toolButton_clicked()
{
    ElaMessageBar::warning(ElaMessageBarType::BottomRight, "Warning", "暂无正在下载音乐", 1000, this->window()); ///< 显示警告
}

/**
 * @brief 设置按钮点击槽函数
 * @note 显示未实现提示
 */
void Downloading::on_setting_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info", "下载设置 功能暂未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void Downloading::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 触发搜索信号
}