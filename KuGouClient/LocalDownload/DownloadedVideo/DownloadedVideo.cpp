/**
* @file DownloadedVideo.cpp
 * @brief 实现 DownloadedVideo 类，提供已下载视频界面功能
 * @author WeiWang
 * @date 2025-01-27
 * @version 1.0
 */

#include "DownloadedVideo.h"
#include "ui_DownloadedVideo.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化已下载视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
DownloadedVideo::DownloadedVideo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadedVideo)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/downloadedvideo.css")); ///< 加载样式表
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
    QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);

}

/**
 * @brief 析构函数，清理资源
 */
DownloadedVideo::~DownloadedVideo()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索信号
 */
void DownloadedVideo::on_search_pushButton_clicked()
{
    emit find_more_music();                              ///< 触发搜索信号
}