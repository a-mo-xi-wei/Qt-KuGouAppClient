/**
 * @file DownloadingWidget.cpp
 * @brief 实现 DownloadingWidget 类，提供下载中界面功能
 * @author WeiWang
 * @date 2025-02-04
 * @version 1.0
 */

#include "DownloadingWidget.h"
#include "ui_DownloadingWidget.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化下载中界面
 * @param parent 父控件指针，默认为 nullptr
 */
DownloadingWidget::DownloadingWidget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::DownloadingWidget)
{
    ui->setupUi(this); ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloading.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll()); ///< 应用样式表
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
            return;
        }
    }
    initUi(); ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
DownloadingWidget::~DownloadingWidget()
{
    delete ui;
}

/**
 * @brief 初始化界面
 * @note 设置按钮图标和工具提示
 */
void DownloadingWidget::initUi()
{
    ui->option_widget->setStyleSheet("font-family: 'TaiwanPearl';font-size: 13px;");
    ui->property_widget->setStyleSheet("font-family: 'TaiwanPearl';font-size: 13px;");
    auto downloading_setting_toolButton_toolTip = new
        ElaToolTip(ui->downloading_setting_toolButton); ///< 创建设置按钮工具提示
    downloading_setting_toolButton_toolTip->setToolTip(ui->downloading_setting_toolButton->text());
    ///< 设置工具提示内容
    ui->downloading_play_toolButton->setIcon(
        QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg"))); ///< 设置播放按钮图标
    ui->downloading_pause_toolButton->setIcon(
        QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/stop-gray.svg"))); ///< 设置暂停按钮图标
    ui->downloading_clear_toolButton->setIcon(
        QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/delete-black.svg"))); ///< 设置清除按钮图标
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发查找更多有声书的信号
 */
void DownloadingWidget::on_search_pushButton_clicked()
{
    emit find_more_audio_book(); ///< 发出查找信号
}

void DownloadingWidget::on_downloading_play_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 暂未实现").arg(ui->downloading_play_toolButton->text()),
                               1000,
                               this->window()); ///< 显示提示
}

void DownloadingWidget::on_downloading_pause_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 暂未实现").arg(ui->downloading_pause_toolButton->text()),
                               1000,
                               this->window()); ///< 显示提示
}

void DownloadingWidget::on_downloading_clear_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 暂未实现").arg(ui->downloading_clear_toolButton->text()),
                               1000,
                               this->window()); ///< 显示提示
}

void DownloadingWidget::on_downloading_setting_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 暂未实现").arg(ui->downloading_setting_toolButton->text()),
                               1000,
                               this->window()); ///< 显示提示
}