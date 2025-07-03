/**
* @file DownloadedWidget.cpp
 * @brief 实现 DownloadedWidget 类，提供已下载界面功能
 * @author WeiWang
 * @date 2025-02-04
 * @version 1.0
 */

#include "DownloadedWidget.h"
#include "ui_DownloadedWidget.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化已下载界面
 * @param parent 父控件指针，默认为 nullptr
 */
DownloadedWidget::DownloadedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadedWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/downloaded.css")); ///< 加载样式表
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
}

/**
 * @brief 析构函数，清理资源
 */
DownloadedWidget::~DownloadedWidget()
{
    delete ui;
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发查找更多有声书的信号
 */
void DownloadedWidget::on_search_pushButton_clicked()
{
    emit find_more_audio_book();                         ///< 发出查找信号
}