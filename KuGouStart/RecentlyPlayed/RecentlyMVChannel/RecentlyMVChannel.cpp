/**
* @file RecentlyMVChannel.cpp
 * @brief 实现 RecentlyMVChannel 类，管理最近MV频道界面
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#include "RecentlyMVChannel.h"
#include "ui_RecentlyMVChannel.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近MV频道界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlyMVChannel::RecentlyMVChannel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlyMVChannel)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/mv.css")); ///< 加载样式表
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
}

/**
 * @brief 析构函数，清理资源
 */
RecentlyMVChannel::~RecentlyMVChannel()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索频道信号
 */
void RecentlyMVChannel::on_search_pushButton_clicked()
{
    emit find_more_channel();                           ///< 触发搜索频道信号
}