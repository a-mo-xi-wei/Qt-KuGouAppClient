/**
* @file RecentlySongList.cpp
 * @brief 实现 RecentlySongList 类，管理最近歌单界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#include "RecentlySongList.h"
#include "ui_RecentlySongList.h"
#include "logger.hpp"

#include <QFile>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近歌单界面
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlySongList::RecentlySongList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlySongList)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/list.css")); ///< 加载样式表
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
RecentlySongList::~RecentlySongList()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 搜索按钮点击槽函数
 * @note 触发搜索音乐信号
 */
void RecentlySongList::on_search_pushButton_clicked()
{
    emit find_more_music();                             ///< 触发搜索音乐信号
}